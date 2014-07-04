
// MiniCalendarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MiniCalendar.h"
#include "MiniCalendarDlg.h"
#include "DateDlg.h"

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
    , m_nWeekNum(6)
    , m_bInit(FALSE)
    , m_trayMgr(GetTrayMgr())
    , m_bLbDown(FALSE)
    , m_bMouseTracking(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_penLine.CreatePen(PS_SOLID, 1, BKG_LINE_COLOR);
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = 160;
	_tcscpy_s(lf.lfFaceName, _T("微软雅黑"));
	m_fontDate.CreatePointFontIndirect(&lf);

	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = 90;
	_tcscpy_s(lf.lfFaceName, _T("微软雅黑"));
	m_fontDay.CreatePointFontIndirect(&lf);
}

void CMiniCalendarDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_PRE, m_btnPreMonth);
    DDX_Control(pDX, IDC_BTN_NEXT, m_btnNextMonth);
}

BEGIN_MESSAGE_MAP(CMiniCalendarDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
    ON_WM_LBUTTONUP()
    ON_BN_CLICKED(IDC_BTN_PRE, &CMiniCalendarDlg::OnBnClickedBtnPre)
    ON_BN_CLICKED(IDC_BTN_NEXT, &CMiniCalendarDlg::OnBnClickedBtnNext)
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
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

	SetWindowText(_T("迷你日历"));

	CTime ct = CTime::GetTickCount();
	m_tToday = ct;
	InitDateInfo(ct.GetYear(), ct.GetMonth());

	SetTimer(CHECK_EVENT, 500, NULL);

    m_bInit = TRUE;
	MoveWindow(0, 0, 1366, 768);

    m_trayMgr.SetTrayNotify(m_hWnd, WM_TRAY_MSG, AfxGetApp()->LoadIcon(IDI_ICON_TRAY), _T("MiniCalendar"), _T("迷你日历V1.0"));

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
		// 基本信息
        DrawCanlendar(dc);
        DrawLines(dc);

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
	pDC->FillSolidRect(rcClient, DEFAULT_BKG_COLOR);
    return TRUE;
}

void CMiniCalendarDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    GetClientRect(m_rcClient);
    m_rcClient.DeflateRect(ROUND_BLACK, ROUND_BLACK, ROUND_BLACK, ROUND_BLACK);
    SetDayRect(m_rcClient);
	
    if (m_bInit)
    {
        m_btnPreMonth.MoveWindow(m_rcPreBtn);
        m_btnNextMonth.MoveWindow(m_rcNextBtn);
    }

    CRect rcTemp(m_rcClient);
    rcTemp.bottom = m_rcFakeTitle.bottom;
    rcTemp.left = m_rcNextBtn.right;
    InvalidateRect(rcTemp);
    rcTemp = m_rcClient;
    rcTemp.top = m_rcFakeTitle.bottom;
    InvalidateRect(rcTemp);
}

void CMiniCalendarDlg::DrawLines(CPaintDC& dc)
{    
    dc.SelectObject(&m_penLine);
    dc.SelectStockObject(HOLLOW_BRUSH);

    // 外边框
    dc.MoveTo(m_rcClient.left, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.top);

    // 按天分割
    for (int i = 0; i <= m_nWeekNum; ++i)
    {
        dc.MoveTo(m_rcClient.left, m_dayArea[i][0].rect().top);
        dc.LineTo(m_rcClient.right, m_dayArea[i][0].rect().top);
    }
    for (int i = 0; i < MAX_WEEK_COL; ++i)
    {
		dc.MoveTo(m_dayArea[0][i].rect().left, m_dayArea[0][0].rect().top);
		dc.LineTo(m_dayArea[0][i].rect().left, m_dayArea[m_nWeekNum][0].rect().bottom);
    }
}

