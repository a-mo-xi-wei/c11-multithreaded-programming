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
void print2(const A& a)//A &&a
{
	cout << "子线程参数地址为: " << &a << "  线程号为：" << this_thread::get_id() << endl;
}
void print3(unique_ptr<int> p)
{
	 
}
int main()
{
	cout << "主线程号为：" << this_thread::get_id() << endl;
	A a(10);
	thread th(print2, a);//不使用ref的话就会调用拷贝构造
	//thread th(print2, std::ref(a)); //真正的传引用

	//unique_ptr<int>p(new int(100));
	//thread th(print3, std::move(p));
	th.join();
	return 0;
}