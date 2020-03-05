#pragma once

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif //#ifndef WIN32_LEAN_AND_MEAN

#include <string>
#include <list>

#include <oledb.h>

#include "CL/Utility/Quid.h"
#include "CL/Math/point2.h"
#include "CL/Math/point3.h"

#include "CL/IO/vstringReplace.h"

namespace CL
{

struct DBTIMESTAMP_EX;
struct PackedTime_;

class vstring
{//std::string을 절대 상속 받아 쓰지말것. 
public:
    vstring() = default;

    template< typename T >
    explicit vstring(T const& rhs);
	
    template< typename T >
    explicit vstring(T const* const& rhs);

    template<typename ... Args>
    explicit vstring(std::string const& format, Args ... args);

    virtual ~vstring() = default;

public://변환
    operator INT8 const()           const    throw();
    operator UINT8 const()          const    throw();
    operator INT16 const()          const    throw();
    operator UINT16 const()         const    throw();
    operator INT32 const()          const    throw();
    operator UINT32 const()         const    throw();
    operator long const()           const    throw();
    operator unsigned long const()  const    throw();
    operator INT64 const()          const    throw();
    operator UINT64 const()         const    throw();

    operator float const()          const    throw();
    operator double const()         const    throw();
    operator bool const()           const    throw();
    operator wchar_t const*()       const    throw();
    operator vstring const&()       const    throw();
    operator std::string const()    const    throw();
    operator std::wstring const()  const    throw();
	operator std::wstring const&()  const    throw();
	operator CL::GUID	const()  const    throw();
    operator CL::QUID	const()  const    throw();
    operator CL::DBTIMESTAMP_EX	const()  const    throw();
    
	template< typename T >
	operator ::Point2<T>	const()  const    throw();

	template< typename T >
	operator ::Point3<T>	const()  const    throw();

public://연산자
    vstring& operator = (char const& rhs);
    vstring& operator = (wchar_t const& rhs);
    vstring& operator = (short const& rhs);
    vstring& operator = (int const& rhs);
    vstring& operator = (long const& rhs);
    vstring& operator = (INT64 const& rhs);
    vstring& operator = (UINT8 const& rhs);
    vstring& operator = (unsigned short const& rhs);
    vstring& operator = (unsigned int const& rhs);
    vstring& operator = (unsigned long const& rhs);
    vstring& operator = (UINT64 const& rhs);
    vstring& operator = (float const& rhs);
    vstring& operator = (double const& rhs);
    vstring& operator = (bool const& rhs);

    vstring& operator = (void const* const& rhs);
    vstring& operator = (char const* const& rhs);
    vstring& operator = (wchar_t const* const& rhs);

    vstring& operator = (std::string const& rhs);
    vstring& operator = (std::wstring const& rhs);

    vstring& operator = (QUID const& rhs);
    vstring& operator = (::GUID const& rhs);
    vstring& operator = (::DBTIMESTAMP const& rhs);
    vstring& operator = (GUID const& rhs);
    vstring& operator = (DBTIMESTAMP_EX const& rhs);
    vstring& operator = (PackedTime_ const& rhs);
    vstring& operator = (vstring const& rhs);

	template< typename T >
	vstring& operator = (::Point2<T> const& rhs);

	template< typename T >
    vstring& operator = (::Point3<T> const& rhs);

    template< typename T1 >
    vstring& operator += (T1 const& rhs);

    template< typename T1 >
    vstring& operator += (T1 const* const& rhs);

    template< >
    vstring& operator += (vstring const& rhs);

    template< >
    vstring& operator += (char const* const& rhs);

    template< >
    vstring& operator += (wchar_t const* const& rhs);

    template<typename T>
    vstring& operator <<(T const& rhs);

    template< >
    vstring& operator <<(vstringReplace const& kReplaceStr);

    template<typename T>
    vstring& operator <<(T const* const& rhs);

    template< typename T1 >
    vstring operator + (T1 const& rhs)const;

    template< typename T1 >
    vstring operator + (T1 const* const& rhs)const;

    bool operator == (vstring const& vstrData)const;
    bool operator == (std::string const& strData)const;
    bool operator == (std::wstring const& strData)const;

    bool operator < (vstring const& rhs)const;

