#include<iostream>
#include<thread>
#include<vector>
#include<list>
using namespace std;
vector<int>v = { 1,2,3 };//共享数据	只读

//void myprint(int num)
//{
//	cout << "id为：" << std::this_thread::get_id() << "的线程打印v的值";
//	for (auto val : v)cout << val;
//	cout << endl;
//}

class A
{
public:
	//把收到的消息（命令）放入一个队列线程
	void inMagRecvQueue()
	{
		for (int i = 0; i < 10000; ++i)
		{
			cout << "inMagRecvQueue（）执行，插入一个元素" << i<<endl;
			msgRecvQueue.push_back(i);//假设这个数字就是收到的命令
		}
	}
	//把数据从消息队列中取出的线程
	void outMagRecvQueue()
	{
		for (int i = 0; i < 10000; ++i)
		{
			if (!msgRecvQueue.empty())
			{
				//消息不为空
				int command = msgRecvQueue.front(); //返回第一个元素，但不进行检查，因此要判断是否为空
				msgRecvQueue.pop_front();//移除，不返回
			}
			else
			{
				cout << "outMagRecvQueue()执行，但目前消息队列为空。。。" << i<<endl;
			}
		}
		cout << "end" << endl;
	}
private:
	list<int>msgRecvQueue;//专门用于代表玩家发送过来的命令
};


int main()
{
	//vector<thread>ths;
	//for (int i = 1; i <= 10; i++)ths.push_back(thread(myprint, i));
	//for (auto& val : ths)
	//{
	//	val.join();
	//}
	//cout << "主线程结束。。。" << endl;

	//1. 只读的数据是安全稳定的不需要什么特别的处理手段
	//2. 有读有写	2个线程写8个线程读	如果代码没有特别的处理程序肯定崩溃
	//	 最简单的不崩溃处理：只允许一个线程作业


	//共享数据的保护案例代码
	//网络游戏服务器。自己创建两个线程，一个线程收集玩家命令（数字），并把命令数据写到一个队列中
	// 另一个线程从队列中取出玩家发送的命令，解析，然后执行玩家需要的动作  

	//采用list双向链表	理由是：频繁的随机插入和删除数据效率高

	//使用成员函数作为线程入口函数
	A a;
	std::thread thout(&A::outMagRecvQueue, &a);//第二个参数是对象的地址，因为成员函数的运行需要this指针
	std::thread thin(&A::inMagRecvQueue, &a);
	thin.join();
	thout.join();
	//以上写法会导致崩溃
	//引入一个C++解决多线程保护共享数据问题的第一个概念	“互斥量”

	return 0;
}