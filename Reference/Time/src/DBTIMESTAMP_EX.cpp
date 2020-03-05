#include "stdafx.h"
#include "CL/Time/DBTIMESTAMP_EX.h"
#include "CL/Time/strptime.h"

#include <ace/OS.h>
#include <ace/Date_Time.h> 
#include <time.h>

#include "CL/IO/vstring.h"
#include "CL/Time/PackedTime.h"

namespace CL
{
    DBTIMESTAMP_EX::DBTIMESTAMP_EX(::DBTIMESTAMP const& rhs)
    {
        * this = rhs;
    }

    DBTIMESTAMP_EX::DBTIMESTAMP_EX(SYSTEMTIME const& rhs)
    {
        * this = rhs;
    }

    DBTIMESTAMP_EX::DBTIMESTAMP_EX(PackedTime_ const& rhs)
    {
        * this = rhs;
    }

    DBTIMESTAMP_EX::DBTIMESTAMP_EX(ACE_Time_Value const& rhs)
    {
        *this = rhs;
    }

    DBTIMESTAMP_EX::DBTIMESTAMP_EX(FILETIME const& rhs)
    {
        *this = rhs;
    }

    void DBTIMESTAMP_EX::Clear()
    {
        year = 0;
        month = 0;
        day = 0;
        hour = 0;
        minute = 0;
        second = 0;
        fraction = 0;
    }

    void DBTIMESTAMP_EX::operator =(DBTIMESTAMP const& rhs)
    {
        year = rhs.year;
        month = rhs.month;
        day = rhs.day;
        hour = rhs.hour;
        minute = rhs.minute;
        second = rhs.second;
        fraction = rhs.fraction;
    }

    bool DBTIMESTAMP_EX::operator !() const
    {
        return IsNull();
    }

    DBTIMESTAMP_EX::operator DBTIMESTAMP const() const throw()
    {
        DBTIMESTAMP kTemp;

        kTemp.year = year;
        kTemp.month = month;
        kTemp.day = day;
        kTemp.hour = hour;
        kTemp.minute = minute;
        kTemp.second = second;
        kTemp.fraction = fraction;
        return kTemp;
    }

    DBTIMESTAMP_EX::operator SYSTEMTIME const() const throw()
    {
        SYSTEMTIME st;
        st.wDayOfWeek = (WORD)DayOfWeek();
        st.wYear = year;
        st.wMonth = month;
        st.wDay = day;
        st.wHour = hour;
        st.wMinute = minute;
        st.wSecond = second;
        st.wMilliseconds = (UINT16)fraction;
        return st;
    }

    DBTIMESTAMP_EX::operator FILETIME const() const throw()
    {
        FILETIME kFileTime;

        SYSTEMTIME const st = *this;

        ::SystemTimeToFileTime(&st, &kFileTime);

        return kFileTime;
    }

    DBTIMESTAMP_EX::operator ACE_Time_Value() const throw()
    {
        FILETIME const kFileTime = *this;

        return ACE_Time_Value(kFileTime);
    }

    DBTIMESTAMP_EX::operator T_TIME const() const throw()
    {
        return ((ACE_Time_Value)*this).get_msec();// .sec() * CL::SECOND;
    }

    T_TIME32 DBTIMESTAMP_EX::ToTime32() const
    {
        if (IsNotNull())
        {
            ACE_Time_Value aceTime = *this;
            return (CL::T_TIME32)aceTime.sec();
        }
        else
        {
            return 0;
        }
    }

    bool DBTIMESTAMP_EX::operator ==(DBTIMESTAMP_EX const& rhs) const
    {
        if (0 == ::memcmp(this, &rhs, sizeof(rhs)))
        {
            return true;
        }
        return false;
    }

    bool DBTIMESTAMP_EX::operator !=(DBTIMESTAMP_EX const& rhs) const
    {
        if (*this == rhs)
        {
            return false;
        }
        return true;
    }

    bool DBTIMESTAMP_EX::IsNull() const
    {
        if (0 == year
            && 0 == month
            && 0 == day
            && 0 == hour
            && 0 == minute
            && 0 == second
            && 0 == fraction
        )
        {
            return true;
        }
        return false;
    }

    bool DBTIMESTAMP_EX::IsNotNull() const
    {
        return !IsNull();
    }

