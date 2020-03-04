#pragma once

namespace Mogi
{
	class cString
	{
	public:
		cString();
		virtual ~cString();

	public:
		const char*									c_str()										{ return m_String.c_str();		}
		size_t										size()										{ return m_String.size();		}

		std::wstring								GetWString();
		


	public:
		// 멀티바이트를 유니코드로 전환
		static bool									ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& strWDst);
		// 유니코드를 멀티바이트로 전환
		static bool									ComvertToMultiByte(IN std::wstring const& strWSrc, OUT std::string& strDst);

	private:
		std::string									m_String;
	};
}
