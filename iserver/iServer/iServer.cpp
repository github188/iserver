// iServer.cpp : ���� DLL Ӧ�ó���ĵ���������
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


