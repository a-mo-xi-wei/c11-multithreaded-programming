#include<iostream>
#include<thread>
#include<list>
#include<mutex>
using namespace std;

class A
{
public:
	void inMsgRecvQueue()
	{
		for (int i = 1; i <= 10000; i++)
		{
			std::unique_lock<mutex>g1(m_mutex1);
			cout << "inMsgRecvQueue函数执行，拿到锁，插入一个元素：" << i << endl;
			msgRecvQueue.push_back(i);
		}
	}
	bool outMsgLUL(int& command)
	{
		std::unique_lock<mutex>g1(m_mutex1);
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			return true;
		}
		return false;
	}
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 1; i <= 10000; i++)
		{
			bool result = outMsgLUL(command);
			if (result)
			{
				cout << "outMsgRecvQueue函数执行，取出一个元素" << endl;
			}
			else
			{
				cout << "outMsgRecvQueue执行，但是消息队列为空。。。" << endl;
			}
		}
		cout << "end" << endl;
	}

private:
	std::mutex m_mutex1;
	std::mutex m_mutex2;
	list<int>msgRecvQueue;
};
int main()
{
	A a;
	thread inth(&A::inMsgRecvQueue,&a);
	thread outth(&A::outMsgRecvQueue,&a);
	inth.join();
	outth.join();

	//	unique_lock是个类模板，工作中一般使用lock_guard（推荐使用）（取代了mutex的lock()和unlock()
	//	unique_lock比lock_guard灵活很多，但在效率上差一点，内存占用多一点。
	 
	//	一、unique_lock可以直接替换Lock_guard
	//	lock_guard可以带第二个参数:	std::adopt_lock	必须得提前lock()
	//	std::lock_guard<std::mutex> sbguard1(m_mutexl, std::adopt_lock);
	//	adopt_lock标记作用;
	//	std::adopt_lock:表示这个互斥量已经被lock了(你必须要把互斥量提前lock了,否则会报异常)

	return 0;
}
