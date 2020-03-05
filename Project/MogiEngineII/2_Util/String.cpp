#include "pch.h"

namespace Mogi
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
		Mogi::cString::ConvertToUnicode(m_String, wstrRet);
		return wstrRet;
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

	bool cString::ComvertToMultiByte(IN std::wstring const& wstrSrc, OUT std::string& strDst)
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

}

