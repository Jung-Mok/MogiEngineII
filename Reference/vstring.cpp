#include "stdafx.h"
#include "CL/IO/vstring.h"

#include <errno.h>
#include <tchar.h>
#include <sstream>

#include "CL/Utility/guid.h"
#include "CL/Time/PackedTime.h"
#include "CL/TIME/DBTIMESTAMP_EX.h"

namespace CL
{
	// base64 참고 : http://www.adp-gmbh.ch/cpp/common/base64.html
	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	static inline bool is_base64(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

//////////////////////////////////////////////////////////////////////////////////////////
// CASTING
vstring::operator INT8 const() const throw()
{
    return static_cast<INT8>(this->operator INT32 const());
}

vstring::operator UINT8 const() const throw()
{
    return static_cast<UINT8>(this->operator UINT32 const());
}

vstring::operator INT16 const() const throw()
{
    return static_cast<INT16>(this->operator INT32 const());
}

vstring::operator UINT16 const() const throw()
{
    return static_cast<UINT16>(this->operator UINT32 const());
}

vstring::operator INT32 const() const throw()
{
    return ::_wtoi(m_wstr.c_str());
}

vstring::operator UINT32 const() const throw()
{
    return static_cast<UINT32>(::wcstoul(m_wstr.c_str(), NULL, 10));
}

vstring::operator INT64 const() const throw()
{
    return ::_wtoi64(m_wstr.c_str());
}

vstring::operator UINT64 const() const throw()
{
    return ::_wcstoui64(m_wstr.c_str(), NULL, 10);
}

vstring::operator long const() const throw()
{
    return ::_wtol(m_wstr.c_str());
}

vstring::operator unsigned long const() const throw()
{
    return ::wcstoul(m_wstr.c_str(), NULL, 10);
}

vstring::operator float const()       const    throw()
{
    return static_cast<float>(operator double const());
}

vstring::operator double const() const throw()
{    
    return ::_wtof(m_wstr.c_str());
}

vstring::operator bool const() const throw()
{
    int iNo = *this;
    if (iNo != 0)
    {
        return true;
    }

    auto kTemp = *this;
    kTemp.Trim();

    if(0 == kTemp.icmp(L"TRUE") )
    {
        return true;
    }

    return false;
}

vstring::operator wchar_t const*()   const    throw()
{
    return m_wstr.c_str();
}

vstring::operator vstring const&() const    throw()
{
    return (*this);
}

vstring::operator std::string const() const    throw()
{
    return ConvToMB(m_wstr);
}

vstring::operator std::wstring const() const    throw()
{
    return m_wstr;
}

vstring::operator std::wstring const&() const    throw()
{
    return m_wstr;
}

vstring::operator CL::GUID const() const    throw()
{
	CL::GUID kGuid(m_wstr);
    return kGuid;
}

vstring::operator CL::QUID const() const    throw()
{
    INT64 const iValue = (*this);
    CL::QUID kQuid(iValue);
    return kQuid;
}


vstring::operator CL::DBTIMESTAMP_EX const()  const    throw()
{
    std::string const sTime = vstring::ConvToMB(Get());

    std::istringstream istr(sTime);
    SYSTEMTIME st = { 0 };
    FILETIME ft = { 0 };

    istr >> st.wYear;
    istr.ignore(1, '-');
    istr >> st.wMonth;
    istr.ignore(1, '-');
    istr >> st.wDay;
    istr.ignore(1, ' ');
    istr >> st.wHour;
    istr.ignore(1, ':');
    istr >> st.wMinute;
    istr.ignore(1, ':');
    istr >> st.wSecond;
    istr.ignore(1, '.');
    istr >> st.wMilliseconds;

    ::SystemTimeToFileTime(&st, &ft);
    
    DBTIMESTAMP_EX kDate;
    kDate = ft;
    return kDate;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Operator ==
bool vstring::operator==(vstring const& vstrData)const
{
    return (m_wstr == vstrData.m_wstr);
}

bool vstring::operator == (std::string const& strData)const
{
    return (m_wstr == ConvToUNI(strData));
}

bool vstring::operator==(std::wstring const& strData)const
{
    return (m_wstr == strData);
}

////////////////////////////////////////////
//    operator=()
vstring& vstring::operator = (char const& rhs)
{
    std::string kStr(1, rhs);
    return operator = (ConvToUNI(kStr));
}

vstring& vstring::operator = (short const& rhs)
{
    return operator = (static_cast<int const>(rhs));
}

vstring& vstring::operator = (int const& rhs)
{
    wchar_t wszTemp[MAX_PATH] = {0,};
    errno_t const err = ::_itow_s(rhs, wszTemp, MAX_PATH, 10);
    if( err )
    {
        return* this;
    }
    return operator = (wszTemp);
}

vstring& vstring::operator = (long const& rhs)
{
    return operator = (static_cast<UINT64 const>(rhs));
}

vstring& vstring::operator = (INT64 const& rhs)
{
    wchar_t wszTemp[MAX_PATH] = {0,};
    errno_t const err = ::_i64tow_s(rhs, wszTemp, MAX_PATH, 10);
    if( err )
    {
        return* this;
    }
    return operator = (wszTemp);
}

vstring& vstring::operator = (UINT8 const& rhs)
{
    return operator = (static_cast<unsigned int const>(rhs));
}

vstring& vstring::operator = (unsigned short const& rhs)
{
    return operator = (static_cast<unsigned int const>(rhs));
}

vstring& vstring::operator = (unsigned int const& rhs)
{
    return operator = (static_cast<UINT64 const>(rhs));
}

vstring& vstring::operator = (unsigned long const& rhs)
{
    return operator = (static_cast<UINT64 const>(rhs));
}

vstring& vstring::operator = (UINT64 const& rhs)
{
    wchar_t wszTemp[MAX_PATH] = {0,};
    errno_t const err = ::_ui64tow_s(rhs, wszTemp, MAX_PATH, 10);
    if(err)
    {
        return* this;
    }
    return operator = (wszTemp);
}

vstring& vstring::operator = (bool const& rhs)
{
    if( rhs )
    {
        return operator = (L"true");
    }
    return operator = (L"false");
}

vstring& vstring::operator = (wchar_t const& rhs)
{
    std::wstring kWStr(1, rhs);
    return operator = (kWStr);
}

vstring& vstring::operator = (float const& rhs)
{
    wchar_t wszTemp[MAX_PATH] = {0,};
    ::swprintf_s( wszTemp, MAX_PATH, L"%f", rhs );//.4f 로 바꾸는게 좋을 수도 있겠다.

//    char buffer[_CVTBUFSIZE] = {0,};
//    errno_t err = _gcvt_s(buffer, _CVTBUFSIZE, rhs, 20);//float 은 플롯 대로 컨버팅. 더블로 캐스팅 하면 결과가 다르게 나옴.
//
//    m_wstr = ConvToUNI(buffer);
    return operator = (wszTemp);
}

vstring& vstring::operator = (double const& rhs)
{
    char buffer[_CVTBUFSIZE] = {0,};
    /*errno_t err =*/ _gcvt_s(buffer, _CVTBUFSIZE, rhs, 20);//double은 더블대로
    return operator = (ConvToUNI(buffer));
}

vstring& vstring::operator = (void const* const& rhs)
{
    wchar_t wszTemp[MAX_PATH] = {0,};
    ::swprintf_s( wszTemp, MAX_PATH, L"0x%p", rhs );
    return operator = (wszTemp);
}

vstring& vstring::operator = (char const* const& rhs)
{
    if( !rhs )
    {
        return operator = (L"Null szString");
    }
    return operator = (ConvToUNI(rhs));
}

vstring& vstring::operator = (wchar_t const* const& rhs)
{
    if(rhs)
    {
        m_wstr = rhs;
    }
    else
    {
        m_wstr = L"Null wszString";
    }
    return* this;
}

vstring& vstring::operator = (std::string const& rhs)
{
    return operator = (ConvToUNI(rhs));
}

vstring& vstring::operator = (std::wstring const& rhs)
{
    m_wstr = rhs;
    return* this;
}

vstring& vstring::operator = (QUID const& rhs)
{
    return operator = ((INT64)(rhs));
}

vstring& vstring::operator = (::GUID const& rhs)
{
    return operator = (CL::GUID(rhs));
}

vstring& vstring::operator = (DBTIMESTAMP const& rhs)
{
    return operator = (CL::DBTIMESTAMP_EX(rhs));
}

vstring& vstring::operator = (CL::GUID const& rhs)
{
    return operator = (rhs.str());
}

vstring& vstring::operator = (DBTIMESTAMP_EX const& rhs)
{
    return operator =(rhs.operator const std::wstring());
}

vstring& vstring::operator = (PackedTime_ const& rhs)
{
    return operator =(rhs.operator std::wstring());
}

vstring& vstring::operator = (vstring const& rhs)
{
    return operator =(rhs.m_wstr);
}

////////////////////////////////////////////
//    operator<()
bool vstring::operator < (vstring const& rhs)const
{
    return m_wstr < rhs.m_wstr;
}

////////////////////////////////////////////
//    etc

// etc static
wchar_t vstring::ConvToUNI(char const src)
{
    wchar_t target = 0;

    size_t size = 0;
    ::mbstowcs_s(&size, &target, sizeof(target), &src, sizeof(src));
    
    return target;
}

errno_t vstring::ConvToUNI(std::string const& strSrc, std::wstring& wstrTgt)
{
    size_t const src_size = strSrc.size();
    size_t const BUFFER_SIZE = (src_size*4);

    if(0 < src_size)
    {
        wchar_t* pUNIBuffer = new wchar_t[BUFFER_SIZE];
        
        size_t size = 0;
        errno_t const err = ::mbstowcs_s(& size, pUNIBuffer, BUFFER_SIZE, strSrc.c_str(), BUFFER_SIZE );
        if( !err && size )
        {
            wstrTgt = pUNIBuffer;
        }
        SAFE_DELETE_ARRAY(pUNIBuffer);
        return err;
    }
    return EINVAL;
}

std::wstring vstring::ConvToUNI(std::string const& strSrc)
{
    std::wstring strOut;
    vstring::ConvToUNI(strSrc, strOut);
    return strOut;
}

std::string vstring::ConvToUTF8FromAnsi(std::string const& strSrc)
{
	std::string strOut;
	auto const src_size = strSrc.length();

	if (0 < src_size)
	{
		auto const conv_length = ::MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)src_size, NULL, NULL);
		if (0 < conv_length)
		{
			std::wstring kBuffer;
			kBuffer.resize(conv_length);
            ::MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)src_size, &kBuffer.at(0), conv_length);

			int const conv_length2 = ::WideCharToMultiByte(CP_UTF8, 0, kBuffer.c_str(), -1, NULL, 0, NULL, NULL);
			if (0 < conv_length2)
			{
				strOut.resize(conv_length2);
				::WideCharToMultiByte(CP_UTF8, 0, kBuffer.c_str(), -1, &strOut.at(0), conv_length2, NULL, NULL);
			}
		}
	}
	return strOut;
}

