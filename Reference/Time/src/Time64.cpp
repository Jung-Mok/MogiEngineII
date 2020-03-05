#include "stdafx.h"
#include "CL/Time/Time64.h"

#include <Ace/TIme_Value.h>
#include <Ace/os_ns_sys_time.h>

#include "CL/Utility/Common.h"

namespace CL
{

    static ACE_Time_Value g_kBaseTime = ACE_OS::gettimeofday(); // ->네트웍이 준값.(레이턴시를 포함하라)
    static ACE_Time_Value g_kStartTime = ACE_OS::gettimeofday(); // ->로컬에서 시간을 재기 시작한값??

    void SetStartTime(T_TIME const& kTime)
    {
        g_kStartTime = ACE_Time_Value(kTime / 1000, kTime % 1000);
    }

    void SetBaseTime(T_TIME const& kBaseTime)
    {
        g_kBaseTime = ACE_Time_Value(kBaseTime / 1000, kBaseTime % 1000);
    }

    ACE_Time_Value GetTime64_ACE_Org()
    {
        ACE_Time_Value const kGab = ACE_OS::gettimeofday() - g_kStartTime;
        return g_kBaseTime + kGab;
    }

    T_TIME GetTime64_Org()
    {
        auto const kAceTime = GetTime64_ACE_Org();

        auto kRetTime = kAceTime.get_msec();//밀리세컨 단위로 리턴함.
        return kRetTime;
    }

    T_TIME32 GetTime32()
    { 
        auto const kAceTime = GetTime64_ACE_Org();
        return (T_TIME32)(kAceTime.get_msec() / 1000);//세컨 단위로 리턴함.
    }

	T_TIME GetTimeUsec()
    { 
        ACE_Time_Value const kTime = GetTime64_ACE_Org();
		return kTime.usec();//마이크로세컨 단위로 리턴함.
    }

    bool TimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval)
    {
        auto const ui64NowTime = GetTime64();

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

    T_TIME DiffTime64(T_TIME const& i64Start, T_TIME const& i64End)
    {
        if( i64Start < i64End )
        {
            return static_cast<T_TIME>(i64End - i64Start);
        }
        
        if( i64Start > i64End )
        {//시작이 크므로. 음수로 전환
            return static_cast<T_TIME>(i64Start-i64End)* -1;//음수.
        }

        return 0;
    }

	D_TIME DeltaTime(T_TIME const& iTime)//100만 분의 1을 해서 1초 단위가 1.xxx 로 나오게
	{
        return static_cast<D_TIME>(iTime) / static_cast<D_TIME>(CL::SECOND);
	}

	T_TIME RealTime(D_TIME const dTime)
	{
        return static_cast<CL::T_TIME>(dTime * (D_TIME)CL::SECOND);
	}
}