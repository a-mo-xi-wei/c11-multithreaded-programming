#include<iostream>
#include<thread>
#include<mutex>
#include<future>
#include<vector>
using namespace std;

int myth(int i)
{
	cout << i << endl;
	cout << "MyThread start" << " threadid= " << this_thread::get_id() << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << "MyThread end" << " threadid= " << this_thread::get_id() << endl;
	return 5;
}

vector<std::packaged_task<int(int)> >mytasks;

int main()
{

	
	cout << "main" << "  threadid= " << this_thread::get_id() << endl;
	

	//std::packaged_task ： 打包任务 把任务包装起来
	// 它是类模板，模板参数是各种可调用对象，通过packaged_task来把各种可调用对象包装起来，方便以后作为线程入口函数来调用



	//std::packaged_task<int(int)>mypt(myth);//把函数myth通过packaged_task包装起	（可用于包装任何可调用对象）
	//std::thread th(std::ref(mypt), 1);//线程直接开始执行，第二个参数，作为线程入口函数的参数
	//th.join();
	//std::future<int>result = mypt.get_future();
	//cout << result.get() << endl;
	//cout << "End" << endl;


	std::packaged_task<int(int)>mypt([](int mypar)
		{
			cout << mypar << endl;
			cout << "MyThread start" << " threadid= " << this_thread::get_id() << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			cout << "MyThread end" << " threadid= " << this_thread::get_id() << endl;
			return 5;
		});


	/*std::thread th(std::ref(mypt), 1);
	th.join();
	std::future<int>result = mypt.get_future();
	cout << result.get() << endl;
	cout << "End" << endl;*/

	
	//mypt(100);//直接调用相当于函数调用
	//std::future<int>result = mypt.get_future();
	//cout << result.get() << endl;

	mytasks.push_back(std::move(mypt));//使用移动语义，放入之后mypt就为空
	std::packaged_task<int(int)>mypt2;
	auto it = mytasks.begin();
	mypt2 = std::move(*it);
	mytasks.erase(it);
	std::future<int>result = mypt2.get_future();
	cout << result.get() << endl;

	return 0;
}