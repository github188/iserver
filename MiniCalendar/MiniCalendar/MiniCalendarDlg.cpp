
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
        DrawLines(dc);
        DrawDay(dc);

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
}

void CMiniCalendarDlg::DrawLines(CPaintDC& dc)
{
    CPen pLine;
    pLine.CreatePen(PS_SOLID, 1, BKG_LINE_COLOR);
    CPen *pOld = dc.SelectObject(&pLine);
    dc.SelectStockObject(HOLLOW_BRUSH);

    // ��߿�
    dc.MoveTo(m_rcClient.left, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.bottom);
    dc.LineTo(m_rcClient.right, m_rcClient.top);
    dc.LineTo(m_rcClient.left, m_rcClient.top);

    // ����ָ�
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