    bool DBTIMESTAMP_EX::operator <(DBTIMESTAMP_EX const& rhs) const
    {
        if (year < rhs.year)
        {
            return true;
        }
        if (year > rhs.year)
        {
            return false;
        }
        //!-- >  year == rhs.year 의 조건이 만족 된다.
        if (month < rhs.month)
        {
            return true;
        }
        if (month > rhs.month)
        {
            return false;
        }

        if (day < rhs.day)
        {
            return true;
        }
        if (day > rhs.day)
        {
            return false;
        }

        if (hour < rhs.hour)
        {
            return true;
        }
        if (hour > rhs.hour)
        {
            return false;
        }

        if (minute < rhs.minute)
        {
            return true;
        }
        if (minute > rhs.minute)
        {
            return false;
        }

        if (second < rhs.second)
        {
            return true;
        }
        if (second > rhs.second)
        {
            return false;
        }

        if (fraction < rhs.fraction)
        {
            return true;
        }
        if (fraction > rhs.fraction)
        {
            return false;
        }

        return false;
    }

    bool DBTIMESTAMP_EX::operator >(DBTIMESTAMP_EX const& rhs) const
    {
        return (rhs < *this);
    }

    bool DBTIMESTAMP_EX::operator <=(DBTIMESTAMP_EX const& rhs) const
    {
        return (*this == rhs) || (*this < rhs);
    }

    bool DBTIMESTAMP_EX::operator >=(DBTIMESTAMP_EX const& rhs) const
    {
        return (*this == rhs) || (rhs < *this);
    }

    void DBTIMESTAMP_EX::Swap(DBTIMESTAMP_EX& rhs)
    {
        std::swap(year, rhs.year);
        std::swap(month, rhs.month);
        std::swap(day, rhs.day);
        std::swap(hour, rhs.hour);
        std::swap(minute, rhs.minute);
        std::swap(second, rhs.second);
        std::swap(fraction, rhs.fraction);
    }

    void DBTIMESTAMP_EX::SetUtcTime()
    {
        DBTIMESTAMP_EX now = Now();
        (*this) = now.ToUniversalTime();
    }

    ACE_Time_Value DBTIMESTAMP_EX::s_kBaseTime = ACE_OS::gettimeofday(); // ->네트웍이 준값.(레이턴시를 포함하라)
    ACE_Time_Value DBTIMESTAMP_EX::s_kStartTime = ACE_OS::gettimeofday(); // ->로컬에서 시간을 재기 시작한값??

    DBTIMESTAMP_EX DBTIMESTAMP_EX::Now()
    {
        auto kGab = ACE_OS::gettimeofday() - s_kStartTime;
        ACE_Time_Value kACENow = s_kBaseTime + (kGab);

        DBTIMESTAMP_EX kNow = DBTIMESTAMP_EX(kACENow);
        return kNow.ToLocalTime();

        //	DBTIMESTAMP_EX kNow;
        //	kNow.SetLocalTime();
        //    return kNow;
    }

	static DBTIMESTAMP_EX Now(T_TIME& time)
	{
		ACE_Time_Value avt;
		avt.set_msec(time);
		return DBTIMESTAMP_EX(avt);
	}

	static DBTIMESTAMP_EX Now(T_TIME32& time32)
	{
		ACE_Time_Value avt(time32);
		return DBTIMESTAMP_EX(avt);
	}

    DBTIMESTAMP_EX const& DBTIMESTAMP_EX::NullData()
    {
        static const DBTIMESTAMP_EX kNullData;
        return kNullData;
    }

    DBTIMESTAMP_EX const DBTIMESTAMP_EX::operator -(DBTIMESTAMP_EX const& rhs) const
    {
        auto kAceGab = (ACE_Time_Value)(*this) - (ACE_Time_Value)(rhs);

        return DBTIMESTAMP_EX(kAceGab);
    }

    DBTIMESTAMP_EX const DBTIMESTAMP_EX::operator +(DBTIMESTAMP_EX const& rhs) const
    {
        auto kAceGab = (ACE_Time_Value)(*this) + (ACE_Time_Value)(rhs);

        return DBTIMESTAMP_EX(kAceGab);
    }

