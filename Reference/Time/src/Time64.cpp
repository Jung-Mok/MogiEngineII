#include "stdafx.h"
#include "CL/Time/Time64.h"

#include <Ace/TIme_Value.h>
#include <Ace/os_ns_sys_time.h>

#include "CL/Utility/Common.h"

namespace CL
{

    static ACE_Time_Value g_kBaseTime = ACE_OS::gettimeofday(); // ->��Ʈ���� �ذ�.(�����Ͻø� �����϶�)
    static ACE_Time_Value g_kStartTime = ACE_OS::gettimeofday(); // ->���ÿ��� �ð��� ��� �����Ѱ�??

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

        auto kRetTime = kAceTime.get_msec();//�и����� ������ ������.
        return kRetTime;
    }

    T_TIME32 GetTime32()
    { 
        auto const kAceTime = GetTime64_ACE_Org();
        return (T_TIME32)(kAceTime.get_msec() / 1000);//���� ������ ������.
    }

	T_TIME GetTimeUsec()
    { 
        ACE_Time_Value const kTime = GetTime64_ACE_Org();
		return kTime.usec();//����ũ�μ��� ������ ������.
    }

    bool TimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval)
    {
        auto const ui64NowTime = GetTime64();

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

    T_TIME DiffTime64(T_TIME const& i64Start, T_TIME const& i64End)
    {
        if( i64Start < i64End )
        {
            return static_cast<T_TIME>(i64End - i64Start);
        }
        
        if( i64Start > i64End )
        {//������ ũ�Ƿ�. ������ ��ȯ
            return static_cast<T_TIME>(i64Start-i64End)* -1;//����.
        }

        return 0;
    }

	D_TIME DeltaTime(T_TIME const& iTime)//100�� ���� 1�� �ؼ� 1�� ������ 1.xxx �� ������
	{
        return static_cast<D_TIME>(iTime) / static_cast<D_TIME>(CL::SECOND);
	}

	T_TIME RealTime(D_TIME const dTime)
	{
        return static_cast<CL::T_TIME>(dTime * (D_TIME)CL::SECOND);
	}
}