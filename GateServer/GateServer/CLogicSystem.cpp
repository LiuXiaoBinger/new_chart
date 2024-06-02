#include "CLogicSystem.h"
#include"HttpConnection.h"

CLogicSystem::CLogicSystem(){
	RegGet("/get_test", [](std::shared_ptr<M_HttpConnection> connection) {
		beast::ostream(connection->_response.body()) << "receive get_test req";
		int i = 0;
		for (auto& elem : connection->_get_params) {
			i++;
			beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;
			beast::ostream(connection->_response.body()) << ", " << " value is " << elem.second << std::endl;
		}
		});

	RegPost("/get_varifycode", [](std::shared_ptr<M_HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->my_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		auto email = src_root["email"].asString();
		std::cout << "email is " << email << std::endl;
		root["error"] = 0;
		root["email"] = src_root["email"];
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;
		});
}

CLogicSystem::~CLogicSystem()
{
	std::cout << "~CLogicSystem()" << std::endl;
}

bool CLogicSystem::HandleGet(std::string path, std::shared_ptr<M_HttpConnection>con)
{
	if(_get_handlers.find(path)==_get_handlers.end())
	return false;
	_get_handlers[path](con);
	return true;
}
bool CLogicSystem::HandlePost(std::string path, std::shared_ptr<M_HttpConnection> con) {
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}

	_post_handlers[path](con);
	return true;
}
void CLogicSystem::RegGet(std::string str, handleHttp handler)
{
	_get_handlers.insert(make_pair(str,handler));
}
void CLogicSystem::RegPost(std::string url, handleHttp handler) {
	_post_handlers.insert(make_pair(url, handler));
}