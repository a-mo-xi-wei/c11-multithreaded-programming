#include<iostream>
#include<thread>
#include<mutex>
#include<future>

using namespace std;

//int my_count = 0;//定义一个全局变量
std::atomic<int>my_count = 0;//封装了一个类型为int的对象,可以像操作一个int类型变量一样来操作这个my_count
//std::mutex m_mutex;

std::atomic<bool>g_ifend = false;//线程退出标记

//void mythread()
//{
//	std::chrono::milliseconds dura(1000);
//	while (g_ifend == false)
//	{
//		//系统未要求线程退出，想干嘛干嘛
//		cout << "thread id= " << std::this_thread::get_id() << " 运行中。。。" << endl;
//		std::this_thread::sleep_for(dura);
//	}
//	cout << "thread id= " << std::this_thread::get_id() << " 运行结束。" << endl;
//
//}

void mythread()
{
	for (int i = 0; i < 100; i++)
	{
		//m_mutex.lock();
		my_count++;//不会被打断
		//my_count+=1;
		//my_count = my_count + 1;//错误
		//m_mutex.unlock();
	}
}



int main()
{
	//原子操作：std::atomic
	//原子操作概念
	//互斥量：多线程变成中 保护共享数据  关锁  操作共享数据  关锁
	//可以把原子操作理解成一种：不需要用到互斥量加锁（无锁）技术的多线程并发编程技术
	//即 在多线程中 不会像++一样被打断的程序执行片段，原子操作在效率上比互斥量更胜一筹
	//互斥量的加锁一般是针对一个代码段（几行代码），而原子操作针对的一般是一个变量，而不是一个代码段；
	//原子操作要么是完成状态要么是未完成状态不会出现半完成状态
	//std::atomic代表原子操作，它是个类模板，是用来封装某个类型的值的
	//一般atomic原子操作，针对++，--，+=，-=，&=，|=，^=。其他的是不支持的

	thread th1(mythread);
	thread th2(mythread);
	std::chrono::milliseconds dura(3000);
	std::this_thread::sleep_for(dura);
	g_ifend = true;//对原子对象进行写操作
	th1.join();
	th2.join();
	cout << "两个线程执行完毕，最终my_count的结果是：" << my_count << endl;//若是与预期值不同则是因为执行++操作的汇编代码被打断
	cout << "程序运行完毕，退出。。。" << endl;
}