void CMiniCalendarDlg::SetDayRect(const CRect& rcClient)
{
    int nDayHeight = (rcClient.Height() - DATE_BASE_HEIGHT - WEEK_NAME_HEIGHT) / m_nWeekNum;
	int nDayWidth = (rcClient.Width() - WEEK_NUM_WIDTH) / (MAX_WEEK_COL - 1);

    int nLeft = 0;
    int nTop = 0;
    int nRight = 0;
    int nBottom = 0;

	// 主体部分
    for (int i = m_nWeekNum; i > 0; --i)
    {
        nTop = rcClient.bottom - (m_nWeekNum - i + 1) * nDayHeight;
        nBottom = nTop + nDayHeight;

		for (int j = MAX_WEEK_COL - 1; j > 0; --j)
		{
			nLeft = rcClient.right - (MAX_WEEK_COL - j) * nDayWidth;
			nRight = nLeft + nDayWidth;

			m_dayArea[i][j].SetRect(CRect(nLeft, nTop, nRight, nBottom));
		}
    }

	// 周次
	nLeft = m_rcClient.left;
	nRight = m_dayArea[1][1].rect().left;
	for (int i = 0; i <= m_nWeekNum; ++i)
	{
		nTop = m_dayArea[i][1].rect().top;
		nBottom = m_dayArea[i][1].rect().bottom;
		m_dayArea[i][0].SetRect(CRect(nLeft, nTop, nRight, nBottom));
	}

	// 星期头部
    nTop = m_rcClient.top + DATE_BASE_HEIGHT;
	nBottom = m_dayArea[1][0].rect().top;
	for (int i = 0; i < MAX_WEEK_COL; ++i)
	{
		nLeft = m_dayArea[1][i].rect().left;
		nRight = nLeft + nDayWidth;
		m_dayArea[0][i].SetRect(CRect(nLeft, nTop, nRight, nBottom));
	}

    // 按钮
    nLeft = m_rcClient.left + LEFT_BLANK_WIDTH;
    nRight = m_rcClient.left + LEFT_BLANK_WIDTH + BTN_WIDTH;
    m_rcPreBtn = CRect(nLeft, m_rcClient.top+3, nRight, m_dayArea[0][0].rect().top-3);

    nLeft = m_rcPreBtn.right + 4;
    nRight = nLeft + 120;
    m_rcFakeTitle = CRect(nLeft, m_rcClient.top+1, nRight, m_dayArea[0][0].rect().top-1);

    nLeft = m_rcFakeTitle.right + 4;
    nRight = nLeft + BTN_WIDTH;
    m_rcNextBtn = CRect(nLeft, m_rcClient.top + 3, nRight, m_dayArea[0][0].rect().top - 3);

    m_rcDayArea = CRect(m_dayArea[1][1].rect().left, m_dayArea[1][1].rect().top,
        m_dayArea[m_nWeekNum][MAX_WEEK_COL - 1].rect().right, m_dayArea[m_nWeekNum][MAX_WEEK_COL - 1].rect().bottom);
}

void CMiniCalendarDlg::DrawCanlendar(CPaintDC& dc)
{
	dc.SetBkMode(TRANSPARENT);

	// 颜色
	PaintColor(dc);

	// 文字....
	PaintText(dc);
}

void CMiniCalendarDlg::InitDateInfo(int nYear, int nMonth)
{
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		for (int j = 1; j < MAX_WEEK_COL; ++j)
		{
			m_dayArea[i][j].clear();
		}
	}

	int nMonthDay = _date::GetMonthDay(nMonth, nYear);

	CTime t;
	int nWeekNum = 1;
	for (int i = 1; i <= nMonthDay; ++i)
	{
		t = CTime(nYear, nMonth, i, 0, 0, 0);
		int nWeekDay = t.GetDayOfWeek();
		m_dayArea[nWeekNum][nWeekDay].SetDate(t);
		m_dayArea[nWeekNum][nWeekDay].SetLunar(L"农历");
		m_dayArea[nWeekNum][nWeekDay].SetMonthFlag(true);

		if (7 == nWeekDay)
		{
			++nWeekNum;
		}
	}
    // 补充其他空白的地方
    // 第一周前面会空，后两周后面会空
    for (int i = MAX_WEEK_COL - 1; i > 0; --i)
    {
        if (0 == m_dayArea[1][i].date().GetTime())
        {
            CTime tTemp = m_dayArea[1][i + 1].date() - CTimeSpan(1, 0, 0, 0);
            m_dayArea[1][i].SetDate(tTemp);
            m_dayArea[1][i].SetLunar(L"农历");
            m_dayArea[1][i].SetMonthFlag(false);
        }
    }
    for (int i = m_nWeekNum - 1; i <= m_nWeekNum; ++i)
    {
        for (int j = 1; j < MAX_WEEK_COL; ++j)
        {
            if (0 == m_dayArea[i][j].date().GetTime())
            {
                CTime tTemp;
                if (1 == j)
                {
                    tTemp = m_dayArea[i-1][MAX_WEEK_COL-1].date() + CTimeSpan(1, 0, 0, 0);
                }
                else
                {
                    tTemp = m_dayArea[i][j - 1].date() + CTimeSpan(1, 0, 0, 0);
                }
                m_dayArea[i][j].SetDate(tTemp);
                m_dayArea[i][j].SetLunar(L"农历");
                m_dayArea[i][j].SetMonthFlag(false);
            }
        }
    }

    m_tDisplayMonth = CTime(nYear, nMonth, 1, 0, 0, 0);
}

void CMiniCalendarDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcFakeTitle.PtInRect(point))
	{
		//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
        JumpToday();
	}
    else if (m_rcDayArea.PtInRect(point))
	{
		//AddSelect(point, (0 > GetKeyState(VK_CONTROL)));
        AddSelect(point, FALSE);
        m_bLbDown = TRUE;
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CMiniCalendarDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bLbDown = FALSE;
    m_tLBMoving = CTime(0);
    m_tLBDown = CTime(0);

    CDialog::OnLButtonUp(nFlags, point);
}

void CMiniCalendarDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bMouseTracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);   //监控鼠标离开   
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = this->m_hWnd;
        if (TrackMouseEvent(&tme))
        {
            m_bMouseTracking = FALSE;            //其他鼠标进入时的操作      
        }
    }
    if (m_bLbDown && m_rcDayArea.PtInRect(point))
    {
        TrackMouseArea(point);
    }

    CDialog::OnMouseMove(nFlags, point);
}

void CMiniCalendarDlg::OnMouseLeave()
{
    m_bLbDown = FALSE;
    m_tLBMoving = CTime(0);
    m_tLBDown = CTime(0);
    m_bMouseTracking = TRUE;

    CDialog::OnMouseLeave();
}

void CMiniCalendarDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == CHECK_EVENT)
	{
		CheckDay();
	}

	CDialog::OnTimer(nIDEvent);
}

void CMiniCalendarDlg::CheckDay()
{
	CTime ct = CTime::GetTickCount();
	if (ct.GetDay() != m_tToday.GetDay() && ct.GetMonth() == m_tToday.GetMonth())	// 时间有变化
	{
		m_tToday = ct;
		InvalidateDay();
	}
	else if (ct.GetMonth() != m_tToday.GetMonth())
	{
		m_tToday = ct;
		InvalidateMonth();
	}
}

void CMiniCalendarDlg::InvalidateDay()
{
	InvalidateRect(m_lastToday->rect());
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		for (int j = 1; j < MAX_WEEK_COL; ++j)
		{
			if (m_dayArea[i][j].is_today())
			{
				m_lastToday = &m_dayArea[i][j];
				break;
			}
		}
	}
	InvalidateRect(m_lastToday->rect());
}

void CMiniCalendarDlg::InvalidateMonth()
{
	CTime ct = CTime::GetTickCount();
	InitDateInfo(ct.GetYear(), ct.GetMonth());
	Invalidate(TRUE);
}


