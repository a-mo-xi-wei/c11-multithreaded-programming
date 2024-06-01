#include<thread>
#include<iostream>
#include<vector>
using namespace std;

//线程入口函数
void myprint(int num)
{
	cout << "myprint线程开始执行。。。线程号：" <<num<< endl;

	cout<< "myprint线程结束执行。。。线程号：" <<num<< endl;
}

int main()
{
	//创建和等待多个线程
	vector<thread>threads;
	//创建10个线程
	//线程入口函数统一使用 myprint.
	//1.多个线程执行顺序是乱的，这与操作系统内部对线程的运行调度机制有关
	//主线程等待所有子线程结束
	//将thread对象放入到容器中，对于一次创建大量线程并对大量线程进行管理非常方便
	for (int i = 1; i <= 10; i++)
	{
		threads.push_back(thread(myprint, i));//创建的同时，就已经开始执行
	}
	for (auto &val : threads)
	{
		val.join();//等待10个线程都返回
	}
	cout << "主线程结束。。。" << endl;
}