    void DBTIMESTAMP_EX::operator =(PackedTime_ const& rhs)
    {
        if (rhs.IsNull())
        {
            Clear();
        }
        else
        {
            year = static_cast<SHORT>(rhs.m_kYear + PackedTime_::Limit::MIN_YEAR);
            month = static_cast<USHORT>(rhs.m_kMonth);
            day = static_cast<USHORT>(rhs.m_kDay);
            hour = static_cast<USHORT>(rhs.m_kHour);
            minute = static_cast<USHORT>(rhs.m_kMin);
            second = static_cast<USHORT>(rhs.m_kSec);
            fraction = 0;
        }
    }

    void DBTIMESTAMP_EX::operator =(SYSTEMTIME const& rhs)
    {
        year = rhs.wYear;
        month = rhs.wMonth;
        day = rhs.wDay;
        hour = rhs.wHour;
        minute = rhs.wMinute;
        second = rhs.wSecond;
        fraction = rhs.wMilliseconds;
    }

    void DBTIMESTAMP_EX::operator =(ACE_Time_Value const& rhs)
    {
        *this = static_cast<FILETIME>(rhs);
    }

    void DBTIMESTAMP_EX::operator =(FILETIME const& rhs)
    {
        SYSTEMTIME st = {0};
        if (rhs.dwLowDateTime != 0 || rhs.dwHighDateTime != 0)
        {
            ::FileTimeToSystemTime(&rhs, &st);
        }
        *this = st;
    }

    DBTIMESTAMP_EX DBTIMESTAMP_EX::ToLocalTime() const
    {
        SYSTEMTIME stLocal;

        SYSTEMTIME const stUtc = *this;

        ::SystemTimeToTzSpecificLocalTime(NULL, &stUtc, &stLocal);

        return DBTIMESTAMP_EX(stLocal);
    }

    DBTIMESTAMP_EX DBTIMESTAMP_EX::ToUniversalTime() const
    {
        SYSTEMTIME stUtc;

        SYSTEMTIME const stLocal = *this;

        ::TzSpecificLocalTimeToSystemTime(NULL, &stLocal, &stUtc);

        return DBTIMESTAMP_EX(stUtc);
    }

    eDayOfWeek::Enum DBTIMESTAMP_EX::DayOfWeek() const
    {
        static const int JAN_DAYS = 31,
            FEB_DAYS = 28,
            MAR_DAYS = 31,
            APR_DAYS = 30,
            MAY_DAYS = 31,
            JUN_DAYS = 30,
            JUL_DAYS = 31,
            AUG_DAYS = 31,
            SEP_DAYS = 30,
            OCT_DAYS = 31,
            NOV_DAYS = 30;

        //ref : http://phstudios.com/forums/index.php?/topic/67-c-calculate-day-of-week/
        int dayOfWeek = 0;
        if (day > 0 && day < 32 && month > 0 && month < 13 && year > 0)
        {
            // calculates the day of the week for December 31 of the previous year
            int decPrev = ((year - 1) * 365 + ((year - 1) / 4) - ((year - 1) / 100) + ((year - 1) / 400)) % 7;

            // adds the days of each month before the month entered
            switch (month)
            {
            case 12:{decPrev += NOV_DAYS;}
            case 11: {decPrev += OCT_DAYS;}
            case 10: {decPrev += SEP_DAYS;}
            case 9: {decPrev += AUG_DAYS;}
            case 8: {decPrev += JUL_DAYS;}
            case 7: {decPrev += JUN_DAYS;}
            case 6: {decPrev += MAY_DAYS;}
            case 5: {decPrev += APR_DAYS;}
            case 4: {decPrev += MAR_DAYS;}
            case 3:
                decPrev += FEB_DAYS;
                if (0 == (year % 4))
                {
                    decPrev += 1;
                }
            case 2:{decPrev += JAN_DAYS;}
            case 1:{decPrev += day;}break;
            default:
                break;
            }

            dayOfWeek = decPrev % eDayOfWeek::MAX;
        }

        return (eDayOfWeek::Enum)dayOfWeek;
    }

    int DBTIMESTAMP_EX::DayOfYear() const
    {
        return GetYDay();
    }

