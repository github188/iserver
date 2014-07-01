#pragma once

class CTrayMgr2
{
public:
    CTrayMgr2(void);
    ~CTrayMgr2(void);

    void SetTrayNotify(HWND hWnd, UINT cbMsgId, HICON hIcon, const CString& strTitle, const CString& strMsg);

    LRESULT OnTaskBarCreate(WPARAM wParam, LPARAM lParam);	// �������ؽ���Ϣ������
    WNDPROC m_pOldWndProc;		// original window proc
private:
     NOTIFYICONDATA m_nid;
};

CTrayMgr2& GetTrayMgr();