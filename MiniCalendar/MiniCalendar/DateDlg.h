#pragma once
#include "data_def.h"

// CDateDlg 对话框

class CDateDlg : public CDialog
{
	DECLARE_DYNAMIC(CDateDlg)

public:
    CDateDlg(const CTime& tStart, const CTime& tStop, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDateDlg();

// 对话框数据
	enum { IDD = IDD_DLG_DATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    void InitCtrls();
    bool m_bInit;
    CTime m_tStart;
    CTime m_tStop;
};
