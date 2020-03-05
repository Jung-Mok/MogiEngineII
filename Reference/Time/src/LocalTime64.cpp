#include "stdafx.h"
#include "CL/Time/Time64.h"

#include <Ace/TIme_Value.h>
#include <Ace/os_ns_sys_time.h>

#include "CL/Utility/Common.h"

namespace CL
{

    static ACE_Time_Value g_kStartTime = ACE_OS::gettimeofday(); // ->로컬에서 시간을 재기 시작한값??

    ACE_Time_Value GetLocalTime64_ACE_Org()
    {
        ACE_Time_Value const kGab = ACE_OS::gettimeofday() - g_kStartTime;
        return kGab;
    }

    T_TIME GetLocalTime64_Org()
    {
        auto const kAceTime = GetLocalTime64_ACE_Org();
        return kAceTime.get_msec();//밀리세컨 단위로 리턴함.
    }

    T_TIME32 GetLocalTime32()
    { 
        auto const kAceTime = GetLocalTime64_ACE_Org();
        return (T_TIME32)kAceTime.get_msec();//밀리세컨 단위로 리턴함.
    }

	T_TIME GetLocalTimeUsec()
    { 
        auto const kTime = GetLocalTime64_ACE_Org();
		return kTime.usec();//마이크로세컨 단위로 리턴함.
    }

    bool LocalTimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval)
    {
        auto const ui64NowTime = GetLocalTime64();

        if (!ui64TImeValue)//최초 콜.
        {
            ui64TImeValue = ui64NowTime;
            return true;
        }
        
        if( (ui64NowTime - ui64TImeValue ) < ui64Interval)
        {//필요 시간을 충족치 못했다.
            return false;
        }
        ui64TImeValue = ui64NowTime;
        return true;
    }

}