    int DBTIMESTAMP_EX::WeekOfYear(int const iso) const
    {
        /*
        (iso == 0)                        (iso == 1)
        2015-12-26  51                    2015-12-26      52      360
        2015-12-27  52                    2015-12-27      52      361
        2015-12-28  52                    2015-12-28      1       362
        2015-12-29  52                    2015-12-29      1       363
        2015-12-30  52                    2015-12-30      1       364
        2015-12-31  52                    2015-12-31      1       365
        2016-01-01  0                     2016-01-01      1       1
        2016-01-02  0                     2016-01-02      1       2
        2016-01-03  1                     2016-01-03      1       3
        2016-01-04  1                     2016-01-04      2       4
        2016-01-05  1                     2016-01-05      2       5
        2016-01-06  1                     2016-01-06      2       6
        2016-01-07  1                     2016-01-07      2       7
        2016-01-08  1                     2016-01-08      2       8
        2016-01-09  1                     2016-01-09      2       9
        2016-01-10  2                     2016-01-10      2       10
        2016-01-11  2                     2016-01-11      3       11
        2016-01-12  2                     2016-01-12      3       12
        2016-01-13  2                     2016-01-13      3       13
        2016-01-14  2                     2016-01-14      3       14
        */
        struct tm tm;
        tm.tm_mday = day;  // day of the month - [1, 31]
        tm.tm_mon = month - 1;   // months since January - [0, 11]
        tm.tm_year = year;;  // years since 1900
        tm.tm_wday = DayOfWeek();  // days since Sunday - [0, 6]
        tm.tm_yday = DayOfYear();  // days since January 1 - [0, 365]
        //tm.tm_isdst; // daylight savings time flag

        char format[32];
        strftime(format, sizeof(format), iso ? "%V" : "%U", &tm);
        return atoi(format);
    }

    //int DBTIMESTAMP_EX::WeekOfYear() const
    //{
    //    /*
    //    2015-12-26       52      360
    //    2015-12-27       53      361
    //    2015-12-28       53      362
    //    2015-12-29       53      363
    //    2015-12-30       53      364
    //    2015-12-31       53      365
    //    2016-01-01       1       1
    //    2016-01-02       1       2
    //    2016-01-03       2       3
    //    2016-01-04       2       4
    //    2016-01-05       2       5
    //    2016-01-06       2       6
    //    2016-01-07       2       7
    //    2016-01-08       2       8
    //    2016-01-09       2       9
    //    2016-01-10       3       10
    //    2016-01-11       3       11
    //    */
    //    CL::DBTIMESTAMP_EX First;
    //    First.year = year;
    //    First.month = 1;
    //    First.day = 1;

    //    int julian = DayOfYear();  // Jan 1 = 1, Jan 2 = 2, etc...
    //    int dow = DayOfWeek();     // Sun = 0, Mon = 1, etc...
    //    int dowJan1 = First.DayOfWeek();   // find out first of year's day
    //    //int badWeekNum = (julian / 7) + 1;  // Get our week# (wrong!  Don't use this)
    //    int weekNum = ((julian + 6) / 7);   // probably better.  CHECK THIS LINE. (See comments.)
    //    if (dow < dowJan1)                 // adjust for being after Saturday of week #1
    //    {
    //        ++weekNum;
    //    }
    //    return (weekNum);
    //}

    DBTIMESTAMP_EX& DBTIMESTAMP_EX::AddMilliseconds(int const value)
    {
        ACE_Time_Value const kDest(0, value * 1000);

        ACE_Time_Value kAceTime;
        kAceTime = (ACE_Time_Value)(*this);
        kAceTime += kDest;

        *this = kAceTime;
        return *this;
    }

    DBTIMESTAMP_EX& DBTIMESTAMP_EX::AddSeconds(__int64 const value)
    {
        ACE_Time_Value const kDest(value);

        ACE_Time_Value kAceTime;
        kAceTime = (ACE_Time_Value)(*this);
        kAceTime += kDest;

        *this = kAceTime;
        return *this;
    }

    DBTIMESTAMP_EX& DBTIMESTAMP_EX::AddMinutes(int const value)
    {
        return AddSeconds(value * 60);
    }

    DBTIMESTAMP_EX& DBTIMESTAMP_EX::AddHours(int const value)
    {
        return AddMinutes(value * 60);
    }

    DBTIMESTAMP_EX& DBTIMESTAMP_EX::AddDays(int const value)
    {
        return AddHours(value * 24);
    }

