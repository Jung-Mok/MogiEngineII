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
