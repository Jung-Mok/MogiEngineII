#pragma once

namespace CL
{
    struct eDayOfWeek
    {
        enum Enum
        {
            NONE = 0,
            SUN = 0, //일
            MON = 1, //월
            TUE = 2, //화
            WED = 3, //수
            THU = 4, //목
            FRI = 5, //금
            SAT = 6, //토
            MAX,
        };

        static int ConvToNo(std::wstring kFindStr)
        {
            CL::vstring::Trim(kFindStr);

            CheckStrToEnum(SUN);
            CheckStrToEnum(MON);
            CheckStrToEnum(TUE);
            CheckStrToEnum(WED);
            CheckStrToEnum(THU);
            CheckStrToEnum(FRI);
            CheckStrToEnum(SAT);

            return NONE;
        }

        static std::wstring ConvToStr(int const e)
        {
            switch (e)
            {
                CaseEnumToStr(SUN);
                CaseEnumToStr(MON);
                CaseEnumToStr(TUE);
                CaseEnumToStr(WED);
                CaseEnumToStr(THU);
                CaseEnumToStr(FRI);
                CaseEnumToStr(SAT);
            default:
                {
                    return L"NONE";
                }
                break;
            }
        }
    };

    extern bool YdayToMday(INT16 const year, USHORT const yday, USHORT& month, USHORT& day);
    extern bool MdayToYday(INT16 const year, USHORT const month, USHORT const day, USHORT& yday);

    //시간관련 작업은 DBTIMESTAMP 의 확장형이기 때문에 네이밍 예외를 둔다

    struct DBTIMESTAMP_EX
    {//계산 때문에 음수가 필요함.
        SHORT  year = 0;
        USHORT month = 0;
        USHORT day = 0;
        USHORT hour = 0;
        USHORT minute = 0;
        USHORT second = 0;
        ULONG fraction = 0;

        struct Limit
        {
            enum Enum
            {
                YEAR = 9999,
                MONTH = 12,
                DAY = 31,
                HOUR = 23,
                MIN = 59,
                SEC = 59,
            };
        };

        DBTIMESTAMP_EX() = default;
        explicit DBTIMESTAMP_EX(::DBTIMESTAMP const& rhs);
        explicit DBTIMESTAMP_EX(SYSTEMTIME const& rhs);
        explicit DBTIMESTAMP_EX(PackedTime_ const& rhs);
        explicit DBTIMESTAMP_EX(FILETIME const& rhs);
        explicit DBTIMESTAMP_EX(ACE_Time_Value const& rhs);
        ~DBTIMESTAMP_EX() = default;//상속 해서 쓰지 마세요.

        void Clear();

        operator DBTIMESTAMP const() const throw();
        operator ACE_Time_Value() const throw();
        operator std::wstring const() const throw();
        operator SYSTEMTIME const() const throw();
        operator FILETIME const() const throw();
        operator T_TIME const() const throw();

        void operator =(DBTIMESTAMP const& rhs);
        bool operator !() const;
        bool operator ==(DBTIMESTAMP_EX const& rhs) const;
        bool operator !=(DBTIMESTAMP_EX const& rhs) const;

        DBTIMESTAMP_EX const operator -(DBTIMESTAMP_EX const& rhs) const;
        DBTIMESTAMP_EX const operator +(DBTIMESTAMP_EX const& rhs) const;

        void operator =(PackedTime_ const& rhs);
        void operator =(SYSTEMTIME const& rhs);
        void operator =(ACE_Time_Value const& rhs);
        void operator =(FILETIME const& rhs);

        bool operator <(DBTIMESTAMP_EX const& rhs) const;
        bool operator >(DBTIMESTAMP_EX const& rhs) const;
        bool operator <=(DBTIMESTAMP_EX const& rhs) const;
        bool operator >=(DBTIMESTAMP_EX const& rhs) const;

        void Swap(DBTIMESTAMP_EX& rhs);

        void SetUtcTime();

		static DBTIMESTAMP_EX Now();
		static DBTIMESTAMP_EX Now(T_TIME& time);
		static DBTIMESTAMP_EX Now(T_TIME32& time32);
        static DBTIMESTAMP_EX const& NullData();

        DBTIMESTAMP_EX ToLocalTime() const;
        DBTIMESTAMP_EX ToUniversalTime() const;

        T_TIME32 ToTime32() const;

        eDayOfWeek::Enum DayOfWeek() const;
        int DayOfYear() const;
        int WeekOfYear(int const iso = 1) const;

        //fraction 이 정확하지 않아 밀리세컨은 지원 하지 않음./Month, Year는 기준이 각각 달라 지원 안함.
        DBTIMESTAMP_EX& AddMilliseconds(int const value);
        DBTIMESTAMP_EX& AddSeconds(__int64 const value);
        DBTIMESTAMP_EX& AddMinutes(int const value);
        DBTIMESTAMP_EX& AddHours(int const value);
        DBTIMESTAMP_EX& AddDays(int const value);

        bool IsNull() const;
        bool IsNotNull() const;
        bool IsCorrect() const;

        void ToStream(CL::Stream& kStream) const;
        bool FromStream(CL::Stream const& kStream);

        USHORT GetYDay() const;
        bool SetYDay(USHORT const yday);

        USHORT MonthMaxDay()const;

        void Set(std::string const& day, std::string const& format = "%Y-%m-%d %H:%M:%S");

        static __int64 ConvertFromDateTime(DBTIMESTAMP_EX const& DateTime); // DBTIMESTAMP_EX -> int64    예) 2015-12-31 12:14 -> 20151231
        static DBTIMESTAMP_EX ConvertToDateTime(__int64 const DateTime);   // int64 -> DBTIMESTAMP_EX    예) 20151231 -> 2015-12-31 00:00

        static int ConvertDate(DBTIMESTAMP_EX const& DateTime); //DBTIMESTAMP_EX -> int    예) 2015-12-31 12:14 -> 20151231
        static DBTIMESTAMP_EX ConvertDate(int const Date); //int -> DBTIMESTAMP_EX    예) 20151231 -> 2015-12-31 00:00

        static int ConvertDateTime(DBTIMESTAMP_EX const& DateTime); //DBTIMESTAMP_EX -> int    예) 2015-12-31 12:14 -> 2015123112
        static DBTIMESTAMP_EX ConvertDateTime(int const Date); //int -> DBTIMESTAMP_EX    예) 2015123112 -> 2015-12-31 12:00

        static ACE_Time_Value s_kBaseTime;
        static ACE_Time_Value s_kStartTime;
    };
}//namespace CL


