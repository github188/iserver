// DateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MiniCalendar.h"
#include "DateDlg.h"


// CDateDlg �Ի���

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


// CDateDlg ��Ϣ�������


BOOL CDateDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    InitCtrls();
    m_bInit = true;
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
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
    // TODO:  �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}


BOOL CDateDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

    return CDialog::OnEraseBkgnd(pDC);
}


void CDateDlg::InitCtrls()
{
}
