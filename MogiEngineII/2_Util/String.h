#pragma once

namespace Mogi
{
	class cString
	{
	public:
		cString();
		virtual ~cString();

	public:
		const char*									c_str()										{ return m_String.c_str(); }
		size_t										size()										{ return m_String.size(); }


		std::wstring								GetWString()
		{
			std::wstring wstrRet;
			Mogi::cString::ConvertToUnicode(m_String, wstrRet);
			return wstrRet;
		}
		



		static bool ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& strWDst)
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

		static bool ComvertToMultiByte(IN std::wstring const& strWSrc, OUT std::string& strDst)
		{
			return true;
		}

	private:
		std::string									m_String;
	};
}
