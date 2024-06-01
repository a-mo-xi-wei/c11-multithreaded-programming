#include<iostream>
#include<thread>
#include<mutex>
#include<future>
#include<vector>
using namespace std;
int myth1(int tem)
{
	cout << "myth1() start threadid= " << this_thread::get_id() << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	return 3;
}
//void myth2(std::future<int>& temp)
void myth2(std::shared_future<int>& temp)
{
	cout << "myth2() start threadid= " << this_thread::get_id() << endl;
	auto result = temp.get();//只能有一次
	//只能调用一次get的原因是：future<int> get()函数的设计，是一个移动语义
	cout << "myth2 result= " << result << endl;
}

int main()
{
	//shared_future 是类模板，get()函数复制数据而不是转移数据，可以多次get()


	cout << "main threadid=" << std::this_thread::get_id() << endl;
	std::packaged_task<int(int)>mypt(myth1);
	std::thread th1(std::ref(mypt),1);
	th1.join();
	//std::future<int>result = mypt.get_future();
	std::shared_future<int>result_s(mypt.get_future());//试了一下没用std::move() 和 ,share()都行

	//std::shared_future<int>result_s(std::move(result));//必须传右值
	//std::shared_future<int>result_s(result.share());//两种方法都行
	//cout <<  result_s.get() << endl;

	//std::thread th2(myth2,std::ref(result));
	std::thread th2(myth2,std::ref(result_s));
	th2.join();
	cout << "End" << endl;
}