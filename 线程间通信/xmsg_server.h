#pragma once
#include "xthread.h"
#include<string>
#include<list>
#include<mutex>
#include<iostream>
class XMsgServer : public XThread
{
public:
	//给当前线程发送消息
	void SendMsg(std::string msg);

private:

	//处理消息的线程入口函数
	void Main()override;
	//消息队列访问
	std::list<std::string>msgs_;

	//互斥访问消息队列
	std::mutex mux_;
};

