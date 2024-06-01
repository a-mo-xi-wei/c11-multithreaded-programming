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
			cout << "inMsgRecvQueue函数执行，拿到锁，插入一个元素：" << i << endl;
			std::unique_lock<mutex>g1(m_mutex1, std::defer_lock);//defer 推迟
			g1.lock();//不用自己unlock
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
	thread inth(&A::inMsgRecvQueue, &a);
	thread outth(&A::outMsgRecvQueue, &a);
	inth.join();
	outth.join();

	//	三、std::defer_lock（第三个参数）同上，不得提前lock()
	//	作用是：提供一个没有加锁的mutex//延迟lock


	return 0;
}