std::string vstring::ConvToUTF8FromUNI(std::wstring const& strSrc)
{
    std::string strOut;
    int const src_size = (int)strSrc.length();

    int conv_length = WideCharToMultiByte(CP_UTF8, 0, strSrc.c_str(), src_size, NULL, 0, NULL, NULL);
    if (0 < conv_length)
    {
        strOut.resize(conv_length);
        auto err = WideCharToMultiByte(CP_UTF8, 0, strSrc.c_str(), src_size, &strOut[0], conv_length, NULL, NULL);
        if (err == 0)
        {
            err = GetLastError();
        }
    }
    return strOut;
}

std::wstring vstring::ConvToUNIFromUTF8(std::string const& strSrc)
{
    std::wstring strOut;
    size_t const src_size = strSrc.size();

    if(0 < src_size)
    {
#ifdef _WIN64
        size_t conv_length = ::MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), static_cast<int>(src_size + 1), NULL, NULL);
#else
        int conv_length = ::MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), src_size + 1, NULL, NULL);
#endif
        if(0 < conv_length)
        {
            wchar_t* pUNIBuffer = new wchar_t[conv_length + 1];
#ifdef _WIN64
            size_t ret = ::MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), static_cast<int>(src_size + 1), pUNIBuffer, static_cast<int>(conv_length));
