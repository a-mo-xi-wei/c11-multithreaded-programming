#include<iostream>
#include<thread>
#include<vector>
#include<list>
#include<mutex>
using namespace std;


class A
{
public:
	//把收到的消息（命令）放入一个队列线程
	void inMagRecvQueue()
	{
		for (int i = 0; i < 10000; ++i)
		{
			cout << "inMagRecvQueue（）执行，插入一个元素" << i << endl;
			{
				std::lock_guard<std::mutex>guard(m_mutex1);
				//m_mutex.lock();
				msgRecvQueue.push_back(i);//假设这个数字就是收到的命令
				//m_mutex.unlock();
			}//此处使用大括号可以适当提升效率
		}
	}

	bool outMsgLUL(int &command)
	{
		std::lock_guard<std::mutex>guard(m_mutex1);
		//只需要以上一条语句就行的原因是：lock_guard构造函数里执行了mutex::lock()
		//guard局部变量析构函数里执行了 mutex::unlock()
		//但这是有代价的，lock_guard锁住了整个函数！降低了效率



		//m_mutex.lock();//不能放在if里面因为if判断的时候就已经访问了共享数据
		if (!msgRecvQueue.empty())
		{
			//消息不为空
			command = msgRecvQueue.front(); //返回第一个元素，但不进行检查，因此要判断是否为空

			msgRecvQueue.pop_front();//移除，不返回
			//m_mutex.unlock();
			return true;
		}
		//m_mutex.unlock();
		return false;
	}
	//把数据从消息队列中取出的线程
	void outMagRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 10000; ++i)
		{
			bool result = outMsgLUL(command);
			if (result)
			{
				cout << "outMagRecvQueue()执行,取出一个元素"  << endl;
			}
			else
			{
				cout << "outMagRecvQueue()执行，但目前消息队列为空。。。" << i << endl;
			}
		}
		cout << "end" << endl;
	}
private:
	list<int>msgRecvQueue;
	std::mutex m_mutex1;//创建一个互斥量
};
int main()
{
	
	A a;
	std::thread thout(&A::outMagRecvQueue, &a);//第二个参数是对象的地址，因为成员函数的运行需要this指针
	std::thread thin(&A::inMagRecvQueue, &a);
	thin.join();
	thout.join();
		
	//保护共享数据，操作时用代码把共享数据锁住，其他想操作共享数据的线程必须等待解锁
	//一、互斥量（Mutex）的基本概念
	//互斥量是类对象，可以理解成一把锁
	//多个线程尝试用lock()成员函数加锁这把锁，只有一个线程能锁定成功
	//成功的标志是lock()函数成功，否则流程卡在lock(),不断地去尝试加锁这把锁
	//	互斥量的使用要小心，保护数据不能多也不能少，少了没达到保护效果，多了影响效率

	//互斥量的用法
	//先lock() 操作共享数据 再unlock()	必须成对出现	一旦忘记有可能非常难排查
	//为防止忘记unlock(),引入一个叫做std::lock_guard的类模板	保姆

	//lock_guard类模板可直接取代lock与unlock,即使用lock_guard后就不能使用lock与unlock()


	return 0;
}