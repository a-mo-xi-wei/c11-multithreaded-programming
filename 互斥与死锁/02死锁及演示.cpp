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
			//m_mutex2.lock();//在实际工程中，这两个锁不一定挨着，可能他们需要保护不同的数据
			//m_mutex1.lock();
			std::lock(m_mutex1, m_mutex2);
			std::lock_guard<mutex>g1(m_mutex1, std::adopt_lock);
			std::lock_guard<mutex>g2(m_mutex2, std::adopt_lock);
			//m_mutex2.lock();//在实际工程中，这两个锁不一定挨着，可能他们需要保护不同的数据
			msgRecvQueue.push_back(i);//假设这个数字就是收到的命令
			//m_mutex2.unlock();
			//m_mutex1.unlock();
		}	
	}

	bool outMsgLUL(int& command)
	{
		//m_mutex2.lock();
		//m_mutex1.lock();
		std::lock(m_mutex1, m_mutex2);
		std::lock_guard<mutex>g1(m_mutex1, std::adopt_lock);
		std::lock_guard<mutex>g2(m_mutex2, std::adopt_lock);
		if (!msgRecvQueue.empty())
		{
			//消息不为空
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			//m_mutex1.unlock();
			//m_mutex2.unlock();
			return true;
		}
		
		//m_mutex1.unlock();
		//m_mutex2.unlock();
		return false;
	}
	void outMagRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 10000; ++i)
		{
			bool result = outMsgLUL(command);
			if (result)
			{
				cout << "outMagRecvQueue()执行,取出一个元素" << endl;
			}
			else
			{
				cout << "outMagRecvQueue()执行，但目前消息队列为空。。。" << endl;
			}
		}
		cout << "end" << endl;
	}
private:
	list<int>msgRecvQueue;
	std::mutex m_mutex1;//一个互斥量一把锁
	std::mutex m_mutex2;
};
int main()
{

	A a;
	std::thread thout(&A::outMagRecvQueue, &a);
	std::thread thin(&A::inMagRecvQueue, &a);
	thin.join();
	thout.join();

	//死锁
	//张三：站在北京 等李四，不挪窝
	//李四：站在深圳 等张三，不挪窝
	//在C++中死锁产生的前提条件是有两个及以上的锁（互斥量）

	//假设有两个线程 A B	两个锁 金锁和银锁
	//(⑴)线程A执行的时候，这个线程先锁金锁，被金锁lock()成功了，然后它去lock银锁。。。
	//出现了上下文切换
	//(2)线程B执行了，这个线程先锁银锁，因为银锁还没有锁住其他线程，所以银锁会lock0成功，线程B要去lock金锁。。。。
	// //此时此刻,死锁就产生了;
	// (3)线程A因为拿不到银锁，流程走不下去，因为后面有需要银锁的，线程B同理

	//	死锁的一般解决方案
	//	一、保证这些互斥量上锁的顺序是一样的，就不会导致死锁

	//	二、std::lock()函数模板（一次性锁住两个及两个以上的互斥量）//多个锁才出场
	//	它不存在这种因为在多个线程中因为所的顺序而导致的死锁的问题，
	//  其特点是要么这几个互斥量都锁住要么都没锁住，一个没锁住，就立马把已经锁住的释放掉
	//	由于一般不同的锁保护的是不同的数据，因此一般不会紧挨在一起，也就不会一起锁住，因此	慎用！！！！！！
	
	//	三、sdt::lock_guard<mutex>g(m_mutex1,std::adopt_lock);//可以解决第二个方法有时会遗漏unlock()的问题




	return 0;
}