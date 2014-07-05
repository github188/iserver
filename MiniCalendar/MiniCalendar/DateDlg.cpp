// DateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MiniCalendar.h"
#include "DateDlg.h"


// CDateDlg 对话框

IMPLEMENT_DYNAMIC(CDateDlg, CDialog)

CDateDlg::CDateDlg(const CTime& tStart, const CTime& tStop, CWnd* pParent /*=NULL*/)
	: CDialog(CDateDlg::IDD, pParent)
    , m_tStart(tStart)
    , m_tStop(tStop)
    , m_bInit(false)
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
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDateDlg 消息处理程序


BOOL CDateDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    InitCtrls();
    m_bInit = true;
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


void CDateDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_bInit)
    {
        
    }
}

void CDateDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO:  在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialog::OnPaint()
}


BOOL CDateDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值

    return CDialog::OnEraseBkgnd(pDC);
}


void CDateDlg::InitCtrls()
{
}
