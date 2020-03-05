#include "stdafx.h"
#include "CL/Time/Time64.h"

#include <Ace/TIme_Value.h>
#include <Ace/os_ns_sys_time.h>

#include "CL/Utility/Common.h"

namespace CL
{

    static ACE_Time_Value g_kStartTime = ACE_OS::gettimeofday(); // ->���ÿ��� �ð��� ��� �����Ѱ�??

    ACE_Time_Value GetLocalTime64_ACE_Org()
    {
        ACE_Time_Value const kGab = ACE_OS::gettimeofday() - g_kStartTime;
        return kGab;
    }

    T_TIME GetLocalTime64_Org()
    {
        auto const kAceTime = GetLocalTime64_ACE_Org();
        return kAceTime.get_msec();//�и����� ������ ������.
    }

    T_TIME32 GetLocalTime32()
    { 
        auto const kAceTime = GetLocalTime64_ACE_Org();
        return (T_TIME32)kAceTime.get_msec();//�и����� ������ ������.
    }

	T_TIME GetLocalTimeUsec()
    { 
        auto const kTime = GetLocalTime64_ACE_Org();
		return kTime.usec();//����ũ�μ��� ������ ������.
    }

    bool LocalTimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval)
    {
        auto const ui64NowTime = GetLocalTime64();

        if (!ui64TImeValue)//���� ��.
        {
            ui64TImeValue = ui64NowTime;
            return true;
        }
        
        if( (ui64NowTime - ui64TImeValue ) < ui64Interval)
        {//�ʿ� �ð��� ����ġ ���ߴ�.
            return false;
        }
        ui64TImeValue = ui64NowTime;
        return true;
    }

}