#else
            int ret = ::MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), src_size + 1, pUNIBuffer, conv_length);
#endif
            if(0 < ret)
            {
                pUNIBuffer[conv_length] = 0;
                strOut = pUNIBuffer;
            }
            SAFE_DELETE_ARRAY(pUNIBuffer);            
        }
    }
    return strOut;
}

errno_t vstring::ConvToMB(std::wstring const& wstrSrc, std::string& strTgt)
{
	int const nLen = ::WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), -1, NULL, 0, NULL, NULL);
    if (!wstrSrc.empty())
	{
		char* szMb = new char[nLen];

		::WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), -1, szMb, nLen, NULL, NULL);

		strTgt = szMb;

		SAFE_DELETE_ARRAY(szMb);
		return 0;
	}
	return EINVAL;
}

/* 아래 함수는 아스키 코드에만 동작 합니다.
{
    size_t const src_size = wstrSrc.size();
    size_t const buffer_size = (src_size*4);

    if(0 < src_size)
    {
        char* pMBBuffer = new char[buffer_size];
        if(pMBBuffer)
        {
            size_t size = 0;
            errno_t const err = ::wcstombs_s(& size, pMBBuffer, buffer_size, wstrSrc.c_str(), buffer_size );
            if( !err && size )
            {
                strTgt = pMBBuffer;
            }

            SAFE_DELETE_ARRAY(pMBBuffer);
            return err;
        }
    }

    return EINVAL;
}
*/

