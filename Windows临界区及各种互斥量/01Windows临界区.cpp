#include<iostream>
#include<thread>
#include<mutex>
#include<list>
#include<future>
#include<Windows.h>

#define _WINDOWSJQ_

using namespace std;

class A
{
public:
	void inMsgRecvQueue()
	{
		for (int i = 1; i <= 10000; i++)
		{
			cout << "inmsgRecvQueue函数执行，插入一个元素" << i<<endl;

#ifdef _WINDOWSJQ_
			EnterCriticalSection(&my_winsec);
			msgRecvQueue.push_back(i);
			LeaveCriticalSection(&my_winsec);
#else
			m_mutex.lock();
			msgRecvQueue.push_back(i);
			m_mutex.unlock();

#endif 
		}
	}
	bool outMsgLUL(int &command)
	{
#ifdef _WINDOWSJQ_
		EnterCriticalSection(&my_winsec);
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			LeaveCriticalSection(&my_winsec);
			return true;
		}
		LeaveCriticalSection(&my_winsec);
#else 
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			m_mutex.unlock();
			return true; 
		}
		m_mutex.unlock();
#endif
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
				cout << "outMsgRecvQueue函数执行，取出一个元素：" << command << endl;

			}
			else
			{
				cout << "outMsgRecvQueue函数执行，取出一个元素：" << i << endl;
			}
		}
		cout << "End" << endl;
	}

	A()
	{
#ifdef _WINDOWSJQ_ 
		InitializeCriticalSection(&my_winsec);//用临界区之前要先初始化
#endif 
	}


private:
	std::mutex m_mutex;
	list<int>msgRecvQueue;

#ifdef _WINDOWSJQ_
	CRITICAL_SECTION my_winsec;// windows中的临界区，非常类似于 mutex
#endif
};


int main()
{


	A a;
	std::thread th1(&A::inMsgRecvQueue,&a);
	std::thread th2(&A::outMsgRecvQueue,&a);
	th1.join();
	th2.join();

	return 0;

}