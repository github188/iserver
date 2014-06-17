// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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
