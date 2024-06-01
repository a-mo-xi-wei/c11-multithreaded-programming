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
			std::lock_guard<std::recursive_mutex>g1(m_mutex);
			testfunc1();//加了三次锁，报异常（只要lock超过一次就会报异常）
			//m_mutex.lock();
			msgRecvQueue.push_back(i);
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

	void testfunc1()
	{
		std::lock_guard<std::recursive_mutex>g1(m_mutex);
		//。。。。。干各种事情
		testfunc2();
	}
	void testfunc2()
	{
		std::lock_guard<std::recursive_mutex>g1(m_mutex);
		//。。。。。干另外一些事情

	}

private:
	//std::mutex m_mutex;
	std::recursive_mutex m_mutex;
	list<int>msgRecvQueue;
};


int main()
{
	//	recursive_mutex递归的独占互斥量	允许同一个线程，同一个互斥量多次被lock	效率上比mutex要差一些
	//  当用到recursive_mutex时，就要考虑代码是否存在优化空间
	//	mutex 独占互斥量，自己lock时，别人lock不了
	A a;
	std::thread th1(&A::inMsgRecvQueue, &a);
	std::thread th2(&A::outMsgRecvQueue, &a);
	th1.join();
	th2.join();

	return 0;

}