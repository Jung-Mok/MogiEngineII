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
		// ��Ƽ����Ʈ�� �����ڵ�� ��ȯ
		static bool									ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& wstrDst);
		// �����ڵ带 ��Ƽ����Ʈ�� ��ȯ
		static bool									ComvertToMultiByte(IN std::wstring const& wstrSrc, OUT std::string& strDst);

	private:
		std::string									m_String;
	};
}
