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
			std::unique_lock<mutex>g1(m_mutex1, std::defer_lock);
			//g1.lock();//不用自己unlock
			if (g1.try_lock())
				msgRecvQueue.push_back(i);
			else cout << "inMsgRecvQueue函数执行，但没有拿到锁，只能干点别的事。。。" << i << endl;
		}
	}
	bool outMsgLUL(int& command)
	{

		//std::lock_guard<mutex>g1(m_mutex1); 
		std::unique_lock<mutex>g1(m_mutex1);
		std::mutex* ptr = g1.release();//现在有责任自己解锁这个m_mutex1
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));//单位毫秒
		if (!msgRecvQueue.empty())
		{
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			ptr->unlock();
			return true;
		}
		ptr->unlock();
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

	//	四、unique_lock的成员函数
	//	1.lock()
	//	2.unlock()
	//	3.try_lock()
	//	4.release() ,返回它管理的mutex对象指针，并释放所有权，也就是说这个unique_lock 与 mutex不在有关系
	//	严格区分unlock()与release()的区别
	//	如果原来mutex对象处于加锁状态，你有责任接管过来并负责解锁（release返回的是原始的mutex指针）
	//	为什么有时候需要unlock() :因为你lock锁住的代码段越少，执行越快，整个程序运行效率越高
	//  有人也把锁头锁住的代码多少称为锁的粒度,粒度一般用粗细来描述;
		//a)锁住的代码少,这个粒度叫细。执行效率高;
		//b)锁住的代码多,粒度叫粗,那执行效率就低;
		//要学会尽量选择合适粒度的代码进行保护，粒度太细，可能漏掉共享数据的保护，粒度太粗，影响效率。
		//选择合适的粒度,是高级程序员的能力和实力的体现;

	return 0;
}
