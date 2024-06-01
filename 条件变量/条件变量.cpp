#include<iostream>
#include<mutex>
#include<thread>
#include<list>
using namespace std;

class A
{
public:
	void inMsgRecvQueue()
	{
		for (int i = 1; i <= 10000; i++)
		{
			cout << "inMsgRecvQueue()函数执行，插入一个元素：" << i << endl;
			unique_lock<mutex>L1(m_mutex);
			msgRecvQueue.push_back(i);
			my_cond.notify_one();//我们尝试把wait()的线程唤醒，执行完这行outMsgQueue里的wait()就会被唤醒，并且解锁
		}
	}
	void outMsgRecvQueue()
	{
		int command = 0;
		while (true)
		{
			unique_lock<mutex>g1(m_mutex);
			//wait()用来等一个东西
			//如果第二个参数返回值是true,那么wait()直接返回 ，并上锁。
			//如果第二个参数返回值是false,那么wait()将解锁互斥量，并阻塞到本行（休眠）
			//阻塞到其他某个线程调用notify_one()成员函数为止
			//如果没有第二个参数，则默认第二个参数返回false即解锁互斥量
			my_cond.wait(g1, [this]//被notify_one唤醒后wait()不断地尝试获取互斥量锁，若获取不到则一直卡在这行，获取到则上锁
				{
					if (!msgRecvQueue.empty())return true;
					else return false;
				});//存在的瑕疵是inMsgRecvQueue（）执行完后本线程就再也无法唤醒了
			//流程只要能走到这里来，那么互斥量一定是上锁的
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			g1.unlock();//因为unique_lock的灵活性，我们可以随时的unlock()解锁，以免锁住太长时间
			cout << "outMsgRecvQueue()函数执行，取出一个元素" << command << endl;
			//执行一些其他代码。。。。。。
			// 。。。。
			// 执行100ms
			//很可能在执行以上100ms的时间内notify_one又在尝试唤醒wait()但没有什么用，因为它一直醒着
		}

	}



private:
	list<int>msgRecvQueue;
	mutex m_mutex;
	std::condition_variable my_cond;//生成一个条件变量对象
};


int main()
{
	//条件变量  std::condition_variable 、wait()、notify_one()
	//线程A: 等待一个条件满足
	//线程B: 专门往消息队列中仍消息（数据）
	//std::condition_variable实际上是一个类，是一个和条件相关的一个类，说白了就是等待一个条件达成
	//这个类是需要和互斥量来配合工作，用的时候我们要生成这个类的对象
	A a;
	thread th1(&A::inMsgRecvQueue, &a);
	thread th2(&A::outMsgRecvQueue, &a);
	th1.join();
	th2.join();
	return 0;
}