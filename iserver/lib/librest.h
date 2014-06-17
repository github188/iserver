
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
	
	// api:���봫��������ʽ��������ܵ����޷�ƥ��
	LIBREST_API int rest_fun_regist(const std::string& api, REST_FUN rest_fun);

	// api:��������uri���ɣ����ᰴ��ע�ắ����������ƥ�������Ӧ�ĺ���
	LIBREST_API int rest_fun_set(const _request& req, _reply& rep);


#ifdef __cplusplus //����c++��������
}
#endif
#endif