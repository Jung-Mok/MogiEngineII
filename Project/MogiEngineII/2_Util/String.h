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
		template< typename T1 >
		cString& operator += (T1 const& rhs)
		{

		}

		template<typename T>
		cString& operator <<(T const& rhs)
		{

		}


	public:
		// 멀티바이트를 유니코드로 전환
		static bool									ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& wstrDst);
		// 유니코드를 멀티바이트로 전환
		static bool									ConvertToMultiByte(IN std::wstring const& wstrSrc, OUT std::string& strDst);
		//static int ICMP()

	private:
		std::string									m_String;
	};
}
