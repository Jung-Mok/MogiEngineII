#pragma once

#include "CL/TIME/DBTIMESTAMP_EX.h"

namespace CL
{

struct PackedTime_
{//비트 플래그라. -값 되면 영역 표현하는데 한계가 있다.
    UINT32 m_kYear  :6;    //0 이면 2000년
    UINT32 m_kMonth :4;
    UINT32 m_kDay   :5;
    UINT32 m_kHour  :5;
    UINT32 m_kMin   :6;
    UINT32 m_kSec   :6;

    struct Limit
    {
        enum Enum
        {
            YEAR        = 64    -1,
            MONTH       = 16    -1,
            DAY         = 32    -1,
            HOUR        = 32    -1,
            MIN         = 64    -1,
            SEC         = 64    -1,

            MIN_YEAR            = 2016,             
            MAX_YEAR            = MIN_YEAR + 63,    //MIN_YEAR + 63 년 까지 표현 가능
        };  
    };

    PackedTime_();
    explicit PackedTime_(DBTIMESTAMP_EX const& rhs);
    explicit PackedTime_(SYSTEMTIME const& rhs);
    void Clear();
    void SetLocalTime();
    bool IsNull() const;
    static PackedTime_ LocalTime();

    bool operator < (PackedTime_ const& rhs)const;
    bool operator == (PackedTime_ const& rhs)const;
    void operator = (DBTIMESTAMP const& rhs);
    void operator = (DBTIMESTAMP_EX const& rhs);
    void operator = (const struct tm& rhs);
    void operator = (SYSTEMTIME const& rhs);
    //+,- Operator 를 만들지 말것. 단위에 한계가 있다.
    operator DBTIMESTAMP() const;
    operator SYSTEMTIME() const;
    operator std::wstring() const;

    void SetTime(UINT32 const dwTime)
    {
        * (UINT32*)(this) = dwTime;
    }

    UINT32 GetTime() const
    {
        return* (UINT32*)(this);
    }

    void ToStream(CL::Stream& kStream)const;
    bool FromStream(CL::Stream const& kStream);
};

}//namespace CL

