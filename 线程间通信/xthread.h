﻿#pragma once
#include<thread>
class XThread
{
public:
	//线程启动
	virtual void Start();
	//设置线程退出标志 并等待
	virtual void Stop();
	//等待线程退出（阻塞）
	virtual void Wait();
	//线程是否退出
	bool is_exit();

private:
	//线程入口
	virtual void Main() = 0;
	bool is_exit_ = false;
	std::thread th_;
};

