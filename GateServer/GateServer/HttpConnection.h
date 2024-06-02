#pragma once
#include "const.h"
#include <unordered_map>
class M_HttpConnection : public std::
    enable_shared_from_this<M_HttpConnection>
{
    friend class CLogicSystem;
public:
    M_HttpConnection(tcp::socket socket);
    void Start();

    void PreParseGetParam();

private:
    //c++的http基于tcp实现，boost库也是一样，我们可以实现一个超时检测函数 发送完数据启动定时器，去监听发送数据所用的时间，超过阈值对方掉线
    void CheckDeadline();

    void WriteResponse();
    void HandleReq();
    tcp::socket  _socket;
    // The buffer for performing reads.
    beast::flat_buffer  _buffer{ 8192 }; //用来接受数据

    // The request message.
    http::request<http::dynamic_body> my_request;//用来解析请求

    // The response message.
    http::response<http::dynamic_body> _response;// 用来回应客户端

    // The timer for putting a deadline on connection processing.
    //boost的定时器采用底层事件循环，所以需要一个事件调度器
    net::steady_timer deadline_{
        _socket.get_executor(), std::chrono::seconds(60) };
private:
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;

	/*std::string _Host_url;
	std::unordered_map<std::string, std::string> _get_params;*/
};
