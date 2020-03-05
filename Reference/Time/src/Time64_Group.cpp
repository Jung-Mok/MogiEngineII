#include "stdafx.h"

#include <concurrent_unordered_map.h>
#include "CL/Time/Time64.h"

#include <Ace/TIme_Value.h>
#include <Ace/os_ns_sys_time.h>

#include "CL/Utility/Common.h"

namespace CL
{
    extern T_TIME GetTime64_Org();

    class TimeInfo
    {
    public:
        TimeInfo() {}
        ~TimeInfo() {}
    public:
        bool SetPause()
        {
            if (!m_kPauseTime)
            {
                m_kPauseTime = GetTime64_Org();
                return true;
            }

            core_log(L"Already Pause Time.");
            return false;
        };

        bool SetResume()
        {
            if (m_kPauseTime)
            {
                auto kNow = GetTime64_Org();
                auto kTimeGap = kNow - m_kPauseTime;
                m_kGapTime += kTimeGap;

                m_kPauseTime = 0;
                return true;
            }

            core_log(L"Already Resume Time.");
            return false;
        }

        CL::T_TIME AdjustTime(CL::T_TIME const& kTime)
        {
            if (m_kPauseTime)
            {//중지중이면 중지한 시간을.
                return m_kPauseTime - m_kGapTime;
            }
            else
            {
                return kTime - m_kGapTime;
            }

            return 0;
        };

    private:
        CL::T_TIME m_kPauseTime = 0;
        CL::T_TIME m_kGapTime = 0;
    };

    static concurrency::concurrent_unordered_map<CL::QUID, TimeInfo> s_kTimeInfo;

    T_TIME GetTime64(CL::QUID const& kGroupQuid)
    {
        auto kRetTime = GetTime64_Org();

        if (kGroupQuid.IsNotNull())
        {
            auto t_itor = s_kTimeInfo.find(kGroupQuid);
            if (s_kTimeInfo.end() != t_itor)
            {
                kRetTime = t_itor->second.AdjustTime(kRetTime);
            }
        }

        return kRetTime;
    }

    bool PauseTime64(CL::QUID const& kGroupQuid)
    {
        if (kGroupQuid.IsNotNull())
        {
            auto pair_ib = s_kTimeInfo.insert({ kGroupQuid, TimeInfo() });
            auto& kTI = pair_ib.first->second;
            return kTI.SetPause();
        }
        return false;
    }

    bool ResumeTime64(CL::QUID const& kGroupQuid)
    {
        if (kGroupQuid.IsNotNull())
        {
            auto t_itor = s_kTimeInfo.find(kGroupQuid);
            if (s_kTimeInfo.end() != t_itor)
            {
                return t_itor->second.SetResume();
            }
        }
        return false;
    }
}
