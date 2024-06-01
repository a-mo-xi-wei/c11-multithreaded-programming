#include<iostream>
#include<thread>
#include<mutex>
#include<list>
#include<future>
#include<Windows.h>

using namespace std;

class A
{
public:
	void inMsgRecvQueue()
	{
		for (int i = 1; i <= 10000; i++)
		{
			cout << "inmsgRecvQueue函数执行，插入一个元素" << i << endl;
			std::lock_guard<std::timed_mutex>g1(m_mutex);
			//m_mutex.lock();
			std::chrono::milliseconds timeout(100);
			//if (m_mutex.try_lock_for(timeout))//等待100毫秒来尝试获取锁
			if (m_mutex.try_lock_until(std::chrono::steady_clock::now()+timeout))	
			{
				msgRecvQueue.push_back(i);
				m_mutex.unlock();
			}
			else
			{
				std::this_thread::sleep_for(100ms);
			}
			//m_mutex.unlock();
		}
	}
	bool outMsgLUL(int& command)
	{
		m_mutex.lock();
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			m_mutex.unlock();
			return true;
		}
		m_mutex.unlock();
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

private:
	//std::mutex m_mutex;
	std::timed_mutex m_mutex;
	list<int>msgRecvQueue;
};


int main()
{
	//	带超时的互斥量 std::timed_mutex     std::recursive_timed_mutex
	//	std::timed_mutex 带超时功能的独占互斥量
		//try_lock_for()	等待一段时间，如果拿到了锁或者时间超时，
		//try_lock_until()	参数是一个未来的时间点，在这个时间点之前拿到了锁就走下来，如果时间到了没拿到锁，也走下来


	A a;
	std::thread th1(&A::inMsgRecvQueue, &a);
	std::thread th2(&A::outMsgRecvQueue, &a);
	th1.join();
	th2.join();

	return 0;

}