void CMiniCalendarDlg::PaintText(CPaintDC& dc)
{
	// 日期
    dc.SetTextColor(BLACK_COLOR);
	dc.SelectObject(&m_fontDate);
	CRect rcArea = m_rcFakeTitle;
	CRect rcTemp = rcArea;
    CString strText = m_tDisplayMonth.Format(_T("%Y年%m月"));
	int nTemp = dc.DrawText(strText, &rcTemp, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	rcArea.top += (rcArea.Height() - nTemp) / 2;
    dc.DrawText(strText, &rcArea, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);

	dc.SelectObject(&m_fontDay);
	// 周次
	rcArea = CRect(m_dayArea[1][0].rect().left, m_dayArea[1][0].rect().top,
		m_dayArea[1][0].rect().right, m_dayArea[m_nWeekNum][0].rect().bottom);
	dc.FillSolidRect(rcArea, WEEK_NUM_COLOR);
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		strText = m_dayArea[i][1].date().Format(_T("%U周"));
		rcTemp = m_dayArea[i][0].rect();
		nTemp = dc.DrawText(strText, &rcTemp, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
		rcArea = m_dayArea[i][0].rect();
		rcArea.top += (rcArea.Height() - nTemp) / 2;
		dc.DrawText(strText, &rcArea, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	}

	// 星期头部
	rcArea = CRect(m_dayArea[0][1].rect().left, m_dayArea[0][0].rect().top,
		m_dayArea[0][MAX_WEEK_COL - 1].rect().right, m_dayArea[0][0].rect().bottom);
	dc.FillSolidRect(rcArea, WEEK_NAME_COLOR);
	for (int i = 0; i < MAX_WEEK_COL; i++)
	{
		strText = CString(_T("    ")) + _date::week.Day(i);
		rcTemp = m_dayArea[0][i].rect();
		nTemp = dc.DrawText(strText, &rcTemp, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
		rcArea = m_dayArea[0][i].rect();
		rcArea.top += (rcArea.Height() - nTemp) / 2;
		dc.DrawText(strText, &rcArea, DT_EDITCONTROL | DT_WORDBREAK);
	}
	// 天
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		for (int j = 0; j < MAX_WEEK_COL; ++j)
		{
			if (0 == m_dayArea[i][j].date().GetTime())
			{
				continue;
			}

			if (m_dayArea[i][j].is_today())
			{
				dc.SetTextColor(WHITE_COLOR);
			}
            else if (!m_dayArea[i][j].this_month())
            {
                dc.SetTextColor(NOT_THIS_MONTH_COLOR);
            }

            strText.Format(_T("%2d"), m_dayArea[i][j].date().GetDay());
            rcTemp = m_dayArea[i][j].rect();
            nTemp = dc.DrawText(strText, &rcTemp, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
            rcArea = m_dayArea[i][j].rect();
            rcArea.bottom = rcArea.top + DAY_HEIGHT;
            rcArea.top += (rcArea.Height() - nTemp) / 2;
            rcArea.left += 7;
            dc.DrawText(strText, &rcArea, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
            strText = m_dayArea[i][j].lunar();
            rcArea.right -= 7;
            dc.DrawText(strText, &rcArea, DT_RIGHT | DT_EDITCONTROL | DT_WORDBREAK);

            if (m_dayArea[i][j].is_today() || !m_dayArea[i][j].this_month())
			{
				dc.SetTextColor(BLACK_COLOR);
            }
		}
	}
}

void CMiniCalendarDlg::PaintColor(CPaintDC& dc)
{
    CRect rcTemp;
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		for (int j = 1; j < MAX_WEEK_COL; ++j)
		{
			if (m_dayArea[i][j].select())
			{
                rcTemp = m_dayArea[i][j].rect();
                rcTemp.DeflateRect(1, 1, 0, 0);
				dc.FillSolidRect(rcTemp, SELECT_DAY_COLOR);
			}
            else
            {
                rcTemp = m_dayArea[i][j].rect();
                rcTemp.DeflateRect(1, 1, 0, 0);
                dc.FillSolidRect(rcTemp, DEFAULT_BKG_COLOR);
            }
			if (m_dayArea[i][j].is_today())
			{
                CRect rcTemp = m_dayArea[i][j].rect();
                rcTemp.bottom = rcTemp.top + DAY_HEIGHT;
                rcTemp.DeflateRect(1, 1, 0, 0);
				dc.FillSolidRect(rcTemp, TODAY_COLOR);
				m_lastToday = &m_dayArea[i][j];
			}
		}
	}
}

void CMiniCalendarDlg::UnselectAll()
{
	for (std::list<_date::DAY_INFO*>::iterator itr = m_selectDay.begin();
		itr != m_selectDay.end(); ++itr)
	{
		(*itr)->SetSelect(false);
		InvalidateRect((*itr)->rect());
	}

	m_selectDay.clear();
}

void CMiniCalendarDlg::AddSelect(CPoint pt, bool bCtrl)
{
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		for (int j = 1; j < MAX_WEEK_COL; ++j)
		{
			if (m_dayArea[i][j].rect().PtInRect(pt))
			{
				if (!m_dayArea[i][j].select() && bCtrl)		// 多选 +1
				{
					m_dayArea[i][j].SetSelect(true);
					m_selectDay.push_back(&m_dayArea[i][j]);
					InvalidateRect(m_dayArea[i][j].rect());
				}
				else if (m_dayArea[i][j].select() && bCtrl)	// 多选 -1
				{
					m_dayArea[i][j].SetSelect(false);
					RemoveSelect(i, j);
					InvalidateRect(m_dayArea[i][j].rect());
				}
				else if (!m_dayArea[i][j].select() && !bCtrl)	// 单选
				{
					UnselectAll();
					m_dayArea[i][j].SetSelect(true);
					m_selectDay.push_back(&m_dayArea[i][j]);
					InvalidateRect(m_dayArea[i][j].rect());
                    // 记录鼠标点中区域的时间
                    m_tLBDown = m_dayArea[i][j].date();
				}
				else if (m_dayArea[i][j].select() && !bCtrl)    // 取消选择
				{
					if (m_selectDay.size() > 1)
					{
						UnselectAll();
						m_dayArea[i][j].SetSelect(true);
						m_selectDay.push_back(&m_dayArea[i][j]);
                        InvalidateRect(m_dayArea[i][j].rect());
                        // 记录鼠标点中区域的时间
                        m_tLBDown = m_dayArea[i][j].date();
					}
				}
			}
		}
	}
}

void CMiniCalendarDlg::RemoveSelect(int i, int j)
{
	for (std::list<_date::DAY_INFO*>::iterator itr = m_selectDay.begin();
		itr != m_selectDay.end(); ++itr)
	{
		if ((*itr) == &m_dayArea[i][j])
		{
			m_selectDay.erase(itr);
			break;
		}
	}
}


void CMiniCalendarDlg::OnBnClickedBtnPre()
{
    int nYear = m_tDisplayMonth.GetYear();
    int nMonth = m_tDisplayMonth.GetMonth() - 1;
    if (0 == nMonth)
    {
        --nYear;
        nMonth = 12;
    }
    ClearSelect();
    InitDateInfo(nYear, nMonth);
    InvalidateText();
}


void CMiniCalendarDlg::OnBnClickedBtnNext()
{
    int nYear = m_tDisplayMonth.GetYear();
    int nMonth = m_tDisplayMonth.GetMonth() + 1;
    if (13 == nMonth)
    {
        ++nYear;
        nMonth = 1;
    }
    ClearSelect();
    InitDateInfo(nYear, nMonth);
    InvalidateText();
}


void CMiniCalendarDlg::InvalidateText()
{
    InvalidateRect(m_rcFakeTitle);

    for (int i = 1; i <= m_nWeekNum; ++i)
    {
        for (int j = 1; j < MAX_WEEK_COL; ++j)
        {
            CRect rcTemp = m_dayArea[i][j].rect();
            rcTemp.bottom = rcTemp.top + DAY_HEIGHT;
            rcTemp.DeflateRect(1, 1, 1, 0);
            InvalidateRect(rcTemp);
        }
    }
    for (int i = 1; i < m_nWeekNum; ++i)
    {
        CRect rcTemp = m_dayArea[i][0].rect();
        rcTemp.DeflateRect(1, 1, 1, 1);
        InvalidateRect(rcTemp);
    }
}


void CMiniCalendarDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CDialog::OnLButtonDblClk(nFlags, point);
}

void CMiniCalendarDlg::ClearSelect(void)
{
    for (int i = 1; i < m_nWeekNum; ++i)
    {
        for (int j = 1; j < MAX_WEEK_COL; ++j)
        {
            if (m_dayArea[i][j].select())
            {
                // 不需要set false, 会被清空
                //m_dayArea[i][j].SetSelect(false);
                InvalidateRect(m_dayArea[i][j].rect());
            }
        }
    }
}

void CMiniCalendarDlg::JumpToday(void)
{
    if (m_tDisplayMonth.GetYear() == m_tToday.GetYear() &&
        m_tDisplayMonth.GetMonth() == m_tToday.GetMonth())
    {
        return ;
    }
    int nYear = m_tToday.GetYear();
    int nMonth = m_tToday.GetMonth();
    ClearSelect();
    InitDateInfo(nYear, nMonth);
    InvalidateText();
}

void CMiniCalendarDlg::TrackMouseArea(CPoint& pt)
{
    TRACE("bad %d, %d\n", pt.x, pt.y);
    for (int i = 1; i <= m_nWeekNum; ++i)
    {
        for (int j = 1; j < MAX_WEEK_COL; ++j)
        {
            TRACE("***(%d,%d) %d, %d, %d, %d\n",i,j, m_dayArea[i][j].rect().left, m_dayArea[i][j].rect().right,
                m_dayArea[i][j].rect().top, m_dayArea[i][j].rect().bottom);
            if (m_dayArea[i][j].rect().PtInRect(pt))
            {
                m_tLBMoving = m_dayArea[i][j].date();
                TRACE("~~~ %I64d, %I64d\n", m_tLBMoving, m_tLBDown);
                break;
            }
        }
    }
    if (CTime(0) == m_tLBMoving)
    {
        TRACE("bad track\n");
        return;
    }

    for (int i = 1; i <= m_nWeekNum; ++i)
    {
        for (int j = 1; j < MAX_WEEK_COL; ++j)
        {
            if (min(m_tLBMoving, m_tLBDown) <= m_dayArea[i][j].date() &&
                max(m_tLBMoving, m_tLBDown) >= m_dayArea[i][j].date())
            {
                if (m_dayArea[i][j].select())
                {
                    continue;
                }
                m_dayArea[i][j].SetSelect(true);
                m_selectDay.push_back(&m_dayArea[i][j]);
                InvalidateRect(m_dayArea[i][j].rect());
                TRACE("select %d,%d\n", i, j);
            }
            else
            {
                if (m_dayArea[i][j].select())
                {
                    m_dayArea[i][j].SetSelect(false);
                    RemoveSelect(i, j);
                    InvalidateRect(m_dayArea[i][j].rect());
                }
            }
        }
    }

}




