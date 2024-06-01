#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

std::mutex res_mutex;
std::once_flag g_flag;
class MyCla//单例类
{
private:
	static void CreateInstance()
	{
		cout << "CreateInstance被执行" << endl;
		m_instance = new MyCla();
		static TrashRecy cl;//静态对象，生命周期一直大程序退出，调用 这个对象的析构函数！牛逼！
	}


private:
	MyCla() {};//私有化构造函数
	static MyCla* m_instance;//静态成员变量

public:

	static MyCla* GetInstance()
	{
		std::call_once(g_flag,CreateInstance); 
		return  m_instance;
	}
	void func()
	{
		cout << "测试" << endl;
	}

	class TrashRecy//类中套类，用来释放对象
	{
	public:
		~TrashRecy()	//巧用析构释放之前new出来的对象
		{
			if (MyCla::m_instance)
			{
				delete MyCla::m_instance;
				MyCla::m_instance = nullptr;
			}
		}
	};
};
//类静态变量初始化
MyCla* MyCla::m_instance = nullptr;
//线程入口函数
void mythread()
{
	cout << "线程开始执行。。。" << endl;
	MyCla* p_a = MyCla::GetInstance();
	cout << "线程执行完毕。。。" << endl;
}
int main()
{
	thread th1(mythread);
	thread th2(mythread);
	th1.join();
	th2.join();
	//std::call_once() : C++11引入的函数，该函数的第二个参数 是一个函数名a()
	//call_once功能是能够保证函数a()只被调用一次
	//call_once需要与一个标记结合使用，这个标记是 std::once_flag  一种结构体
	//call_once具备互斥量这种能力，并且效率上，比互斥量消耗的资源更少
	//call_once()就是通过这个标记来决定对应的函数a()是否执行，调用call_once成功后call_once就把这个标记设置为一种已调用的状态
	//后续再次调用call_once,只要once_flag被设置为“已调用”状态那么，对应的函数a()就不会被执行了
}