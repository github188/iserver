
// MiniCalendarDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MiniCalendar.h"
#include "MiniCalendarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMiniCalendarDlg �Ի���


CMiniCalendarDlg::CMiniCalendarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniCalendarDlg::IDD, pParent)
    , m_nWeekNum(5)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_penLine.CreatePen(PS_SOLID, 1, BKG_LINE_COLOR);
	
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = 160;
	_tcscpy_s(lf.lfFaceName, _T("΢���ź�"));
	m_fontDate.CreatePointFontIndirect(&lf);

	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = 100;
	_tcscpy_s(lf.lfFaceName, _T("΢���ź�"));
	m_fontDay.CreatePointFontIndirect(&lf);
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
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMiniCalendarDlg ��Ϣ�������

BOOL CMiniCalendarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetWindowText(_T("MiniCanlendar"));
	MoveWindow(0, 0, 1024, 600);

	InitDateInfo();

	SetTimer(CHECK_EVENT, 1000, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMiniCalendarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CPaintDC dc(this);
		// ������Ϣ
        DrawCanlendar(dc);
        DrawLines(dc);

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	Invalidate(TRUE);
}

void CMiniCalendarDlg::DrawLines(CPaintDC& dc)
{    
    dc.SelectObject(&m_penLine);
    dc.SelectStockObject(HOLLOW_BRUSH);

    // ��߿�
    dc.MoveTo(m_rcClient.left, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.top);

    // ����ָ�
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

	// ���岿��
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

	// �ܴ�
	nLeft = m_rcClient.left;
	nRight = m_dayArea[1][1].rect().left;
	for (int i = 0; i <= m_nWeekNum; ++i)
	{
		nTop = m_dayArea[i][1].rect().top;
		nBottom = m_dayArea[i][1].rect().bottom;
		m_dayArea[i][0].SetRect(CRect(nLeft, nTop, nRight, nBottom));
	}

	// ����ͷ��
	nTop = m_dayArea[1][0].rect().top - WEEK_NAME_HEIGHT;
	nBottom = m_dayArea[1][0].rect().top;
	for (int i = 0; i < MAX_WEEK_COL; ++i)
	{
		nLeft = m_dayArea[1][i].rect().left;
		nRight = nLeft + nDayWidth;
		m_dayArea[0][i].SetRect(CRect(nLeft, nTop, nRight, nBottom));
	}

	m_rcFakeTitle = CRect(m_rcClient.left, m_rcClient.top, m_rcClient.right, m_dayArea[0][0].rect().top);
}

void CMiniCalendarDlg::DrawCanlendar(CPaintDC& dc)
{
	dc.SetBkMode(TRANSPARENT);

	// ��ɫ
	PaintColor(dc);

	// ����....
	PaintText(dc);
}


void CMiniCalendarDlg::InitDateInfo()
{
	CTime ct = CTime::GetTickCount();
	
	int nYear = ct.GetYear();
	int nMonth = ct.GetMonth();
	int nMonthDay = _date::GetMonthDay(nMonth, ct.GetYear());

	CTime t;
	int nWeekNum = 1;
	for (int i = 1; i <= nMonthDay; ++i)
	{
		t = CTime(nYear, nMonth, i, 0, 0, 0);
		int nWeekDay = t.GetDayOfWeek();
		m_dayArea[nWeekNum][nWeekDay].SetDate(t);
		m_dayArea[nWeekNum][nWeekDay].SetLunar(L"ũ��");

		if (7 == nWeekDay)
		{
			++nWeekNum;
		}
	}
	m_nWeekNum = nWeekNum;
	m_tToday = ct;
}

void CMiniCalendarDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcFakeTitle.PtInRect(point))
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}

	CDialog::OnLButtonDown(nFlags, point);
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
	if (ct.GetDay() != m_tToday.GetDay() && ct.GetMonth() == m_tToday.GetMonth())	// ʱ���б仯
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
	InvalidateRect(m_rcFakeTitle);
	// �������ɫ��Ҫ�޸�λ��
}

void CMiniCalendarDlg::InvalidateMonth()
{

}


void CMiniCalendarDlg::PaintText(CPaintDC& dc)
{
	// ����
	dc.SelectObject(&m_fontDate);
	CRect rcArea = m_rcFakeTitle;
	CRect rcTemp = rcArea;
	CString strText = m_tToday.Format(_T("%Y��%m��"));
	int nTemp = dc.DrawText(strText, &rcTemp, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	rcArea.top += (rcArea.Height() - nTemp) / 2;
	rcArea.left += 64;
	dc.DrawText(strText, &rcArea, DT_EDITCONTROL | DT_WORDBREAK);

	dc.SelectObject(&m_fontDay);
	// �ܴ�
	rcArea = CRect(m_dayArea[1][0].rect().left, m_dayArea[1][0].rect().top,
		m_dayArea[1][0].rect().right, m_dayArea[m_nWeekNum][0].rect().bottom);
	dc.FillSolidRect(rcArea, WEEK_NUM_COLOR);
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		strText = m_dayArea[i][1].date().Format(_T("%U��"));
		rcTemp = m_dayArea[i][0].rect();
		nTemp = dc.DrawText(strText, &rcTemp, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
		rcArea = m_dayArea[i][0].rect();
		rcArea.top += (rcArea.Height() - nTemp) / 2;
		dc.DrawText(strText, &rcArea, DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
	}

	// ����ͷ��
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
	// ��
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

			strText.Format(_T(" %2d��"), m_dayArea[i][j].date().GetDay());
			rcTemp = m_dayArea[i][j].rect();
			nTemp = dc.DrawText(strText, &rcTemp, DT_CALCRECT | DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
			rcArea = m_dayArea[i][j].rect();
			rcArea.bottom = rcArea.top + DAY_HEIGHT;
			rcArea.top += (rcArea.Height() - nTemp) / 2;
			dc.DrawText(strText, &rcArea, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);

			strText = m_dayArea[i][j].lunar() + _T(" ");
			dc.DrawText(strText, &rcArea, DT_RIGHT | DT_EDITCONTROL | DT_WORDBREAK);

			if (m_dayArea[i][j].is_today())
			{
				dc.SetTextColor(BLACK_COLOR);
			}
		}
	}
}

void CMiniCalendarDlg::PaintColor(CPaintDC& dc)
{
	for (int i = 1; i <= m_nWeekNum; ++i)
	{
		for (int j = 1; j < MAX_WEEK_COL; ++j)
		{
			if (!m_dayArea[i][j].is_today() && m_dayArea[i][j].select())
			{
				dc.FillSolidRect(m_dayArea[i][j].rect(), SELECT_DAY_COLOR);
			}
			if (m_dayArea[i][j].is_today())
			{
				dc.FillSolidRect(m_dayArea[i][j].rect(), TODAY_COLOR);
			}
		}
	}
}
