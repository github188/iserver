#include "StdAfx.h"
#include "ApiProc.h"
#include "UserMgr.h"

namespace _opera_type {

    const std::string opera_login = "1";
    const std::string opera_logout = "2";
    const std::string opera_heartb = "3";
}

namespace _stock_replies {

    const char ok[] = "";
    const char created[] =
        "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created</h1></body>"
        "</html>";
    const char accepted[] =
        "<html>"
        "<head><title>Accepted</title></head>"
        "<body><h1>202 Accepted</h1></body>"
        "</html>";
    const char no_content[] =
        "<html>"
        "<head><title>No Content</title></head>"
        "<body><h1>204 Content</h1></body>"
        "</html>";
    const char multiple_choices[] =
        "<html>"
        "<head><title>Multiple Choices</title></head>"
        "<body><h1>300 Multiple Choices</h1></body>"
        "</html>";
    const char moved_permanently[] =
        "<html>"
        "<head><title>Moved Permanently</title></head>"
        "<body><h1>301 Moved Permanently</h1></body>"
        "</html>";
    const char moved_temporarily[] =
        "<html>"
        "<head><title>Moved Temporarily</title></head>"
        "<body><h1>302 Moved Temporarily</h1></body>"
        "</html>";
    const char not_modified[] =
        "<html>"
        "<head><title>Not Modified</title></head>"
        "<body><h1>304 Not Modified</h1></body>"
        "</html>";
    const char bad_request[] =
        "<html>"
        "<head><title>Bad Request</title></head>"
        "<body><h1>400 Bad Request</h1></body>"
        "</html>";
    const char unauthorized[] =
        "<html>"
        "<head><title>Unauthorized</title></head>"
        "<body><h1>401 Unauthorized</h1></body>"
        "</html>";
    const char forbidden[] =
        "<html>"
        "<head><title>Forbidden</title></head>"
        "<body><h1>403 Forbidden</h1></body>"
        "</html>";
    const char not_found[] =
        "<html>"
        "<head><title>Not Found</title></head>"
        "<body><h1>404 Not Found</h1></body>"
        "</html>";
    const char internal_server_error[] =
        "<html>"
        "<head><title>Internal Server Error</title></head>"
        "<body><h1>500 Internal Server Error</h1></body>"
        "</html>";
    const char not_implemented[] =
        "<html>"
        "<head><title>Not Implemented</title></head>"
        "<body><h1>501 Not Implemented</h1></body>"
        "</html>";
    const char bad_gateway[] =
        "<html>"
        "<head><title>Bad Gateway</title></head>"
        "<body><h1>502 Bad Gateway</h1></body>"
        "</html>";
    const char service_unavailable[] =
        "<html>"
        "<head><title>Service Unavailable</title></head>"
        "<body><h1>503 Service Unavailable</h1></body>"
        "</html>";


    std::string to_string(const http_status::http_status_type& _status)
    {
        switch (_status)
        {
        case http_status::ok:
            return ok;
        case http_status::created:
            return created;
        case http_status::accepted:
            return accepted;
        case http_status::no_content:
            return no_content;
        case http_status::multiple_choices:
            return multiple_choices;
        case http_status::moved_permanently:
            return moved_permanently;
        case http_status::moved_temporarily:
            return moved_temporarily;
        case http_status::not_modified:
            return not_modified;
        case http_status::bad_request:
            return bad_request;
        case http_status::unauthorized:
            return unauthorized;
        case http_status::forbidden:
            return forbidden;
        case http_status::not_found:
            return not_found;
        case http_status::internal_server_error:
            return internal_server_error;
        case http_status::not_implemented:
            return not_implemented;
        case http_status::bad_gateway:
            return bad_gateway;
        case http_status::service_unavailable:
            return service_unavailable;
        default:
            return internal_server_error;
        }
    }

