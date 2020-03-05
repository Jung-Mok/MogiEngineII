#pragma once

#include <algorithm>
#include "CL/Time/Time64.h"

namespace CL
{

class StopWatch
{
public:
    using CONT_RAP = std::list<std::pair<std::wstring, T_TIME>>;

    StopWatch(CL::QUID const& kTimeGrpQuid = CL::QUID::NullData());
    ~StopWatch() = default;

public:
    void Start();
    void Pause();
    void Rap(std::wstring const& Tag = L"");
    void End();
    T_TIME GetDuration()const;
    CONT_RAP const& ContRap()const { return m_ContRap; }

protected:
    bool m_bIsPause = false;
    T_TIME m_kStartTime = 0;
    T_TIME m_kDuration = 0;
    T_TIME m_kPauseStartTime = 0;
    CONT_RAP m_ContRap;

    CL::QUID const m_kTimeGrpQuid;
};

class DurationWatch
{
public:
    DurationWatch(CL::QUID const& kTimeGrpQuid = CL::QUID::NullData()) 
        : m_kTimeGrpQuid(kTimeGrpQuid)
    {};
    ~DurationWatch() {};

public:
    void Start(CL::T_TIME const kDurTime)
    {
        //        report_log("Ω√¿€:" << m_kDurationTime);

        auto const kNow = CL::GetTime64(m_kTimeGrpQuid);
        m_kStartTime = kNow;
        m_kPrevCheckTime = kNow;
        m_kDurationTime = kDurTime;
    }

    void Pause()
    {
        //        report_log("∏ÿ√„:");
        m_bIsPause = true;
        //        CheckOccured();
    }

    void Resume()
    {
        //        report_log("∞Ëº”:");

        m_bIsPause = false;
        m_kPrevCheckTime = CL::GetTime64(m_kTimeGrpQuid);
    }

	CL::D_TIME GetRemain() const
	{
		return m_kDurationTime * 0.001f;
	}

    bool CheckOccured()
    {
        if (!m_bIsPause)
        {
            auto const kNow = CL::GetTime64(m_kTimeGrpQuid);
            auto const kPassTime = kNow - m_kPrevCheckTime;

            m_kDurationTime = std::max<__int64>(m_kDurationTime - kPassTime, 0);
//            report_log("≥≤¿∫Ω√∞£:" << m_kDurationTime);
        }

        m_kPrevCheckTime = CL::GetTime64(m_kTimeGrpQuid);

        if (!m_kDurationTime)
        {
            return true;
        }
        return false;
    }

protected:
    bool m_bIsPause = false;

    CL::T_TIME m_kStartTime = 0;
    CL::T_TIME m_kPrevCheckTime = 0;
    CL::T_TIME m_kDurationTime = 0;

    CL::QUID const m_kTimeGrpQuid;
};

}//namespace CL
