#pragma once
#ifndef __LIB_HTTP__
#define __LIT_HTTP__

#ifdef LIBHTTP_EXPORTS
#define LIBHTTP_API __declspec(dllexport)
#else
#define LIBHTTP_API __declspec(dllimport)
#endif
#include <string>
#include "type_def.h"

#ifdef __cplusplus //c++
extern "C" {
#endif
	
	LIBHTTP_API int http_run(const std::string& port, const unsigned num_threads, const std::string& doc_root, FUN_DO_API fun, void* p);
    LIBHTTP_API int http_stop();

#ifdef __cplusplus //定义c++命名规则
}
#endif
#endif