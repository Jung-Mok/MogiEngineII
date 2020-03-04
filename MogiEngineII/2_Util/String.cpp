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

	bool cString::ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& strWDst)
	{
		size_t const src_size = strSrc.size();
		size_t const BUFFER_SIZE = (src_size * 4);

		if (0 < src_size)
		{
			wchar_t* pUNIBuffer = new wchar_t[BUFFER_SIZE];

			size_t size = 0;
			errno_t const err = ::mbstowcs_s(&size, pUNIBuffer, BUFFER_SIZE, strSrc.c_str(), BUFFER_SIZE);
			if (!err && size)
			{
				strWDst = pUNIBuffer;
			}

			delete[] pUNIBuffer;
			return err;
		}

		return true;
	}

	bool cString::ComvertToMultiByte(IN std::wstring const& strWSrc, OUT std::string& strDst)
	{
		return true;
	}

}

