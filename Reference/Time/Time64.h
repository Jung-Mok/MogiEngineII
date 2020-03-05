#pragma once

#include "cl/Utility/Quid.h"

namespace CL
{

typedef float D_TIME;//�и������̶� �Ʒ� �ڸ��� ©��.
typedef __int64 T_TIME;//������ ������ ���� �ݵ�� signed.
typedef UINT32 T_TIME32;

T_TIME const MILLISECOND = 1;
T_TIME const SECOND = MILLISECOND * 1000;
T_TIME const MINUTE = SECOND * 60;
T_TIME const HOUR   = MINUTE * 60;
T_TIME const DAY    = HOUR * 24;
T_TIME const YEAR   = DAY * 365;/* days since January 1 - [0,365] */

extern void SetStartTime(T_TIME const& kTime);
extern void SetBaseTime(T_TIME const& kBaseTime);

extern T_TIME32 GetTime32();//���� ������ ������.
extern T_TIME GetTime64(QUID const& kGroupQuid = QUID::NullData());//�и����� ������ ������.

extern bool PauseTime64(QUID const& kGroupQuid);
extern bool ResumeTime64(QUID const& kGroupQuid);

extern T_TIME GetTimeUsec();//����ũ�μ��� ������ ������.
extern T_TIME DiffTime64(T_TIME const& ui64Start, T_TIME const& ui64End = GetTime64());
extern bool TimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval);

extern D_TIME DeltaTime(T_TIME const& iTime = GetTime64());//�ʴ����� ��ȯ. 1.xxx ��
extern T_TIME RealTime(D_TIME const dTime);//��Ÿ Ÿ���� �и����� ������ ��ȯ

//LocalTime
extern T_TIME32 GetLocalTime32();//�и����� ������ ������.
extern T_TIME GetLocalTime64(QUID const& kGroupQuid = QUID::NullData());//�и����� ������ ������.

extern bool PauseLocalTime64(QUID const& kGroupQuid);
extern bool ResumeLocalTime64(QUID const& kGroupQuid);

extern T_TIME GetLocalTimeUsec();//����ũ�μ��� ������ ������.
extern bool LocalTimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval);

}//namespace CL

#include "StopWatch.h"
