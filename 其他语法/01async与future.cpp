#include<iostream>
#include<thread>
#include<mutex>
#include<future>
using namespace std;

class A
{
public:
	int m_myth(int p)
	{
		cout << p << endl;
		cout << "MyThread start" << " threadid= " << this_thread::get_id() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		cout << "MyThread end" << " hreadid= " << this_thread::get_id() << endl;
		return 5;
	}
};


int myth()
{
	cout << "MyThread start" << " threadid= " << this_thread::get_id() << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << "MyThread end" << " hreadid= " << this_thread::get_id() << endl;
	return 5;
}


int main()
{

	//	std::async  std::future 创建后台任务并返回值
	//	希望线程返回一个结果 
	//	std::async是一个函数模板，用来启动一个异步任务，启动之后，返回一个std::future对象（类模板）
	//	“启动一个异步任务”：就是 自动创建 一个线程并开始执行对应的线程入口函数，返回一个std::future对象
	//	这个对象里边就含有线程入口函数所返回的结果（即线程返回结果），我们可以通过调用future对象的成员函数get()来获取结果
	//	std::future提供了一种访问异步操作结果的机制，就是说这个结果可能没有办法马上获取到
	//	但在不就的将来，（线程执行完毕的时候），就能够拿到结果

	


	cout << "main" << "  threadid= " << this_thread::get_id() << endl;
	//future<int>result = async(myth);//创建一个线程并开始执行，绑定关系
	A a;
	int tempar = 12;
	//future<int>result = std::async(&A::m_myth, &a, tempar);//类的成员函数也行
	future<int>result = std::async(std::launch::deferred,&A::m_myth, &a, tempar);
	//	我们通过额外向std::async()传递一个参数，该参数类型是std:launch类型(枚举类型),来达到一些特殊的目的;
	//	std::launch::deferred:表示线程入口函数调用被延迟到std:future的wait()或者get()函数调用时才执行;
	//	那如果wait()或者get)没有被调用，那么线程会执行吗?没执行。实际上，线程根本就没创建。
	//	std::launch::deferred 延迟调用，并且没有创建新线程，是在主线程中调用的线程入口函数
	//	在get(),或wait()处才调用子线程任务,并且主线程还要等待结果返回，就相当于同步了。
	//	std::launch::async ,在调用std::async函数的时候就开始创建线程（相当于默认参数，但不是默认参数，而是重载）



	cout << "continue...." << endl;
	cout << result.get() << endl;//卡在这里，等待myth执行完毕，拿到结果、
	//cout << result.get() << endl;//只能调用一次，不能调用多次
	//result.wait();//等待线程返回，本身并不返回结果
	cout << "End" << endl;

	//若是既没有调用wait()，也没有调用get() ，那么在线程结束时，析构函数自动调用wait()









}