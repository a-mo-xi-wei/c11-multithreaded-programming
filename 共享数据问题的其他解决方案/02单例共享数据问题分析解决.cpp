#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

std::mutex res_mutex;
class MyCla//单例类
{
private:
	MyCla() {};//私有化构造函数
	static MyCla* m_instance;//静态成员变量

public:

	static MyCla* GetInstance()
	{
		if (m_instance == nullptr)
		{
			unique_lock<mutex>mymutex(res_mutex);
			if (m_instance == nullptr)
			{
				m_instance = new MyCla();
				static TrashRecy cl;
			}
		}
		return  m_instance;
	}
	void func()
	{
		cout << "测试" << endl;
	}

	class TrashRecy//类中套类，用来释放对象
	{
	public:
		~TrashRecy()	//巧用析构释放之前new出来的对象
		{
			if (MyCla::m_instance)
			{
				delete MyCla::m_instance;
				MyCla::m_instance = nullptr;
			}
		}
	};



};
//类静态变量初始化
MyCla* MyCla::m_instance = nullptr;

//线程入口函数
void mythread()
{
	cout << "线程开始执行。。。" << endl;
	MyCla* p_a = MyCla::GetInstance();
	cout << "线程执行完毕。。。" << endl;
}

int main()
{
	//MyCla* p_a = MyCla::GetInstance();
	//p_a->func();
	//MyCla::GetInstance()->func();//该装载的数据装载

	//单例设计模式共享数据问题分析、解决
	//面临的问题：需要我们自己创建的线程（而不是主线程）中来创建单例类的对象。（这种线程可能不止一个）
	//我们可能要面临GetInstance()这种成员函数要互斥

	//虽然这两个线程是同一个入口函数，一定要记住这是两个线程
	thread th1(mythread);
	thread th2(mythread);
	th1.join();
	th2.join();
}