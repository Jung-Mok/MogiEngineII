#include "pch.h"

namespace nMogi
{
	cString::cString()
	{

	}

	cString::~cString()
	{

	}

	std::wstring cString::GetWString()
	{
		std::wstring wstrRet;
		nMogi::cString::ConvertToUnicode(m_String, wstrRet);
		return wstrRet;
	}

	bool cString::Format(char const* pFormat, ...)
	{
		char Buffer[MAX_STRING_BUFFER_SIZE] = { 0 };
		va_list Args;
		va_start(Args, pFormat);
		vsprintf_s(Buffer, MAX_STRING_BUFFER_SIZE, pFormat, Args);
		va_end(Args);

		m_String += Buffer;

		return true;
	}

	void cString::Append(char const* strSrc)
	{
		m_String += strSrc;
	}

	void cString::Append(std::string const& strSrc)
	{
		m_String += strSrc;
	}

	void cString::Append(std::wstring const& strSrc)
	{
		std::string Temp;
		cString::ConvertToMultiByte(strSrc, Temp);
		m_String += Temp;
	}

	void cString::Append(char const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String += Temp;
	}

	void cString::Append(short const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String += Temp;
	}

	void nMogi::cString::Append(int const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String += Temp;
	}

	void nMogi::cString::Append(__int64 const& Src)
	{
		std::string Temp;
		cString::I64toA(Src, Temp);
		m_String += Temp;
	}

	void cString::Append(unsigned char const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String += Temp;
	}

	void cString::Append(unsigned short const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String += Temp;
	}

	void nMogi::cString::Append(unsigned int const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String += Temp;
	}

	void cString::Append(unsigned __int64 const& Src)
	{
		std::string Temp;
		cString::I64toA(Src, Temp);
		m_String += Temp;
	}

	void cString::Append(double const& Src)
	{
		std::string Temp;
		cString::FtoA(Src, Temp);
		m_String += Temp;
	}

	void cString::SetString(char const* strSrc)
	{
		m_String = strSrc;
	}

	void cString::SetString(std::string const& strSrc)
	{
		m_String = strSrc;
	}

	void cString::SetString(std::wstring const& strSrc)
	{
		std::string Temp;
		cString::ConvertToMultiByte(strSrc, Temp);
		m_String = Temp;
	}

	void cString::SetString(char const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String = Temp;
	}

	void cString::SetString(short const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String = Temp;
	}

	void nMogi::cString::SetString(int const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String = Temp;
	}

	void nMogi::cString::SetString(__int64 const& Src)
	{
		std::string Temp;
		cString::I64toA(Src, Temp);
		m_String = Temp;
	}

	void cString::SetString(unsigned char const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String = Temp;
	}

	void cString::SetString(unsigned short const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String = Temp;
	}

	void nMogi::cString::SetString(unsigned int const& Src)
	{
		std::string Temp;
		cString::ItoA(Src, Temp);
		m_String = Temp;
	}

	void cString::SetString(unsigned __int64 const& Src)
	{
		std::string Temp;
		cString::I64toA(Src, Temp);
		m_String = Temp;
	}

	void cString::SetString(double const& Src)
	{
		std::string Temp;
		cString::FtoA(Src, Temp);
		m_String = Temp;
	}

	cString::operator std::string const() const throw()
	{
		return m_String;
	}

	cString::operator std::wstring const() const throw()
	{
		std::wstring Temp;
		cString::ConvertToUnicode(m_String, Temp);
		return Temp;
	}

	bool cString::ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& wstrDst)
	{
		size_t const BUFFER_SIZE = (strSrc.size() * 2);

		if (0 >= BUFFER_SIZE)
			return false;

		wchar_t* pUNIBuffer = new wchar_t[BUFFER_SIZE + sizeof(wchar_t)];

		size_t RetSize = 0;

		errno_t const RetValue = ::mbstowcs_s(&RetSize, pUNIBuffer, BUFFER_SIZE, strSrc.c_str(), BUFFER_SIZE);

		if (0 != RetValue)
			return false;

		if (RetSize == 0)
			return false;

		wstrDst = pUNIBuffer;

		delete[] pUNIBuffer;

		return true;
	}

	bool cString::ConvertToMultiByte(IN std::wstring const& wstrSrc, OUT std::string& strDst)
	{
		if (true == wstrSrc.empty())
			return false;

		int const BUFFER_SIZE = ::WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), -1, NULL, 0, NULL, NULL);

		if (true == wstrSrc.empty())
			return false;
		
		char* pMutiBuffer = new char[BUFFER_SIZE + sizeof(char)];

		::WideCharToMultiByte(CP_ACP, 0, wstrSrc.c_str(), -1, pMutiBuffer, BUFFER_SIZE, NULL, NULL);

		strDst = pMutiBuffer;

		delete[] pMutiBuffer;

		return true;
	}

	int cString::AtoI(IN std::string const strSrc)
	{
		return ::atoi(strSrc.c_str());
	}

	double cString::AtoF(IN std::string const strSrc)
	{
		return ::atof(strSrc.c_str());
	}

	__int64 cString::AtoI64(IN std::string const strSrc)
	{
		return ::_atoi64(strSrc.c_str());
	}

	unsigned int cString::AtoUI(IN std::string const strSrc)
	{
		return ::strtoul(strSrc.c_str(), NULL, 10);
	}

	unsigned __int64 cString::AtoUI64(IN std::string const strSrc)
	{
		return ::_strtoi64(strSrc.c_str(), NULL, 16);
	}

	void cString::ItoA(IN int const& Src, OUT std::string& strDst)
	{
		int Temp = 0;
		char Buffer[MAX_CONVERT_BUFFER_SIZE];
		_itoa_s(Src, Buffer, MAX_CONVERT_BUFFER_SIZE, Temp);
		strDst = Buffer;
	}

	void cString::I64toA(IN __int64 const& Src, OUT std::string& strDst)
	{
		int Temp = 0;
		char Buffer[MAX_CONVERT_BUFFER_SIZE];
		_i64toa_s(Src, Buffer, MAX_CONVERT_BUFFER_SIZE, Temp);
		strDst = Buffer;
	}

	void cString::FtoA(IN double const& Src, OUT std::string& strDst)
	{
		nMogi::cString temp;
		temp.Format("%f", Src);
		strDst = temp;
	}

}
