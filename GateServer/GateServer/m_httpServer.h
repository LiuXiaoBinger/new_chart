#pragma once
#include"const.h"

class m_httpServer :public std::
	enable_shared_from_this<m_httpServer>
{
public:
	m_httpServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start();

private:
	tcp::acceptor _acceptor;
	net::io_context& _ioc;
	tcp::socket   _socket;
};

