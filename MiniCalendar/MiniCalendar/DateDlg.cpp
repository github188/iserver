// DateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MiniCalendar.h"
#include "DateDlg.h"


// CDateDlg 对话框

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


// CDateDlg 消息处理程序
