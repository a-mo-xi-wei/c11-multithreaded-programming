#include<iostream>
#include<thread>
using namespace std;


class A
{
public:
	int m_a;
	A(int a) :m_a(a) { cout << "调用构造函数。。" << this << endl; }
	A(const A &aa)
	{
		m_a = aa.m_a;
		cout << "调用拷贝构造函数。。" << this << endl;
	}
	~A()
	{
		cout << "调用析构函数。。。" << this << endl;
	}
};

//void myprint(const int &id, char* buf)//实测&id其实是复制了一份，并没有绑定到实参的地址上 
void myprint(const int id,const A &buf)//若buf传的不是引用则又会再拷贝构造一份
{
	cout << id << endl;//实际上是值传递，不是真引用是假引用，因此即时主线程结束，这里仍然是安全的，所以这里不推荐使用引用）
	cout << &buf << endl;//指针在detach子线程时，绝对会有问题！！！！，所以一定不能传指针
}


int main()
{
	//1. 传递临时对象作为线程参数
	int aa = 1;
	int& bb = aa;
	//char ch[] = { "this is a test!!!" };

	//thread th(myprint, bb, ch);//即使如此还是存在bug!!!
	//事实上，还存在一种情况：main()函数已经执行完了(ch已经被回收了)
	// 系统才开始将ch隐式转换为string!!!可此时已经不存在ch了！！！
	//因此最佳的写法是
	int i = 12;
	thread th(myprint, bb, A(i));//此处直接将mybuf转换成string对象，这是一个可以保证在线程中肯定有效的对象
	th.join();
	//th.detach();
	cout << "Hello" << endl;
	
}