
// MiniCalendar.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMiniCalendarApp:
// �йش����ʵ�֣������ MiniCalendar.cpp
//

class CMiniCalendarApp : public CWinAppEx
{
public:
	CMiniCalendarApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMiniCalendarApp theApp;