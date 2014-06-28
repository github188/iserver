
// MiniCalendarDlg.h : ͷ�ļ�
//

#pragma once
#include "data_def.h"

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
    void DrawLines(CPaintDC& dc);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CRect m_rcClient;
	_date::DAY_INFO m_dayArea[MAX_WEEK_ROW][MAX_WEEK_COL];  // ��0��������ʾ����ͷ
    int m_nWeekNum;
    void SetDayRect(const CRect& rcClient);
	void DrawCanlendar(CPaintDC& dc);
	void PaintText(CPaintDC& dc);
	void PaintColor(CPaintDC& dc);

	_date::DAY_INFO* m_lastToday;
	std::list<_date::DAY_INFO*> m_selectDay;

	CPen m_penLine;
	CFont m_fontDay;
	CFont m_fontDate;
	
	// ��ǰʱ��
	CTime m_tToday;

	// ��������С����Ϊ�ٵı�����
	CRect m_rcFakeTitle;

	void InitDateInfo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void CheckDay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void InvalidateDay();
	void InvalidateMonth();
	void UnselectAll();
	void AddSelect(CPoint pt);
	void RemoveSelect(int i, int j);
};
