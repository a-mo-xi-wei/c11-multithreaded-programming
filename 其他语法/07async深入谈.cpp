#include<iostream>
#include<thread>
#include<mutex>
#include<future>
#include<vector>

using namespace std;

int mythread()
{
	cout << "thread id= " << std::this_thread::get_id() << " 运行中。。。" << endl;
	std::this_thread::sleep_for(5s);
	return 1;
}

int main()
{
	//std::async参数详述，async用来创建一个异步任务
	cout << "thread id= " << std::this_thread::get_id() << " 运行中。。。" << endl;
	std::future<int>result = std::async(mythread);
	
	//std::launch::deferred【延迟调用】  std::launch::async【强制创建一个线程】
	//std::thread()创建线程可能会因为系统资源紧张导致创建失败而系统崩溃
	//std::async()一般不叫创建一个线程，而是叫做创建一个异步任务
	//deferred延迟调用并且不创建新线程，知道get(),wait()

	//std::luanch::async：强制这个异步任务在新线程上执行，即强制创建新线程
	//若不提供第一个参数，则默认是  async|deferred  //由系统自行决定异步还是同步
	//总而言之相较于thread()async()有保障一点，不容易因为系统资源紧张导致线程创建失败

	//std::async不确定性问题的解决
	//不加额外参数的std::async调用，让系统决定是否创建新线程
	//问题的焦点在于这个异步任务到底有没有被推迟执行，看以下代码：

	std::future_status sta = result.wait_for(0s);          //(std::chrono::milliseconds(0));
	if (sta == std::future_status::deferred)
	{
		//线程被延迟执行，系统资源紧张采用了 std::launch::deferred 策略
		cout << result.get() << endl;
	}
	else
	{
		//任务没有被推迟，已经开始运行了，线程已经被创建
		if (sta == std::future_status::ready)
		{
			//线程成功返回
			cout << "线程成功执行完毕并返回" << endl;
			cout << result.get() << endl;
		}
		else if (sta == std::future_status::timeout)
		{
			cout << "线程成功创建但执行超时。。。" << endl;
			cout << result.get() << endl;
		}
	}

}