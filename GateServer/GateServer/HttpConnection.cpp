#include "HttpConnection.h"
#include"CLogicSystem.h"
M_HttpConnection::M_HttpConnection(tcp::socket socket)
    : _socket(std::move(socket)) {
}
//
//``
//启动服务器，在浏览器输入`http://localhost:8080/get_test`
//
//会看到服务器回包`receive get_test req`
//
//如果我们输入带参数的url请求`http://localhost:8080/get_test?key1=value1&key2=value2`
//
//会收到服务器反馈`url not found`
//
//所以对于get请求带参数的情况我们要实现参数解析，我们可以自己实现简单的url解析函数


//char 转为16进制
unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}
unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}
std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//判断是否仅有数字和字母构成
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ') //为空字符
			strTemp += "+";
		else
		{
			//其他字符需要提前加%并且高四位和低四位分别转为16进制
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] & 0x0F);
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//还原+为空
		if (str[i] == '+') strTemp += ' ';
		//遇到%将后面的两个字符从16进制转为char再拼接
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}
void M_HttpConnection::Start()
{
    auto self = shared_from_this();

    http::async_read(_socket, _buffer, my_request, [self](beast::error_code ec, std::size_t bytes_transferred) {
        try {
            if (ec) {
                std::cout << "http read err is " << ec.what() << std::endl;
                return;
            }

            //处理读到的数据
            boost::ignore_unused(bytes_transferred);
            self->HandleReq();
            self->CheckDeadline();
        }
        catch (std::exception& exp) {
            std::cout << "exception is " << exp.what() << std::endl;
        }
        });
    
}
void M_HttpConnection::PreParseGetParam() {
	// 提取 URI  
	auto uri = my_request.target();
	// 查找查询字符串的开始位置（即 '?' 的位置）  
	auto query_pos = uri.find('?');
	if (query_pos == std::string::npos) {
		_get_url = uri;
		return;
	}

	_get_url = uri.substr(0, query_pos);
	std::string query_string = uri.substr(query_pos + 1);
	std::string key;
	std::string value;
	size_t pos = 0;
	while ((pos = query_string.find('&')) != std::string::npos) {
		auto pair = query_string.substr(0, pos);
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(pair.substr(0, eq_pos)); // 假设有 url_decode 函数来处理URL解码  
			value = UrlDecode(pair.substr(eq_pos + 1));
			_get_params[key] = value;
		}
		query_string.erase(0, pos + 1);
	}
	// 处理最后一个参数对（如果没有 & 分隔符）  
	if (!query_string.empty()) {
		size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos) {
			key = UrlDecode(query_string.substr(0, eq_pos));
			value = UrlDecode(query_string.substr(eq_pos + 1));
			_get_params[key] = value;
		}
	}
}
void M_HttpConnection::CheckDeadline()
{
    auto ptr = shared_from_this();

    deadline_.async_wait(
        [ptr](beast::error_code ec)
        {
            if (!ec)
            {
                // Close socket to cancel any outstanding operation.
                ptr->_socket.close(ec);//关闭客户端
            }
        });
    
}

void M_HttpConnection::WriteResponse() {
    auto ptr = shared_from_this();
    _response.content_length(_response.body().size());
    http::async_write(
        _socket,
        _response,
        [ptr](beast::error_code ec, std::size_t)
        {
            ptr->_socket.shutdown(tcp::socket::shutdown_send, ec);
            ptr->deadline_.cancel();
        });
}

void M_HttpConnection::HandleReq() {
    //设置版本
    _response.version(my_request.version());
    //设置为短链接
    _response.keep_alive(false);
    //处理get请求
    if (my_request.method() == http::verb::get) {
		PreParseGetParam();
        bool success = CLogicSystem::Get_M_ptr()->HandleGet(_get_url/*路由*/, shared_from_this());
		std::cout << my_request.target() << std::endl;
        if (!success) {
            _response.result(http::status::not_found);
            _response.set(http::field::content_type, "text/plain");
            beast::ostream(_response.body()) << "url not found\r\n";
            WriteResponse();
            return;
        }

        _response.result(http::status::ok);
        _response.set(http::field::server, "GateServer");
        WriteResponse();
        return;
    }
	if (my_request.method() == http::verb::post) {
		bool success = CLogicSystem::Get_M_ptr()->HandlePost(my_request.target(), shared_from_this());
		if (!success) {
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}

		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}
}
