// DateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MiniCalendar.h"
#include "DateDlg.h"


// CDateDlg �Ի���

IMPLEMENT_DYNAMIC(CDateDlg, CDialog)

CDateDlg::CDateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDateDlg::IDD, pParent)
{

}

CDateDlg::~CDateDlg()
{
}

void CDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDateDlg, CDialog)
END_MESSAGE_MAP()


// CDateDlg ��Ϣ�������
