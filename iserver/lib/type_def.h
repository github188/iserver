#ifndef __TYPE_DEF___
#define __TYPE_DEF___

#include <vector>

namespace http_status
{
    enum http_status_type
    {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        multiple_choices = 300,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503
    };
}

struct _header
{
	std::string name;
	std::string value;
};

struct _request
{
	std::string method;
	std::string uri;
	int http_version_major;
	int http_version_minor;
	std::vector<_header> headers;
	std::string extern_string;
};

struct _reply
{
    http_status::http_status_type status;
	std::string content;
	std::vector<_header> headers;
	std::string extern_string;
};

typedef void(_stdcall *FUN_DO_API)(const _request& req, _reply& rep, void* p);

#endif