    _reply stock_reply(const http_status::http_status_type& status, const std::string& content_type = "text/html");
    _reply stock_reply(const http_status::http_status_type& status, const std::string& content_type)
    {
        _reply rep;
        rep.status = status;
        rep.content = to_string(status);
        rep.headers.resize(2);
        rep.headers[0].name = "Content-Length";
        rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
        rep.headers[1].name = "Content-Type";
        rep.headers[1].value = content_type;
        return rep;
    }
}


// 处理用户状态改变
void user_state(const _request& req, _reply& rep);

// 获取区域列表
void region_oper(const _request& req, _reply& rep);

// 获取区域信息
void region_oper2(const _request& req, _reply& rep);

// 获取用户列表
void user_oper(const _request& req, _reply& rep);

// 获取用户信息
void user_oper2(const _request& req, _reply& rep);

// 转发消息
void trans_msg(const _request& req, _reply& rep);

CApiProc::CApiProc()
{
    // 注册处理函数
    rest_fun_regist(std::string("^/api/state/\\d{1,3}$"), user_state);
    rest_fun_regist(std::string("^/api/region$"), region_oper);
    rest_fun_regist(std::string("^/api/region/\\d{1,3}$"), region_oper2);
    rest_fun_regist(std::string("^/api/region/\\d{1,3}/user$"), user_oper);
    rest_fun_regist(std::string("^/api/user/\\d{1,3}$"), user_oper2);
    rest_fun_regist(std::string("^/api/transaction$"), trans_msg);
}

int CApiProc::ApiProc(const _request& req_, _reply& rep_)
{
    int ret = rest_fun_set(req_, rep_);
    if (-1 == ret)
    {
        rep_ = _stock_replies::stock_reply(http_status::bad_request);
    }
    return ret;
}


void user_state(const _request& req, _reply& rep)
{
    printf("user_state:%s\n%s\n", req.uri.c_str(), req.extern_string.c_str());
    // /api/state/1
    if (req.method != "PUT" && req.method != "POST")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    std::string accept_type = "application/xml";
    for (size_t t = 0; t < req.headers.size(); ++t)
    {
        std::string name = req.headers[t].name;
        std::transform(name.begin(), name.end(), name.begin(), tolower);
        if (name == "accept")
        {
            accept_type = req.headers[t].value;
        }
    }

    std::string uri = req.uri;
    unsigned pos = uri.find_last_of('/', std::string::npos);
    if (std::string::npos == pos)
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }
    std::string opera = uri.substr(pos+1);

    std::string content;
    if (_opera_type::opera_login == opera)  // 用户登录
    {
        if (false == GetUserMgr().UserLogin_Xml(req.extern_string, content))
        {
            rep = _stock_replies::stock_reply(http_status::bad_request);
            return ;
        }
    }
    else if (_opera_type::opera_logout == opera)    // 用户登出
    {
        if (false == GetUserMgr().UserLogout_Xml(req.extern_string, content))
        {
            rep = _stock_replies::stock_reply(http_status::bad_request);
            return ;
        }
    }
    else if (_opera_type::opera_heartb == opera)    // 在线用户心跳
    {
        if (false == GetUserMgr().UserHeartB_Xml(req.extern_string, content))
        {
            rep = _stock_replies::stock_reply(http_status::bad_request);
            return ;
        }
    }
    else
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    rep.status = http_status::ok;
    rep.content = fcA2U(content.c_str());
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = accept_type;
}

void region_oper(const _request& req, _reply& rep)
{
    printf("get_region:%s\n", req.uri.c_str());
    if (req.method != "GET" && req.method != "POST")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    std::string accept_type = "application/xml";
    for (size_t t = 0; t < req.headers.size(); ++t)
    {
        std::string name = req.headers[t].name;
        std::transform(name.begin(), name.end(), name.begin(), tolower);
        if (name == "accept")
        {
            accept_type = req.headers[t].value;
        }
    }

    std::string content;
    if (req.method == "GET")    // 获取区域列表
    {
        if (false == GetUserMgr().GetRegion_Xml(content))
        {
            rep = _stock_replies::stock_reply(http_status::internal_server_error);
            return ;
        }
    }
    else if (req.method == "POST") // 创建新的区域
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    rep.status = http_status::ok;
    rep.content = fcA2U(content.c_str());
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = accept_type;
}

