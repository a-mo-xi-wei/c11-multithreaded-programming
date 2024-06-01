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
			cout << "inMsgRecvQueue函数执行，插入一个元素：" << i << endl;
			//std::lock_guard<mutex>g1(m_mutex1);
			std::unique_lock<mutex>g1(m_mutex1,try_to_lock);
			if (g1.owns_lock())//拿到锁
			{
				cout << "inMsgRecvQueue函数执行，拿到锁，插入一个元素：" << i << endl;
				msgRecvQueue.push_back(i);
			}
			else
			{
				cout << "inMsgRecvQueue函数执行，但没有拿到锁，只能干点别的事。。。" << i << endl;
			}
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
	thread inth(&A::inMsgRecvQueue, &a);
	thread outth(&A::outMsgRecvQueue, &a);
	inth.join();
	outth.join();

	//	二、std::try_to_lock（第二个参数）
	//	尝试使用mutex的lock()去锁定这个线程，但如果没有锁定成功，也会立即返回，并不会阻塞在哪里
	//	用try_to_lock的前提是自己不能去lock(),否则我就会lock()两次

	

	return 0;
}
