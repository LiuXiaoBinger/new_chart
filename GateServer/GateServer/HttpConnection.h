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
    //c++��http����tcpʵ�֣�boost��Ҳ��һ�������ǿ���ʵ��һ����ʱ��⺯�� ����������������ʱ����ȥ���������������õ�ʱ�䣬������ֵ�Է�����
    void CheckDeadline();

    void WriteResponse();
    void HandleReq();
    tcp::socket  _socket;
    // The buffer for performing reads.
    beast::flat_buffer  _buffer{ 8192 }; //������������

    // The request message.
    http::request<http::dynamic_body> my_request;//������������

    // The response message.
    http::response<http::dynamic_body> _response;// ������Ӧ�ͻ���

    // The timer for putting a deadline on connection processing.
    //boost�Ķ�ʱ�����õײ��¼�ѭ����������Ҫһ���¼�������
    net::steady_timer deadline_{
        _socket.get_executor(), std::chrono::seconds(60) };
private:
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;

	/*std::string _Host_url;
	std::unordered_map<std::string, std::string> _get_params;*/
};