void region_oper2(const _request& req, _reply& rep)
{
    // /api/region/2
    printf("get_region_info:%s\n", req.uri.c_str());
    if (req.method != "GET" && req.method != "PUT" && req.method != "DELETE")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    std::string accept_type = "application/xml";
    for (size_t t = 0; t < req.headers.size(); ++t)
    {
        std::string name = req.headers[t].name;
        std::transform(name.begin(), name.end(), name.begin(), tolower);
        if (name == "accept")
        {
            accept_type = req.headers[t].value;
        }
    }

    std::string uri = req.uri;
    unsigned pos = uri.find_last_of('/', std::string::npos);
    if (std::string::npos == pos)
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }
    std::string str_id = uri.substr(pos+1);

    std::string content;
    if (req.method == "GET")
    {
        if (false == GetUserMgr().GetRegionInfo_Xml(str_id, content))
        {
            rep = _stock_replies::stock_reply(http_status::internal_server_error);
            return ;
        }
    }
    else if (req.method == "PUT")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }
    else if (req.method == "DELETE")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    rep.status = http_status::ok;
    rep.content = fcA2U(content.c_str());
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = accept_type;
}

void user_oper(const _request& req, _reply& rep)
{
    printf("get_user:%s\n", req.uri.c_str());
    if (req.method != "GET" && req.method != "POST")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    std::string accept_type = "application/xml";
    for (size_t t = 0; t < req.headers.size(); ++t)
    {
        std::string name = req.headers[t].name;
        std::transform(name.begin(), name.end(), name.begin(), tolower);
        if (name == "accept")
        {
            accept_type = req.headers[t].value;
        }
    }

    // /api/region/33/user
    std::string uri = req.uri;
    unsigned pos = uri.find("region");
    if (std::string::npos == pos)
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }
    unsigned end_pos = uri.find_last_of('/');
    if (std::string::npos == end_pos)
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }
    unsigned start_pos = pos+std::string("region").length()+1;
    std::string str_id = uri.substr(start_pos, end_pos - start_pos);

    std::string content;
    if (req.method == "GET")
    {
        if (false == GetUserMgr().GetUser_Xml(str_id, content))
        {
            rep = _stock_replies::stock_reply(http_status::internal_server_error);
            return ;
        }
    }
    else if (req.method == "POST") // 创建新的用户
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    rep.status = http_status::ok;
    rep.content = fcA2U(content.c_str());
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = accept_type;
}

void user_oper2(const _request& req, _reply& rep)
{
    printf("get_user_info:%s\n", req.uri.c_str());
    if (req.method != "GET" && req.method != "PUT" && req.method != "DELETE")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    std::string accept_type = "application/xml";
    for (size_t t = 0; t < req.headers.size(); ++t)
    {
        std::string name = req.headers[t].name;
        std::transform(name.begin(), name.end(), name.begin(), tolower);
        if (name == "accept")
        {
            accept_type = req.headers[t].value;
        }
    }

    std::string uri = req.uri;
    int pos = uri.find_last_of('/', std::string::npos);
    std::string str_id = uri.substr(pos+1);

    std::string content;
    if (req.method == "GET")
    {
        if (false == GetUserMgr().GetUserInfo_Xml(str_id, content))
        {
            rep = _stock_replies::stock_reply(http_status::internal_server_error);
            return ;
        }
    }
    else if (req.method == "PUT")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }
    else if (req.method == "DELETE")
    {
        rep = _stock_replies::stock_reply(http_status::bad_request);
        return ;
    }

    rep.status = http_status::ok;
    rep.content = fcA2U(content.c_str());
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = accept_type;
}
void trans_msg(const _request& req, _reply& rep)
{
    printf("trans_msg:%s\n", req.uri.c_str());
}
