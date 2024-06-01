#include<iostream>
#include<thread>
using namespace std;

class MyCla//单例类
{

private:
	MyCla() {};//私有化构造函数
	static MyCla* m_instance;//静态成员变量

public:
	static MyCla* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new MyCla();
			static TrashRecy cl;//静态对象，生命周期一直大程序退出，调用 这个对象的析构函数！牛逼！
		}
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




int main()
{
	//设计模式概谈
	//“设计模式”：是一种代码的写法（跟一般的代码写法不同）：程序灵活，维护起来可能方便，但若要其他人接管，阅读代码会很痛苦
	//用“设计模式”理念写出来的代码是很晦涩的。
	//外国人在应付特别大的项目的时候，把项目的开发经验、模块划分经验，总结整理成设计模式（先由开发需求，后有理论总结和整理）
	//设计模块式有它独特的优点，要活学活用，不要深陷其中


	//	单例设计模式
	//	单例设计模式的使用频率比较高
	//	单例：在整个项目中，有某个或某些特殊的类，属于该类的对象，我只能创建一个，多了创建不了
	//	单例类；

	MyCla* p_a = MyCla::GetInstance();//创建一个对象，返回该类对象的指针
	MyCla* p_b = MyCla::GetInstance();//p_b==p_a;  啧啧啧牛
	//看到这 我也只能感叹   牛逼！！！！
	p_a->func();
	p_b->func();

}