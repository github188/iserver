
#ifndef __LIB_REST__
#define __LIB_REST__

#ifdef LIBREST_EXPORTS
#define LIBREST_API __declspec(dllexport)
#else
#define LIBREST_API __declspec(dllimport)
#endif

#include <string>
#include "type_def.h"

#ifdef __cplusplus //c++
extern "C" {
#endif

	typedef void(*REST_FUN)(const _request& req, _reply& rep);
	
	// api:必须传入正则表达式，否则可能倒是无法匹配
	LIBREST_API int rest_fun_regist(const std::string& api, REST_FUN rest_fun);

	// api:传入请求uri即可，将会按照注册函数进行正则匹配调用相应的函数
	LIBREST_API int rest_fun_set(const _request& req, _reply& rep);


#ifdef __cplusplus //定义c++命名规则
}
#endif
#endif