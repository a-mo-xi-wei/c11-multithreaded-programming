#include "xmsg_server.h"

void XMsgServer::SendMsg(std::string msg)
{
	std::unique_lock<std::mutex>lock(mux_);
	msgs_.push_back(msg);

}

void XMsgServer::Main()
{
	while (!is_exit())
	{
		std::unique_lock<std::mutex>lock(mux_, std::try_to_lock);
		if (lock.owns_lock())
		{
			if (msgs_.empty())continue;
			else
			{
				while (!msgs_.empty())
				{
					//消息处理业务逻辑
					std::cout << "receive: " << msgs_.front() << std::endl;
					msgs_.pop_front();
				}
			}
		}
	}
}
