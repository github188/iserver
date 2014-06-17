// iServer.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "iServer.h"

#include "UserMgr.h"
#include "RestRequest.h"

ISERVER_API int fniServer(void)
{
    GetUserMgr().LoadRes();
    GetRestRequest().Start();
    return 0;
}


