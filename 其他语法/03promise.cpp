#include<iostream>
#include<thread>
#include<mutex>
#include<future>
using namespace std;

void mythread(std::promise<int>&temp,int cal)
{
	//做一系列复杂的运算
	cal++;
	cal *= 10;
	//.....
	//做其他运算，假设整整花了3秒
	std::chrono::milliseconds dura(3000);
	std::this_thread::sleep_for(dura);
	//终于计算出结果
	int result = cal;//保存结果
	temp.set_value(result);//结果保存到temp这个对象中去
}
void mythread2(std::future<int>& tem)
{
	auto result = tem.get();
	cout << "成功返回result= " << result << endl;
	cout<<"End"<<endl;
}

int main()
{
	//std::promise,类模板
	//我们能够在某个线程中给它赋值，然后可以再其他线程使用
	//总结：通过promise保存一个值，在将来某个时刻我们通过把一个future绑定到这个promise上来得到这个绑定的值
	std::promise<int>pro;//声明一个保存的值类型为int的promise对象
	std::thread th1(mythread, std::ref( pro), 10);
	th1.join();
	//
	std::future<int>result = pro.get_future();//pro与future绑定，用于获取线程返回值
	//cout << "result= " << result.get() << endl;
	//cout << "End" << endl;
	std::thread th2(mythread2, std::ref(result));
	th2.join();



	return 0;
}