std::string vstring::ConvToMB(std::wstring const& wstrSrc)
{
    std::string strOut;
    vstring::ConvToMB(wstrSrc, strOut);
    return strOut;
}

std::string vstring::ConvToUTF8(std::string const& strSrc)
{
	BSTR	bstrCode; 
	char	*pszUTFCode = nullptr;

    int nLength = MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)strSrc.size() + 1, NULL, NULL);
	bstrCode = SysAllocStringLen(NULL, nLength); 
    MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), (int)strSrc.size() + 1, bstrCode, nLength);

    int	nLength2 = WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, 0, NULL, NULL);
	pszUTFCode = new char[nLength2+1]; 
	WideCharToMultiByte(CP_UTF8, 0, bstrCode, -1, pszUTFCode, nLength2, NULL, NULL); 

    ::SysFreeString(bstrCode);
    std::string kstring(pszUTFCode);
    SAFE_DELETE_ARRAY(pszUTFCode);

	return kstring;
}

std::string vstring::Base64Encode(std::string const& strSrc)
{
	std::string ret;
	int i = 0;
	unsigned char char_array_3[3] = {0,};
	unsigned char char_array_4[4] = {0,};

	size_t in_len = strSrc.length();
	unsigned char const* bytes_to_encode = (unsigned char const*)strSrc.c_str();

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); ++i)
				ret += base64_chars[char_array_4[i]];

			i = 0;
		}
	}

	if (i)
	{
		for (int j = i; j < 3; ++j)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (int j = 0; (j < i + 1); ++j)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';
	}

	return ret;
}

