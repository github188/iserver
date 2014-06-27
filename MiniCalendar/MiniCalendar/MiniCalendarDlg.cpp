
// MiniCalendarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MiniCalendar.h"
#include "MiniCalendarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMiniCalendarDlg 对话框


CMiniCalendarDlg::CMiniCalendarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniCalendarDlg::IDD, pParent)
    , m_nWeekNum(5)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniCalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMiniCalendarDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CMiniCalendarDlg 消息处理程序

BOOL CMiniCalendarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMiniCalendarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMiniCalendarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CPaintDC dc(this);
        DrawLines(dc);
        DrawDay(dc);

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMiniCalendarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CMiniCalendarDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rcClient;
    GetClientRect(rcClient);
    pDC->FillSolidRect(rcClient, RGB(248, 248, 248));

    return TRUE;
}

void CMiniCalendarDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    GetClientRect(m_rcClient);
    m_rcClient.DeflateRect(ROUND_BLACK, ROUND_BLACK, ROUND_BLACK, ROUND_BLACK);
    SetDayRect(m_rcClient);
}

void CMiniCalendarDlg::DrawLines(CPaintDC& dc)
{
    CPen pLine;
    pLine.CreatePen(PS_SOLID, 1, BKG_LINE_COLOR);
    CPen *pOld = dc.SelectObject(&pLine);
    dc.SelectStockObject(HOLLOW_BRUSH);

    // 外边框
    dc.MoveTo(m_rcClient.left, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.top);

    // 按天分割
    for (int i = 0; i < m_nWeekNum; ++i)
    {
        dc.MoveTo(m_rcClient.left, m_rcDay[i][0].top);
        dc.LineTo(m_rcClient.right, m_rcDay[i][0].top);
    }
    for (int i = 0; i < WEEK_DAY; ++i)
    {
        dc.MoveTo(m_rcDay[0][i].left, m_rcClient.top);
        dc.LineTo(m_rcDay[0][i].left, m_rcClient.bottom);
    }
}

void CMiniCalendarDlg::SetDayRect(const CRect& rcClient)
{
    int nDayHeight = (rcClient.Height() - DATE_BASE_HEIGHT - WEEK_NAME_HEIGHT) / m_nWeekNum;
    int nDayWidth = rcClient.Width() / 7;

    int nLeft = 0;
    int nTop = 0;
    int nRight = 0;
    int nBottom = 0;

    for (int i = m_nWeekNum; i > 0; --i)
    {
        nTop = rcClient.bottom - (m_nWeekNum - i) * nDayHeight;
        nBottom = nTop + nDayHeight;

        for (int j = 0; j < WEEK_DAY; ++j)
        {
            nLeft = rcClient.left + j * nDayWidth;
            nRight = nLeft + nDayWidth;

            m_rcDay[i][j] = CRect(nLeft, nTop, nRight, nBottom);
        }
    }
}

void CMiniCalendarDlg::DrawDay(CPaintDC& dc)
{
    CString strDay;
    for (int i = 0; i < m_nWeekNum; ++i)
    {
        for (int j = 0; j < WEEK_DAY; ++j)
        {
            strDay.Format("%d,%d", i, j);

            dc.TextOut(m_rcDay[i][j].left, m_rcDay[i][j].top, strDay);
        }
    }
}