    bool DBTIMESTAMP_EX::IsCorrect() const
    {
        if ((!year) || (year >= Limit::YEAR))
        {
            return false;
        }
        if ((!month) || (month > Limit::MONTH))
        {
            return false;
        }
        if ((!day) || (day > Limit::DAY))
        {
            return false;
        }
        if (hour > Limit::HOUR)
        {
            return false;
        }
        if (minute > Limit::MIN)
        {
            return false;
        }
        if (second > Limit::SEC)
        {
            return false;
        }

        return true;
    }

    DBTIMESTAMP_EX::operator std::wstring const() const throw()
    {
        if (IsNull())
        {
            return L"NULL";
        }

        auto cFraction = fraction;
        if (cFraction > 999)
        {
            CL::vstring vFraction(fraction);
            vFraction.Get() = vFraction.Get().substr(0, 3);
            cFraction = vFraction;
        }

        TCHAR szTime[MAX_PATH] = {0,};
        _stprintf_s(szTime, MAX_PATH, L"%d-%02d-%02d %02d:%02d:%02d.%03d",
                    year, month, day,
                    hour, minute, second,
                    cFraction);

        std::wstring ret(szTime);
        /* 
            ret += vstring(year);   ret += L"-";
            ret += vstring(month);  ret += L"-";
            ret += vstring(day);    ret += L" ";
            ret += vstring(hour);   ret += L":";
            ret += vstring(minute); ret += L":";
            ret += vstring(second);    
        */
        return ret;
    }

    void DBTIMESTAMP_EX::ToStream(CL::Stream& kStream) const
    {
        kStream.Push(year);
        kStream.Push(month);
        kStream.Push(day);
        kStream.Push(hour);
        kStream.Push(minute);
        kStream.Push(second);
        kStream.Push(fraction);
    }

    bool DBTIMESTAMP_EX::FromStream(CL::Stream const& kStream)
    {
        kStream.Pop(year);
        kStream.Pop(month);
        kStream.Pop(day);
        kStream.Pop(hour);
        kStream.Pop(minute);
        kStream.Pop(second);
        kStream.Pop(fraction);
        return true;
    }

    USHORT DBTIMESTAMP_EX::GetYDay() const
    {
        USHORT yday = 0;
        MdayToYday(year, month, day, yday);
        return yday;
    }

    bool DBTIMESTAMP_EX::SetYDay(USHORT const yday)
    {
        return YdayToMday(year, yday, month, day);
    }

    int DBTIMESTAMP_EX::ConvertDate(DBTIMESTAMP_EX const& DateTime)
    {
        int Date = DateTime.year * 10000;
        Date += DateTime.month * 100;
        Date += DateTime.day;
        return Date;
    }

    DBTIMESTAMP_EX DBTIMESTAMP_EX::ConvertDate(int const Date)
    {
        CL::DBTIMESTAMP_EX DateTime;
        if (Date)
        {
            auto TmpDate = Date;
            DateTime.year = static_cast<SHORT>(TmpDate / 10000);
            TmpDate %= 10000;
            DateTime.month = static_cast<USHORT>(TmpDate / 100);
            TmpDate %= 100;
            DateTime.day = static_cast<USHORT>(TmpDate);
        }
        return DateTime;
    }

    int DBTIMESTAMP_EX::ConvertDateTime(DBTIMESTAMP_EX const& DateTime)
    {
        int Date = DateTime.year * 1000000;
        Date += DateTime.month * 10000;
        Date += DateTime.day * 100;
        Date += DateTime.hour;
        return Date;
    }

    DBTIMESTAMP_EX DBTIMESTAMP_EX::ConvertDateTime(int const Date)
    {
        CL::DBTIMESTAMP_EX DateTime;
        if (Date)
        {
            auto TmpDate = Date;
            DateTime.year = static_cast<SHORT>(TmpDate / 1000000);
            TmpDate %= 1000000;
            DateTime.month = static_cast<USHORT>(TmpDate / 10000);
            TmpDate %= 10000;
            DateTime.day = static_cast<USHORT>(TmpDate / 100);
            TmpDate %= 100;
            DateTime.hour = static_cast<USHORT>(TmpDate);
        }
        return DateTime;
    }

#define DATETIME_YEAR   10000000000
#define DATETIME_MONTH  100000000
#define DATETIME_DAY    1000000
#define DATETIME_HOUR   10000
#define DATETIME_MINUTE 100

