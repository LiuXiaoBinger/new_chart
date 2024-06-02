#pragma once
#include"const.h"
#include"singleton.h"
#include<map>
class M_HttpConnection;
typedef std::function<void(std::shared_ptr< M_HttpConnection>)> handleHttp;
class CLogicSystem :public Singleton<CLogicSystem>
{
	friend class Singleton<CLogicSystem>;
public:
	
	~CLogicSystem();
	bool HandleGet(std::string, std::shared_ptr<M_HttpConnection>);
	bool HandlePost(std::string path, std::shared_ptr<M_HttpConnection> con);
	void RegGet(std::string, handleHttp handler);
	void RegPost(std::string url, handleHttp handler);
private:
	CLogicSystem();
	std::map<std::string, handleHttp> _post_handlers;
	std::map<std::string, handleHttp> _get_handlers;
};

