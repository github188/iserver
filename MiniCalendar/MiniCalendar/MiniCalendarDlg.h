
// MiniCalendarDlg.h : ͷ�ļ�
//

#pragma once
#include "data_def.h"
#include "afxwin.h"
#include "TrayMgr2.h"

// CMiniCalendarDlg �Ի���
class CMiniCalendarDlg : public CDialog
{
// ����
public:
	CMiniCalendarDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MINICALENDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedBtnPre();
    afx_msg void OnBnClickedBtnNext();
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();

    void DrawLines(CPaintDC& dc);
    void SetDayRect(const CRect& rcClient);
	void DrawCanlendar(CPaintDC& dc);
	void PaintText(CPaintDC& dc);
    void PaintColor(CPaintDC& dc);
    void InitDateInfo(int nYear, int nMonth);
    void CheckDay();
    void InvalidateDay();
    void InvalidateMonth();
    void UnselectAll();
    void AddSelect(CPoint pt, bool bCtrl);
    void RemoveSelect(int i, int j);
    void InvalidateText();
    void ClearSelect(void);
    void JumpToday(void);
    void TrackMouseArea(CPoint& pt);

	_date::DAY_INFO* m_lastToday;
	std::list<_date::DAY_INFO*> m_selectDay;
    std::list<_date::_date_info> m_lsDate;

	CPen m_penLine;
	CFont m_fontDay;
	CFont m_fontDate;

    CRect m_rcClient;
    CRect m_rcDayArea;
    _date::DAY_INFO m_dayArea[MAX_WEEK_ROW][MAX_WEEK_COL];  // ��0��������ʾ����ͷ
    int m_nWeekNum;
	// ��ǰʱ��
	CTime m_tToday;

	// ��������С����Ϊ�ٵı�����
	CRect m_rcFakeTitle;

    BOOL m_bInit;
	CRect m_rcPreBtn;
	CRect m_rcNextBtn;
    CButton m_btnPreMonth;
    CButton m_btnNextMonth;
    CTime m_tDisplayMonth;

    CTrayMgr2& m_trayMgr;
    BOOL m_bLbDown;
    BOOL m_bMouseTracking;

    CTime m_tLBDown, m_tLBMoving;

};
