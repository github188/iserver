
#ifndef __DATA_DEF__
#define __DATA_DEF__

#include <list>

const int ROUND_BLACK       = 3;
const int MAX_WEEK_ROW      = 7;
const int MAX_WEEK_COL      = 8;
const int DATE_BASE_HEIGHT  = 35;
const int WEEK_NAME_HEIGHT  = 25;
const int DAY_HEIGHT		= 24;
const int CHECK_EVENT	    = 1;
const int WEEK_NUM_WIDTH	= 25;

const COLORREF BKG_LINE_COLOR = RGB(222,222,222);

namespace _date
{
	typedef class _day_info
	{
	private:
		CRect m_rcDay;
		CTime m_date;
		CString m_strLunarDate;
		std::list<CString> strLog;

	public:
		_day_info()
			: m_rcDay(CRect(0,0,0,0))
			, m_date(CTime(0))
		{

		}
		void SetRect(const CRect& rc)
		{
			m_rcDay.CopyRect(rc);
		}

		const CRect& rcDay() const
		{
			return m_rcDay;
		}

		void SetDate(const CTime& ct)
		{
			m_date = ct;
		}

		const CTime& date() const
		{
			return m_date;
		}
		
		void SetLunar(const CString& str)
		{
			m_strLunarDate = str;
		}

		const CString& lunar() const
		{
			return m_strLunarDate;
		}
	}DAY_INFO;



	const CString sunday = _T("星期日");
	const CString monday = _T("星期一");
	const CString tuesday = _T("星期二");
	const CString wednesday = _T("星期三");
	const CString thursday = _T("星期四");
	const CString friday = _T("星期五");
	const CString saturday = _T("星期六");

	class _Week
	{
	private:
		CString m_strWeekName[MAX_WEEK_COL + 1];
		CString m_strInvalide;
	public:
		_Week()
		{
			m_strWeekName[1] = sunday;
			m_strWeekName[2] = monday;
			m_strWeekName[3] = tuesday;
			m_strWeekName[4] = wednesday;
			m_strWeekName[5] = thursday;
			m_strWeekName[6] = friday;
			m_strWeekName[7] = saturday;

			m_strInvalide = _T("");
		}

		CString& Day(int n)
		{
			if (n > 0 && n <= MAX_WEEK_COL)
			{
				return m_strWeekName[n];
			}
			return m_strInvalide;
		}
	};
	static _Week week;
	
	static bool LeapYear(int nYear)
	{
		if (0 == nYear % 4 && 0 != nYear % 100)
		{
			return true;
		}
		if (0 == nYear % 400)
		{
			return true;
		}
		return false;
	}

	static int GetMonthDay(int nMonth, int nYear)
	{
		switch (nMonth)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 2:
		{
				  if (LeapYear(nYear))
				  {
					  return 29;
				  }
				  return 28;
		}
		default:
			return 30;
		}
	}

}
#endif