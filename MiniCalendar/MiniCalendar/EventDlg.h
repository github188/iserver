#pragma once

#include "data_def.h"

// CEventDlg �Ի���

class CEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventDlg)

public:
	CEventDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEventDlg();

// �Ի�������
	enum { IDD = IDD_DLG_EVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
};
