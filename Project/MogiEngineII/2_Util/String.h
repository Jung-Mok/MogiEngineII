#pragma once

namespace nMogi
{
	#define MAX_STRING_BUFFER_SIZE 1024
	#define MAX_CONVERT_BUFFER_SIZE 128

	class cString
	{
	public:
		cString();
		virtual ~cString();

	public:
		const char*									c_str()										{	return m_String.c_str();		}
		size_t										size()										{	return m_String.size();			}
		size_t										length()									{	return m_String.length();		}

		std::wstring								GetWString();
		bool										Format(char const* pFormat, ...);

		void										Append(char const* strSrc);
		void										Append(std::string const& strSrc);
		void										Append(std::wstring const& wstrSrc);
		void										Append(char const& Src);
		void										Append(short const& Src);
		void										Append(int const& Src);
		void										Append(__int64 const& Src);
		void										Append(unsigned char const& Src);
		void										Append(unsigned short const& Src);
		void										Append(unsigned int const& Src);
		void										Append(unsigned __int64 const& Src);
		void										Append(double const& Src);

	public:
		void										SetString(char const* strSrc);
		void										SetString(std::string const& strSrc);
		void										SetString(std::wstring const& strSrc);
		void										SetString(char const& Src);
		void										SetString(short const& Src);
		void										SetString(int const& Src);
		void										SetString(__int64 const& Src);
		void										SetString(unsigned char const& Src);
		void										SetString(unsigned short const& Src);
		void										SetString(unsigned int const& Src);
		void										SetString(unsigned __int64 const& Src);
		void										SetString(double const& Src);

	public:
		operator std::string const() const throw();
		operator std::wstring const() const throw();

		bool operator == (nMogi::cString const& strSrc)const;
		bool operator == (std::string const& strSrc)const;
		bool operator == (std::wstring const& wstrSrc)const;

		template<typename T>						cString& operator <<(T const& Src)			{	this->Append(Src);		return *this;	}
		template<typename T>						cString& operator +=(T const& Src)			{	this->Append(Src);		return *this;	}
		template<typename T>						cString& operator =(T const& Src)			{	this->SetString(Src);	return *this;	}

	public:
		// 멀티바이트를 유니코드로 전환
		static bool									ConvertToUnicode(IN std::string const& strSrc, OUT std::wstring& wstrDst);
		// 유니코드를 멀티바이트로 전환
		static bool									ConvertToMultiByte(IN std::wstring const& wstrSrc, OUT std::string& strDst);

		static int									AtoI(IN std::string const strSrc);
		static double								AtoF(IN std::string const strSrc);
		static __int64								AtoI64(IN std::string const strSrc);
		static unsigned int							AtoUI(IN std::string const strSrc);
		static unsigned __int64						AtoUI64(IN std::string const strSrc);

		static void									ItoA(IN int const& Src, OUT std::string& strDst);
		static void									I64toA(IN __int64 const& Src, OUT std::string& strDst);
		static void									FtoA(IN double const& Src, OUT std::string& strDst);

	private:
		std::string									m_String;
	};
}
