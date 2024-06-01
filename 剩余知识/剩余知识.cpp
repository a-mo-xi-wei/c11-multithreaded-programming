#include<iostream>
#include<thread>
#include<mutex>
#include<future>
#include<list>

using namespace std;


class A
{
public:
	atomic<int>atm;
	A()
	{
		atm = 0;
		//auto atm1 = atm;//不允许使用赋值拷贝操作，因为对于=操作使用了=delete
		// load():原子方式读atomic对象的值
		atomic<int>atm1(atm.load());
		//store()以原子方式写入内容
		atm1.store(10);

	}

	void outMsgRecvQueue()
	{
		int command = 0;
		while (true)
		{
			std:unique_lock<mutex>g1(m_mutex);
			m_cond.wait(g1, [this]
				{
					if (!msgRecvQueue.empty())return true;//能够很好地处理虚假唤醒
					return false;
				});
			//若是单纯的以为被唤醒就一定不为空会造成
			command = msgRecvQueue.front();
			msgRecvQueue.pop_front();
			g1.unlock();
			cout << "outMsgRecvQueue函数执行，取出一个元素 " << command << endl;
		}
	}
	void inMsgRecvQueue()
	{
		for (int i = 1; i <= 10000; i++)
		{
			cout << "inMsgRecvQueue函数执行，插入一个元素 " << i << endl;
			std::unique_lock<mutex>g1(m_mutex);
			msgRecvQueue.push_back(i);
			m_cond.notify_one();//可能存在唤醒时msgRecvQueue中并没有内容  这就是虚假唤醒
		}
	}

private:;
	   std::mutex m_mutex;
	   std::condition_variable m_cond;
	   list<int>msgRecvQueue;
};

int main()
{
	//	虚假唤醒：wait中要有第二参数（lambda）并且这个lambda要正确判断要处理的公共数据是否存在
	A a;
	std::thread th1(&A::inMsgRecvQueue, &a);
	std::thread th2(&A::outMsgRecvQueue, &a);
	th1.join();
	th2.join();


	//二:浅谈线程池//（2.1）场景设想
	//服务器程序，--》客户端，每来一个客户端，就创建一个新线程为该客户提供服务。
	//a)网络游戏，2万玩家不可能给每个玩家创建个新线程,此程序写法在这种场景下不通;
	//b)程序稳定性问题:编写的代码中，偶尔创建一个线程这种代码，这种写法，就让人感到不安;
	//线程池:把一堆线程弄到一起，统一管理。这种统一管理调度，循环利用线程的方式，就叫线程池;
	//实现方式
	//在程序启时，我一次性的创建好一定数量的线程。10，8，100-200,更让人放心，觉得程序代码更稳定;
	// 三:线程创建数量谈
	// (3.1）线程开的数量极限问题  2000个线程基本就是极限
	//	线程创建数量建议
	//a)采用某些技术开发程序; api接口提供商建议你创建线程数量= cpu数量，cpu *2 , opu*2+2，遵照专业建议和指示来，专业意见
	//b)创建多线程完成业务;一个线程等于一条执行通路;100要堵塞充值，我们这里开110个线程，那是很合适的;
	//c)1800个线程，建议，线程数量尽量不要超过500个，能控制在200个之内;


}