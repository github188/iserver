#pragma once

#include "data_def.h"

// CEventDlg 对话框

class CEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventDlg)

public:
	CEventDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEventDlg();

// 对话框数据
	enum { IDD = IDD_DLG_EVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
};
