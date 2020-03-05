#include "stdafx.h"
#include "CL/Time/TimeChecker_Impl.h"

namespace CL
{

CL::T_TIME TimeChecker_Impl::GetSpendTime()const
{
    auto kNow = CL::GetTime64();
    return kNow - m_kStartTime;
}
}
