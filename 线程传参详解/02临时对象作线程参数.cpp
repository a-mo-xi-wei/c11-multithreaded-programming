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
};
void print2( A &a)//A &&a
{
	cout << "子线程参数地址为: " << &a << "  线程号为：" << this_thread::get_id() << endl;
}
int main()
{
	cout << "主线程号为：" << this_thread::get_id() << endl;
	int i = 1;
	thread th(print2, A(i));//临时对象
	th.join();
	return 0;
}