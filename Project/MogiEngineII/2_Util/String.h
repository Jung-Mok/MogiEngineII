#pragma once

namespace Mogi
{
	#define MAX_STRING_BUFFER_SIZE 1024

	class cString
	{
	public:
		cString();
		virtual ~cString();

	public:
		const char*									c_str()										{ return m_String.c_str();		}
		size_t										size()										{ return m_String.size();		}

		std::wstring								GetWString();
		bool										Format(char const* pFormat, ...);

		void										Append(char const* strSrc)
		{
			m_String += strSrc;
		}
		

	public:
		template<typename T>
		cString& operator <<(T const& rhs)
		{
			this->Append(rhs);
			return *this;
		}


	public:
		// ��Ƽ����Ʈ�� �����ڵ�� ��ȯ
		static bool									ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& wstrDst);
		// �����ڵ带 ��Ƽ����Ʈ�� ��ȯ
		static bool									ConvertToMultiByte(IN std::wstring const& wstrSrc, OUT std::string& strDst);
		//static int ICMP()

	private:
		std::string									m_String;
	};
}
