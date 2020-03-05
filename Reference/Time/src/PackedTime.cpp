#include "stdafx.h"
#include "CL/Time/PackedTime.h"

#include "CL/IO/vstring.h"

namespace CL
{

PackedTime_::PackedTime_()
{
    Clear();
}

PackedTime_::PackedTime_(DBTIMESTAMP_EX const& rhs)
{
    *this = rhs;
}

PackedTime_::PackedTime_(SYSTEMTIME const& rhs)
{
    *this = rhs;
}

void PackedTime_::Clear()
{
    m_kYear = 0;
    m_kMonth = 0;
    m_kDay = 0;
    m_kHour = 0;
    m_kMin = 0;
    m_kSec = 0;
}

void PackedTime_::SetLocalTime()
{
    SYSTEMTIME kTime;
    ::GetLocalTime(&kTime);

    (*this) = kTime;
}

PackedTime_ PackedTime_::LocalTime()
{
    PackedTime_ kTime;
    kTime.SetLocalTime();
    return kTime;
}

bool PackedTime_::operator < (PackedTime_ const& rhs)const
{
    if( m_kYear < rhs.m_kYear ){return true;}
    if( m_kYear > rhs.m_kYear ){return false;}

    if( m_kMonth < rhs.m_kMonth ){return true;}
    if( m_kMonth > rhs.m_kMonth ){return false;}
    
    if( m_kDay < rhs.m_kDay ){return true;}
    if( m_kDay > rhs.m_kDay ){return false;}

    if( m_kHour < rhs.m_kHour ){return true;}
    if( m_kHour > rhs.m_kHour ){return false;}

    if( m_kMin < rhs.m_kMin ){return true;}
    if( m_kMin > rhs.m_kMin ){return false;}

    if( m_kSec < rhs.m_kSec ){return true;}
    if( m_kSec > rhs.m_kSec ){return false;}

    return false;
}

void PackedTime_::operator = (DBTIMESTAMP const& rhs)
{
    UINT32 year = rhs.year;
    year = std::max<UINT32>(year, Limit::MIN_YEAR);
    year = std::min<UINT32>(year, Limit::MAX_YEAR);

    m_kYear = ( year - Limit::MIN_YEAR );
    m_kMonth = rhs.month;
    m_kDay = rhs.day;
    m_kHour = rhs.hour;
    m_kMin = rhs.minute;
    m_kSec = rhs.second;
}

void PackedTime_::operator = (DBTIMESTAMP_EX const& rhs)
{
   * this = (DBTIMESTAMP)rhs;
}

void PackedTime_::operator = (const struct tm& rhs)
{
    UINT32 year = rhs.tm_year + 1900;
    year = std::max<UINT32>(year, Limit::MIN_YEAR);
    year = std::min<UINT32>(year, Limit::MAX_YEAR);

    m_kYear = ( year - Limit::MIN_YEAR );
    m_kMonth = rhs.tm_mon + 1;
    m_kDay = rhs.tm_mday;
    m_kHour = rhs.tm_hour;
    m_kMin = rhs.tm_min;
    m_kSec = rhs.tm_sec;
}

void PackedTime_::operator = (SYSTEMTIME const& rhs)
{
    UINT32 year = static_cast<UINT32>(rhs.wYear);
    year = std::max<UINT32>(year, Limit::MIN_YEAR);
    year = std::min<UINT32>(year, Limit::MAX_YEAR);

    m_kYear = (year - Limit::MIN_YEAR);
    m_kMonth= rhs.wMonth;
    m_kDay  = rhs.wDay;
    m_kHour = rhs.wHour;
    m_kMin  = rhs.wMinute;
    m_kSec  = rhs.wSecond;
}

bool PackedTime_::operator == (PackedTime_ const& rhs)const
{
    return 0 == ::memcmp(this, &rhs, sizeof(PackedTime_));
}

PackedTime_::operator DBTIMESTAMP() const
{
    DBTIMESTAMP kTime;
    
    kTime.year = static_cast<INT16>(m_kYear + Limit::MIN_YEAR);
    kTime.month = static_cast<UINT16>(m_kMonth);
    kTime.day = static_cast<UINT16>(m_kDay);
    kTime.hour = static_cast<UINT16>(m_kHour);
    kTime.minute = static_cast<UINT16>(m_kMin);
    kTime.second = static_cast<UINT16>(m_kSec);
    kTime.fraction = 0;
    return kTime;
}

PackedTime_::operator SYSTEMTIME() const
{
    SYSTEMTIME Time;
    
    Time.wYear = static_cast<UINT16>(m_kYear + Limit::MIN_YEAR);
    Time.wMonth = static_cast<UINT16>(m_kMonth);
    Time.wDay = static_cast<UINT16>(m_kDay);
    Time.wHour = static_cast<UINT16>(m_kHour);
    Time.wMinute = static_cast<UINT16>(m_kMin);
    Time.wSecond = static_cast<UINT16>(m_kSec);
    Time.wMilliseconds = 0;
    return Time;
}

PackedTime_::operator std::wstring()const
{
    if( IsNull() )
    {
        return L"NULL";
    }

    vstring ret;
	ret += vstring(m_kYear + Limit::MIN_YEAR);    ret += L"-";
    ret += vstring(m_kMonth);   ret += L"-";
    ret += vstring(m_kDay);     ret += L" ";
    ret += vstring(m_kHour);    ret += L":";
    ret += vstring(m_kMin);     ret += L":";
    ret += vstring(m_kSec);    
    return static_cast< std::wstring const>(ret);
}

bool PackedTime_::IsNull() const
{
    return ( m_kMonth==0 || m_kDay==0 );
}

void PackedTime_::ToStream(CL::Stream& kStream)const
{
    auto kTime = GetTime();
    kStream.Push(kTime);
}

bool PackedTime_::FromStream(CL::Stream const& kStream)
{
    UINT32 kTime = 0;
    if (kStream.Pop(kTime))
    {
        SetTime(kTime);
        return true;
    }
    return false;
}

}//namespace CL