std::string vstring::Base64Decode(std::string const& strSrc)
{
	size_t in_len = strSrc.length();
	int i = 0;
	size_t in_ = 0;
	unsigned char char_array_4[4] = {0}, char_array_3[3]= {0};
	std::string ret;

	while (in_len-- && (strSrc[in_] != '=') && is_base64(strSrc[in_]))
	{
		char_array_4[i++] = strSrc[in_]; in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; ++i)
				char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); ++i)
            {
                ret += char_array_3[i];
            }

			i = 0;
		}
	}

	if (i)
	{
		for (int j = i; j < 4; ++j)
			char_array_4[j] = 0;

		for (int j = 0; j < 4; ++j)
			char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (int j = 0; (j < i - 1); ++j)
        {
            ret += char_array_3[j];
        }
	}

	return ret;
}

void vstring::clear()
{
    m_wstr.clear();
}

size_t vstring::size() const
{
    return m_wstr.size();
}

size_t vstring::length() const
{
    return m_wstr.length();
}

bool vstring::IsEmpty()const
{
    return m_wstr.empty();
}

std::wstring vstring::ConvTo16Bit(DWORD const dwIn)
{
    wchar_t kTemp[MAX_PATH] = {0, };
    swprintf_s(kTemp, MAX_PATH, L"%X", dwIn);
    return std::wstring(kTemp);
}

std::wstring vstring::ConvToHex(DWORD const dwIn)
{
    wchar_t kTemp[MAX_PATH] = {0, };
    swprintf_s(kTemp, MAX_PATH, L"0x%08X", dwIn);
    return std::wstring(kTemp);
}

std::wstring vstring::ConvToHex(BYTE const* pData, size_t const iArrayCount, size_t const iLimitArray, bool const bAddHeader)
{
    return ConvToHex((char const* )(pData), iArrayCount, iLimitArray, bAddHeader);
}

std::wstring vstring::ConvToHex(char const* pData, size_t const iArrayCount, size_t const iLimitArray, bool const bAddHeader)
{
    if( !pData )
    {
        return std::wstring(L"Null Pointer");
    }

    size_t const iMultiply = 2; // 'a' --> 0x?? 로 두배 뻥튀기
    //size_t const iOneBytePerChar = 2;

    std::wstring kTemp;
    if(bAddHeader)
    {
        kTemp += L"0x";
    }
    kTemp.reserve( kTemp.size() + (iArrayCount*  iMultiply) ); // '0x' + 'xxxxx................'

    size_t iCur = 0;
    char const* const pEnd = (pData + iArrayCount);
    char const* pBegin = pData;
    wchar_t szTemp[10] = {0 ,};
    while( pEnd != pBegin )
    {
        if( iLimitArray < iCur )
        {
            kTemp.append( std::wstring(L".....") );
            break;
        }

        _stprintf_s(szTemp, 10, L"%02X", BYTE(*pBegin));
        kTemp.push_back( szTemp[0] );
        kTemp.push_back( szTemp[1] );

        ++iCur;
        ++pBegin;
    }
    return kTemp;
}

INT64 vstring::ConvFromHex(std::wstring const& str)
{
    return std::wcstoll(str.c_str(), nullptr, 16);
}

// etc member
int vstring::icmp(vstring const& vstrData) const
{
    return icmp( vstrData.m_wstr );
}

int vstring::icmp(std::wstring const& strData) const
{
    return icmp( m_wstr, strData );
}

int vstring::icmp(std::wstring const& lhs, std::wstring const& rhs)
{
    return ::_wcsicmp( lhs.c_str(), rhs.c_str() );
}

int vstring::icmp(std::string const& lhs, std::string const& rhs)
{
    return ::_stricmp( lhs.c_str(), rhs.c_str() );
}

int vstring::cmp(vstring const& vstrData) const
{
    return cmp(vstrData.m_wstr);
}

int vstring::cmp(std::wstring const& strData) const
{
    return cmp(m_wstr, strData);
}

int vstring::cmp(std::wstring const& lhs, std::wstring const& rhs)
{
    return ::wcscmp(lhs.c_str(), rhs.c_str());
}

int vstring::cmp(std::string const& lhs, std::string const& rhs)
{
    return ::strcmp(lhs.c_str(), rhs.c_str());
}

void vstring::Trim()
{
    Trim(m_wstr);
}

}//namespace CL
