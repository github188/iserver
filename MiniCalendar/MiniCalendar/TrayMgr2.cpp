#include "StdAfx.h"
#include "TrayMgr2.h"

CTrayMgr2& GetTrayMgr()
{
    static CTrayMgr2 theTray;
    return theTray;
}

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TASKBARCREATED) 
    {
        GetTrayMgr().OnTaskBarCreate(wParam, lParam);
    }

    return ::CallWindowProc(GetTrayMgr().m_pOldWndProc, hwnd, uMsg, wParam, lParam);
}

CTrayMgr2::CTrayMgr2(void)
{
    memset(&m_nid, 0, sizeof(NOTIFYICONDATA));
}

CTrayMgr2::~CTrayMgr2(void)
{
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

void CTrayMgr2::SetTrayNotify(HWND hWnd, UINT cbMsgId, HICON hIcon, const CString& strTitle, const CString& strMsg)
{
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd   = hWnd;
    m_nid.hIcon  = hIcon;
    m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
    m_nid.uCallbackMessage = cbMsgId;
    _tcscpy_s(m_nid.szTip, ARRAYSIZE(m_nid.szTip), strMsg);
    m_nid.uTimeout = 10;
    _tcscpy_s(m_nid.szInfoTitle, ARRAYSIZE(m_nid.szInfoTitle), strTitle);
    _tcscpy_s(m_nid.szInfo, ARRAYSIZE(m_nid.szInfo), strMsg);
    m_nid.dwInfoFlags = 0x00000004;

    Shell_NotifyIcon(NIM_ADD, &m_nid);

    // 替换默认的窗口消息处理过程函数
#pragma warning(disable: 4311 4312)
    m_pOldWndProc = (WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)HookWndProc);
#pragma warning(default: 4311 4312)
}


LRESULT CTrayMgr2::OnTaskBarCreate(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    Shell_NotifyIcon(NIM_ADD, &m_nid);
    return 0;
}