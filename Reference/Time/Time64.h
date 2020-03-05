#pragma once

#include "cl/Utility/Quid.h"

namespace CL
{

typedef float D_TIME;//밀리세컨이라도 아래 자리가 짤림.
typedef __int64 T_TIME;//딜레이 계산등을 위해 반드시 signed.
typedef UINT32 T_TIME32;

T_TIME const MILLISECOND = 1;
T_TIME const SECOND = MILLISECOND * 1000;
T_TIME const MINUTE = SECOND * 60;
T_TIME const HOUR   = MINUTE * 60;
T_TIME const DAY    = HOUR * 24;
T_TIME const YEAR   = DAY * 365;/* days since January 1 - [0,365] */

extern void SetStartTime(T_TIME const& kTime);
extern void SetBaseTime(T_TIME const& kBaseTime);

extern T_TIME32 GetTime32();//세컨 단위로 리턴함.
extern T_TIME GetTime64(QUID const& kGroupQuid = QUID::NullData());//밀리세컨 단위로 리턴함.

extern bool PauseTime64(QUID const& kGroupQuid);
extern bool ResumeTime64(QUID const& kGroupQuid);

extern T_TIME GetTimeUsec();//마이크로세컨 단위로 리턴함.
extern T_TIME DiffTime64(T_TIME const& ui64Start, T_TIME const& ui64End = GetTime64());
extern bool TimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval);

extern D_TIME DeltaTime(T_TIME const& iTime = GetTime64());//초단위로 변환. 1.xxx 초
extern T_TIME RealTime(D_TIME const dTime);//델타 타임을 밀리세컨 단위로 변환

//LocalTime
extern T_TIME32 GetLocalTime32();//밀리세컨 단위로 리턴함.
extern T_TIME GetLocalTime64(QUID const& kGroupQuid = QUID::NullData());//밀리세컨 단위로 리턴함.

extern bool PauseLocalTime64(QUID const& kGroupQuid);
extern bool ResumeLocalTime64(QUID const& kGroupQuid);

extern T_TIME GetLocalTimeUsec();//마이크로세컨 단위로 리턴함.
extern bool LocalTimeCheck64(T_TIME& ui64TImeValue, T_TIME const& ui64Interval);

}//namespace CL

#include "StopWatch.h"
