#include "m_httpServer.h"
#include "HttpConnection.h"
m_httpServer::m_httpServer(boost::asio::io_context& ioc, unsigned short& port)
	:_ioc(ioc), _acceptor(ioc,tcp::endpoint(tcp::v4(),port)), _socket(ioc)
{

}

void m_httpServer::Start()
{
    auto self = shared_from_this();
    //std::make_shared<M_HttpConnection>(std::move(self->_socket))->Start();
    _acceptor.async_accept(_socket, [self](beast::error_code ec) {
        try {
            //���������������ӣ���������������
            if (ec) {
                self->Start();
                return;
            }

            //���������ӣ�����HpptConnection�����������
            std::make_shared<M_HttpConnection>(std::move(self->_socket))->Start();
            //��������
            self->Start();
        }
        catch (std::exception& exp) {
            std::cout << "exception is " << exp.what() << std::endl;
            self->Start();
        }
        });
}