    void clear();
    size_t size() const;
    size_t length() const;
    bool IsEmpty()const;

    std::wstring const& Get()const;
    std::wstring& Get();

public://함수
    static wchar_t ConvToUNI(char const src);
    static errno_t ConvToUNI(std::string const& strSrc, std::wstring& wstrTgt);
    static errno_t ConvToMB(std::wstring const& wstrSrc, std::string& strTgt);

	static std::string ConvToUTF8FromAnsi(std::string const& strSrc);
    // 유니코드 시스템에서 UTF8을 사용하는 라이브러리에 문자열을 전달할때 아래를 참고.
    // 유니코드 시스템 -> UTF8 라이브러리 : ConvToUTF8FromUNI 사용.
    // UTF8 라이브러리 -> 유니코드 시스템 : ConvToUNIFromUTF8 사용.
    static std::string ConvToUTF8FromUNI(std::wstring const& strSrc);
    static std::wstring ConvToUNIFromUTF8(std::string const& strSrc);

    static std::wstring ConvToUNI(std::string const& strSrc);
    static std::string ConvToMB(std::wstring const& wstrSrc);
    static std::string ConvToUTF8(std::string const& strSrc);
	
	static std::string Base64Encode(std::string const& strSrc);
	static std::string Base64Decode(std::string const& strSrc);

    template<typename T_STRING >
    static T_STRING CropTextByKey(T_STRING const& strSrc, typename T_STRING::value_type const& cKey, INT32 const& iAddChar);

    template<typename T_STRING>
    static void ConvToUPR(T_STRING& wstrSrc);

    template<typename T_STRING>
    static void ConvToLWR(T_STRING& strSrc);

    static std::wstring ConvTo16Bit(DWORD const dwIn);
    static std::wstring ConvToHex(DWORD const dwIn);
    static std::wstring ConvToHex(char const* pData, size_t const iArrayCount, size_t const iLimitArray = 1000, bool const bAddHeader=true);
    static std::wstring ConvToHex(BYTE const* pData, size_t const iArrayCount, size_t const iLimitArray = 1000, bool const bAddHeader=true);
    static INT64 ConvFromHex(std::wstring const& str);

    template<typename T_STRING, typename T_CONT >//= std::list<T_STRING>
    void CutTextByKey(T_STRING const& kKey, T_CONT& rOut, bool const bEmptyLineUse = false);

    template<typename T_STRING, typename T_CONT  >//= std::list<T_STRING>
    static void CutTextByKey(T_STRING const& kOrg, T_STRING const& kKey, T_CONT& rOut, bool const bEmptyLineUse = false);

    template<typename T_STRING, typename T_CONT  >//= std::list<T_STRING>
    static void CutTextByKey(T_STRING const& kOrg, T_STRING const& kKey, T_CONT& rOut, T_STRING const& BundleS, T_STRING const& BundleE, bool const bEmptyLineUse = false);

    // member
    int icmp(vstring const& vstrData) const;
    int icmp(std::wstring const& strData) const;
    static int icmp(std::wstring const& lhs, std::wstring const& rhs);
    static int icmp(std::string const& lhs, std::string const& rhs);

    int cmp(vstring const& vstrData) const;
    int cmp(std::wstring const& strData) const;
    static int cmp(std::wstring const& lhs, std::wstring const& rhs);
    static int cmp(std::string const& lhs, std::string const& rhs);

    template< typename T_STRING, typename T_REPLACE >
    void Replace(T_STRING const& kFindStr, T_REPLACE const& kReplaceData );

    template< typename T_STRING, typename T_REPLACE >
    static void Replace(T_STRING& kOrg, T_STRING const& kFindStr, T_REPLACE const& kReplaceData );

    template< typename T_STRING >
    static void Trim(T_STRING& kOrg);
    
    template< >
    static void Trim(std::string& kOrg);

    template< >
    static void Trim(std::wstring& kOrg);

    void Trim();

private:
    std::wstring m_wstr;
};

}//namespace CL

#include "vstring.inl"


template<>
struct std::hash< CL::vstring >
{
    size_t operator()(CL::vstring const& rhs)const
    {
        return std::hash<std::wstring>()(rhs.Get());
    }
};