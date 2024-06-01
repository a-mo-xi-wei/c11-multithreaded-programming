#include<iostream> 
#include<thread>
#include<Windows.h>
using namespace std;
 
void myPrint()
{
	cout << "我的线程开始执行。。。" << endl;
	cout << "我的线程结束执行1。。。" << endl;
	cout << "我的线程结束执行2。。。" << endl;
	cout << "我的线程结束执行3。。。" << endl;
	cout << "我的线程结束执行4。。。" << endl;
	cout << "我的线程结束执行5。。。" << endl;
	cout << "我的线程结束执行6。。。" << endl;
	cout << "我的线程结束执行7。。。" << endl;
	cout << "我的线程结束执行8。。。" << endl;
	cout << "我的线程结束执行9。。。" << endl;
	cout << "我的线程结束执行10。。。" << endl;
}
class TA
{
public:
	int m_i;
	TA(int& i) :m_i(i) { cout << "构造函数被执行。。。" << endl; }//不要使用引用，因为一旦主线程结束局部变量就会被回收
	TA (const TA& it)
	{
		m_i = it.m_i;
		cout << "拷贝构造函数执行。。。" << endl;
	}
	~TA()
	{
		cout << "析构函数执行..." << endl;
	}
	void operator()()
	{
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
		cout << "m_i= "<<m_i << endl;
	}
};
int main()
{
	//程序运行起来生成一个进程，同时该进程的主线程（唯一）开始自动运行
	//cout << "I Love China" << endl;//主线程执行代码，从main函数返回，则整个进程执行完毕

	//主线程从main函数开始执行，那么我们自己的线程，也需要从一个函数开始运行（初始函数）
	//一旦函数运行结束，就代表着线程运行结束
	//整个进程是否执行完毕的标志是：主线程是否执行完毕。
	//如果主线程结束子线程还未结束则系统强行将其终止（传统的），C++11有了例外
	//	detach：分离,也就是主线程不和子线程汇合，即主线程不必等待子线程可以先执行结束
	// 一旦detach后与这个主线程关联的thread对象就会与主线程失去关联，此时这个子线程就会驻留在后台运行
	// 也就相当于被C++运行时库接管，当这个子线程执行完成后，由运行时库负责清理该线程相关资源。（守护线程）
	//1.包含头文件thread//标准库中的类
	//2.初始函数

	//thread myth(myPrint);//存放可调用对象
	//if (myth.joinable())cout << "1.joinable!!!!!" << endl;
	//else cout << "1. not joinable!!!!" << endl;

	//myth.join();//阻塞主线程让子线程执行完毕然后和主线程汇合//类似普通函数
	//myth.join();//使子线程失去控制，一旦使用detach就不能再使用join()否则抛出异常
	//if (myth.joinable())cout << "2.joinable!!!!!" << endl;
	//else cout << "2. not joinable!!!!" << endl;
	//cout << 1 << endl;
	//cout << 1 << endl;
	//cout << 1 << endl;
	//cout << 1 << endl;
	//cout << "主线程安全收尾1" << endl;
	//cout << "主线程安全收尾2" << endl;
	//cout << "主线程安全收尾3" << endl;
	//cout << "主线程安全收尾4" << endl;
	
	
	//int i = 6;//局部变量
	//TA ta(i);//局部对象
	//thread th(ta);//重载()的类对象是可调用对象	//ta被复制到线程中去，主线程结束ta被销毁，但已经被复制进去，因此无影响
	
	auto it = []()
		{
			cout << "我的线程开始执行。。。" << endl;
			cout << "我的线程结束执行1。。。" << endl;
			cout << "我的线程结束执行2。。。" << endl;
			cout << "我的线程结束执行3。。。" << endl;
			cout << "我的线程结束执行4。。。" << endl;
			cout << "我的线程结束执行5。。。" << endl;
			cout << "我的线程结束执行6。。。" << endl;
			cout << "我的线程结束执行7。。。" << endl;
			cout << "我的线程结束执行8。。。" << endl;
			cout << "我的线程结束执行9。。。" << endl;
			cout << "我的线程结束执行10。。。" << endl;
		};
	thread th(it);
	//th.join();
	th.detach();
	if (th.joinable())cout << "joinable!!!!" << endl;
	else cout << "not joinable!!!!" << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << 20 << endl;
	cout << "主线程安全收尾1" << endl;
	cout << "主线程安全收尾2" << endl;
	cout << "主线程安全收尾3" << endl;
	cout << "主线程安全收尾4" << endl;
	return 0;
}