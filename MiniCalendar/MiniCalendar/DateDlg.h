#pragma once
#include "data_def.h"

// CDateDlg �Ի���

class CDateDlg : public CDialog
{
	DECLARE_DYNAMIC(CDateDlg)

public:
    CDateDlg(const CTime& tStart, const CTime& tStop, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDateDlg();

// �Ի�������
	enum { IDD = IDD_DLG_DATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
