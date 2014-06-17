// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>



// TODO: 在此处引用程序需要的其他头文件
#include <WinSock2.h>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/threadpool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <boost/date_time.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>


#ifdef _UNICODE
#define _tstring wstring
#else
#define _tstring string
#endif

#include "../lib/BoostMutex.h"
#include "../lib/utils.h"
#include "../lib/Markup.h"
#include "../lib/fcTchar.h"

#include "../lib/message.pb.h"
#pragma comment(lib, "../lib/message.lib")
using namespace Chat;

#include "../lib/libtransfer.h"
#pragma comment(lib, "../lib/libtransfer.lib")

#include "../lib/libhttp.h"
#pragma comment(lib, "../lib/libhttp.lib")

#include "../lib/librest.h"
#pragma comment(lib, "../lib/librest.lib")
