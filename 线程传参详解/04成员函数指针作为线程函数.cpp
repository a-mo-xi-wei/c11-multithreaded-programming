#include<iostream>
#include<thread>
using namespace std;

class A
{
public:
	int m_i;
	A(int i) :m_i(i)
	{
		cout << "调用构造函数...  地址为：" << this << "  线程号为：" << this_thread::get_id() << endl;
	}
	A(const A& a)
	{
		m_i = a.m_i;
		cout << "调用拷贝构造函数...  地址为：" << this << "  线程号为：" << this_thread::get_id() << endl;
	}
	~A()
	{
		cout << "调用析构函数...  地址为：" << this << "  线程号为：" << this_thread::get_id() << endl;
	}
	void thread_work(int num)
	{
		cout << "子线程执行  地址为：" << this << "  线程号为：" << this_thread::get_id() << endl;

	}
	void operator()(int num)
	{
		cout << "operator()子线程执行  地址为：" << this << "  线程号为：" << this_thread::get_id() << endl;
	}
};

int main()
{
	cout << "主线程号为：" << this_thread::get_id() << endl;
	A a(10);
	
	//thread th(&A::thread_work, a, 15);//换个成员函数
	//thread th(&A::operator(),a, 15);//同下，都调用了（）
	thread th( a, 15);
	th.join();
	return 0;
}