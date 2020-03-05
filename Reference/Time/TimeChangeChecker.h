#pragma once

#include <algorithm>
#include "CL/Time/Time64.h"

namespace CL
{
    class TimeChangeChecker
    {
    public:
        TimeChangeChecker() {}
        ~TimeChangeChecker() {}

    public:
        void OnTick()
        {
            ClearState();

            auto const kNow = CL::DBTIMESTAMP_EX::Now();
            
            m_bIsChange_year = (kNow.year != m_kPrevCheckDay.year);
            m_bIsChange_month = (kNow.month != m_kPrevCheckDay.month);
            m_bIsChange_day = (kNow.day != m_kPrevCheckDay.day);
            m_bIsChange_hour = (kNow.hour != m_kPrevCheckDay.hour);
            m_bIsChange_minute = (kNow.minute != m_kPrevCheckDay.minute);
            m_bIsChange_second = (kNow.second != m_kPrevCheckDay.second);

            m_bIsChange_week_day = (kNow.DayOfWeek() != m_kPrevCheckDay.DayOfWeek());

            m_kPrevCheckDay = kNow;
        }

        CL::DBTIMESTAMP_EX const& Now()const { return m_kPrevCheckDay; }

        bool IsYearChange()const { return m_bIsChange_year; }
        bool IsMonthChange()const { return m_bIsChange_month; }
        bool IsDayChange()const { return m_bIsChange_day; }
        bool IsHourChange()const { return m_bIsChange_hour; }
        bool IsMinuteChange()const { return m_bIsChange_minute; }
        bool IsSecondChange()const { return m_bIsChange_second; }
        bool IsWeekDayChange()const { return m_bIsChange_week_day; }

        bool IsWeekDay(eDayOfWeek::Enum const eDay)const 
        {
            if (IsWeekDayChange())
            {
                auto const kNowWeekDay = CL::DBTIMESTAMP_EX::Now().DayOfWeek();
                if (kNowWeekDay == eDay)
                {
                    return true;
                }
            }
            return false;
        }

    private:
        void ClearState()
        {
            m_bIsChange_year = false;
            m_bIsChange_month = false;
            m_bIsChange_day = false;
            m_bIsChange_hour = false;
            m_bIsChange_minute = false;
            m_bIsChange_second = false;
            m_bIsChange_week_day = false;
        }

    private:
        bool m_bIsChange_year = false;
        bool m_bIsChange_month = false;
        bool m_bIsChange_day = false;
        bool m_bIsChange_hour = false;
        bool m_bIsChange_minute = false;
        bool m_bIsChange_second = false;
        bool m_bIsChange_week_day = false;

        CL::DBTIMESTAMP_EX m_kPrevCheckDay = CL::DBTIMESTAMP_EX::Now();
    };

}//namespace CL
