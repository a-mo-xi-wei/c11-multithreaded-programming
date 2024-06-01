#include<iostream>
#include<thread>
#include<mutex>
#include<future>
using namespace std;

int mythread()
{
	cout << "MyThread start" << " threadid= " << this_thread::get_id() << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << "MyThread end" << " hreadid= " << this_thread::get_id() << endl;
	return 5;
}

int main()
{
	cout << "main threadid= " << this_thread::get_id() << endl;
	std::future<int>result = std::async(mythread);
	//std::future<int>result = std::async(std::launch::deferred, mythread);
	cout << "Continue...." << endl;
	//cout << result.get() << endl;

	std::future_status statu = result.wait_for(std::chrono::seconds(4));
	if (statu == std::future_status::timeout)
	{
		//超时，表示线程还没有执行完
		cout << "超时了，线程未执行完毕。" << endl;
	}
	else if (statu == std::future_status::ready)
	{
		//表示线程成功返回
		cout << "线程成功执行完毕，并返回" << endl;
		cout << result.get() << endl;
	}
	else if (statu == std::future_status::deferred)
	{
		//如果async的第一个参数被设置为std::launch::deferred,则本条件成立
		cout << "线程被延迟执行。" << endl;
		cout << result.get() << endl;
	}

	cout << "End" << endl;
}