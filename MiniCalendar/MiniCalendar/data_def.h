
#ifndef __DATA_DEF__
#define __DATA_DEF__

#include <list>

const int ROUND_BLACK       = 3;
const int MAX_WEEK_ROW      = 7;
const int MAX_WEEK_COL      = 8;
const int DATE_BASE_HEIGHT  = 35;
const int WEEK_NAME_HEIGHT  = 25;
const int DAY_HEIGHT        = 21;
const int CHECK_EVENT       = 1;
const int WEEK_NUM_WIDTH    = 18;
const int DAY_WIDTH         = 32;

const int LEFT_BLANK_WIDTH  = 24;
const int BTN_WIDTH         = 24;

const int WM_TRAY_MSG       = WM_USER + 2014;

const COLORREF BKG_LINE_COLOR   = RGB(222, 222, 222);
const COLORREF WEEK_NUM_COLOR   = RGB(250, 250, 222);
const COLORREF WEEK_NAME_COLOR  = RGB(225, 242, 250);
const COLORREF TODAY_COLOR      = RGB(0, 114, 198);
const COLORREF SELECT_DAY_COLOR = RGB(153, 200, 233);
const COLORREF WHITE_COLOR      = RGB(255, 255, 255);
const COLORREF BLACK_COLOR      = RGB(0, 0, 0);
const COLORREF DEFAULT_BKG_COLOR = RGB(248, 248, 248);
const COLORREF NOT_THIS_MONTH_COLOR = RGB(150, 150, 150);

namespace _date
{

    class _date_info
    {
    private:
        CString m_strSubject;
        CTime m_tStart, m_tStop;
        BOOL m_bAllday;
        CString m_strPlace;
        int m_nRemindTime;
        CString m_strDetail;

    public:
        void SetSubject(const CString& strSubject)
        {
            m_strSubject = strSubject;
        }
        const CString& subject() const
        {
            return m_strSubject;
        }

        void SetDateTime(const CTime& tStart, const CTime& tStop, BOOL bAllday)
        {
            m_tStart = tStart;
            m_tStop = tStop;
            m_bAllday = bAllday;
        }
        const CTime& start() const
        {
            return m_tStart;
        }
        const CTime& stop() const
        {
            return m_tStop;
        }
        BOOL allday(){ return m_bAllday; }

        void SetPlace(const CString& strPlace)
        {
            m_strPlace = strPlace;
        }
        const CString& place() const
        {
            return m_strPlace;
        }

        void SetRemindTime(int nRemindTime){ m_nRemindTime = nRemindTime; }
        int remindtime(){ return m_nRemindTime; }

        void SetDetail(const CString& strDetail)
        {
            m_strDetail = strDetail;
        }
        const CString& detail() const
        {
            return m_strDetail;
        }
    };

    typedef class _day_info
    {
    private:
        CRect m_rcDay;
        CTime m_date;
        CString m_strLunarDate;
        std::list<_date_info> lsDate;
        bool m_bSelect;
        bool m_bThisMonth;

    public:
        _day_info()
            : m_rcDay(CRect(0,0,0,0))
            , m_date(CTime(0))
            , m_bSelect(false)
            , m_bThisMonth(false)
        {

        }
        void clear()
        {
            //m_rcDay = CRect(0, 0, 0, 0);
            m_date = CTime(0);
            m_bSelect = false;
        }

        void SetRect(const CRect& rc)
        {
            m_rcDay.CopyRect(rc);
        }

        const CRect& rect() const
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
        void SetSelect(bool bSelect)
        {
            m_bSelect = bSelect;
        }
        bool select()
        {
            return m_bSelect;
        }
        void SetMonthFlag(bool bFlag)
        {
            m_bThisMonth = bFlag;
        }
        bool this_month()
        {
            return m_bThisMonth;
        }
        bool is_today()
        {
            CTime ct = CTime::GetTickCount();
            if (m_date.GetDay() == ct.GetDay() &&
                m_date.GetMonth() == ct.GetMonth() &&
                m_date.GetYear() == ct.GetYear())
            {
                return true;
            }
            return false;
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