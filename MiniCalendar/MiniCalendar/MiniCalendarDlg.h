
// MiniCalendarDlg.h : 头文件
//

#pragma once
#include "data_def.h"

// CMiniCalendarDlg 对话框
class CMiniCalendarDlg : public CDialog
{
// 构造
public:
	CMiniCalendarDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MINICALENDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	_date::DAY_INFO m_dayArea[MAX_WEEK_ROW][MAX_WEEK_COL];  // 第0行用来显示星期头
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
	
	// 当前时间
	CTime m_tToday;

	// 日期栏大小，作为假的标题栏
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
