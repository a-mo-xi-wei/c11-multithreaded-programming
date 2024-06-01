#include<iostream>
#include<thread>
#include<list>
#include<mutex>
using namespace std;

class A
{
public:
	std::unique_lock<mutex>move_lock()
	{
		std::unique_lock<mutex>temp(m_mutex1);
		return temp;
	}
	void inMsgRecvQueue()
	{
		for (int i = 1; i <= 10000; i++)
		{
			cout << "inMsgRecvQueue函数执行，拿到锁，插入一个元素：" << i << endl;
			//std::unique_lock<mutex>g1(m_mutex1);
			std::unique_lock<mutex>g2 = move_lock();
			msgRecvQueue.push_back(i);
		}
	}
	bool outMsgLUL(int& command)
	{

		//std::lock_guard<mutex>g1(m_mutex1); 
		std::unique_lock<mutex>g1(m_mutex1);
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));//单位毫秒
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
	//	unique_lock所有权的传递
	//	std::unique_lock<mutex>g1(m_mutex1);  g1拥有m_mutex1的所有权
	//	g1可以把对m_mutex1的所有权转移给其他的unique_lock对象（所有权可以转移不能复制）
	//  可以使用std::move()转移（简单直接）
	//	或者自己写个函数返回std::unique_lock<mutex>
 
	return 0;
}
