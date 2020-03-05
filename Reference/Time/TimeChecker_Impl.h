#pragma once 

#include "CL/Message/ITimeChecker.h"

namespace CL
{
    class TimeChecker_Impl
        : public ITimeChecker
    {
    public:
        TimeChecker_Impl() = default;
        virtual ~TimeChecker_Impl() = default;

    public:
        virtual CL::T_TIME GetSpendTime()const;
    
    private:
        CL::T_TIME const m_kStartTime = CL::GetTime64();
    };
}