#pragma once


// CDateDlg �Ի���

class CDateDlg : public CDialog
{
	DECLARE_DYNAMIC(CDateDlg)

public:
	CDateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDateDlg();

// �Ի�������
	enum { IDD = IDD_DLG_DATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
