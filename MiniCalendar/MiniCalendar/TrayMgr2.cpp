#include "StdAfx.h"
#include "TrayMgr2.h"

CTrayMgr2& GetTrayMgr()
{
    static CTrayMgr2 theTray;
    return theTray;
}

CTrayMgr2::CTrayMgr2(void)
{
}

CTrayMgr2::~CTrayMgr2(void)
{
}