    __int64 DBTIMESTAMP_EX::ConvertFromDateTime(DBTIMESTAMP_EX const& DateTime)
    {
        __int64 datetime = DateTime.year * DATETIME_YEAR;
        datetime += DateTime.month * DATETIME_MONTH;
        datetime += DateTime.day * DATETIME_DAY;
        datetime += DateTime.hour * DATETIME_HOUR;
        datetime += DateTime.minute * DATETIME_MINUTE;
        datetime += DateTime.second;
        return datetime;
    }

    DBTIMESTAMP_EX DBTIMESTAMP_EX::ConvertToDateTime(__int64 const DateTime)
    {
        CL::DBTIMESTAMP_EX Result;
        if (DateTime > 0)
        {
            auto TmpDate = DateTime;
            Result.year = static_cast<SHORT>(TmpDate / DATETIME_YEAR);
            TmpDate %= DATETIME_YEAR;
            Result.month = static_cast<USHORT>(TmpDate / DATETIME_MONTH);
            TmpDate %= DATETIME_MONTH;
            Result.day = static_cast<USHORT>(TmpDate / DATETIME_DAY);
            TmpDate %= DATETIME_DAY;
            Result.hour = static_cast<USHORT>(TmpDate / DATETIME_HOUR);
            TmpDate %= DATETIME_HOUR;
            Result.minute = static_cast<USHORT>(TmpDate / DATETIME_MINUTE);
            TmpDate %= DATETIME_MINUTE;
            Result.second = static_cast<USHORT>(TmpDate);
        }
        return Result;
    }

    int const MAX_MONTH = 12;

    void CalcCalander(INT16 const year, INT16 aiAccDays[MAX_MONTH + 1])
    {
        INT16 const aiMonthDays[MAX_MONTH] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (!(year % 4))
        {
            ++aiAccDays[1];
        }

        for (int idx = 0; _countof(aiMonthDays) > idx; ++idx)
        {
            auto& days_itor = aiMonthDays[idx];

            for (int acc_idx = idx; MAX_MONTH > acc_idx; ++acc_idx)
            {
                auto& acc_itor = aiAccDays[acc_idx + 1];
                acc_itor += days_itor;
            }
        }
    }

    bool YdayToMday(INT16 const year, USHORT const yday, USHORT& month, USHORT& day)
    {
        if (0 < yday
            && 366 >= yday)
        {
            INT16 aiAccDays[MAX_MONTH + 1] = {0,};
            CalcCalander(year, aiAccDays);

            for (int iIDX = 0; _countof(aiAccDays) > iIDX; ++iIDX)
            {
                if (yday > aiAccDays[iIDX])
                {
                    month = (INT16)(iIDX + 1);//0월이 없음
                    day = (INT16)(yday - aiAccDays[iIDX] + 1);//0일이 없음.
                }
            }
            return true;
        }
        return false;
    }

    bool MdayToYday(INT16 const year, USHORT const month, USHORT const day, USHORT& yday)
    {
        INT16 aiAccDays[MAX_MONTH + 1] = {0,};
        CalcCalander(year, aiAccDays);

        if (0 < month
            && MAX_MONTH >= month
            && 0 < day
            && 32 >= day)//1달 넘길 수 없음.
        {
            yday = aiAccDays[month - 1] + day;
            return true;
        }
        return false;
    }

USHORT DBTIMESTAMP_EX::MonthMaxDay()const
{
    INT16 const aiMonthDays[MAX_MONTH] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (0 < month
    && MAX_MONTH >= month)
    {
        int add = 0;
        if (!(year % 4) && (month == 2)) { add = 1; }
        return static_cast<USHORT>(aiMonthDays[month - 1] + add);
    }
    return 0;
}

void DBTIMESTAMP_EX::Set(std::string const& s, std::string const& format)
{
    struct tm tm;
    if (CL::strptime(s.c_str(), format.c_str(), &tm))
    {
        year = (SHORT)tm.tm_year;
        month = (USHORT)tm.tm_mon;
        day = (USHORT)tm.tm_mday;
        hour = (USHORT)tm.tm_hour;
        minute = (USHORT)tm.tm_min;
        second = (USHORT)tm.tm_sec;
    }
}

}//namespace CL


