#include "stdafx.h"
#include "CL/time/Stopwatch.h"

namespace CL
{

StopWatch::StopWatch(CL::QUID const& kTimeGrpQuid)
    :m_kTimeGrpQuid(kTimeGrpQuid)
{
}

void StopWatch::Start()
{
    if (!m_kStartTime)
    {//두번 시작 못하게.
        m_kStartTime = GetTime64(m_kTimeGrpQuid);
        m_kDuration = 0;
    }

    if (m_kPauseStartTime)
    {
        Pause();
    }
}

void StopWatch::Pause()
{
    if (m_kStartTime)
    {//시작 한 경우만
        if (!m_bIsPause)
        {
            m_kPauseStartTime = GetTime64(m_kTimeGrpQuid);
        }
        else
        {
            auto const iPauseEndTime = GetTime64(m_kTimeGrpQuid);
            m_kStartTime += (iPauseEndTime - m_kPauseStartTime);
        }

        m_bIsPause = !m_bIsPause;
    }
}

void StopWatch::Rap(std::wstring const& Tag)
{
    if (m_kDuration == 0)
    {
        auto const iEndTime = GetTime64(m_kTimeGrpQuid);
        m_ContRap.push_back({ Tag, iEndTime - m_kStartTime });
    }
}

void StopWatch::End()
{
    auto const iEndTime = GetTime64(m_kTimeGrpQuid);
    m_kDuration = iEndTime - m_kStartTime;
}

T_TIME StopWatch::GetDuration()const
{
    return m_kDuration;
}


}//namespace CL
