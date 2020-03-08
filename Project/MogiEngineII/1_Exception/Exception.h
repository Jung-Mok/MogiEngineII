#pragma once

namespace nMogi
{
	class cException
	{
	public:
		cException() {}
		cException(unsigned int _ErrorCode, EXCEPTION_POINTERS* _pExceptionPointers, int _GetLastError = 0);
		virtual ~cException();

	public:
		unsigned int								GetCode()				{ return m_ExceptionCode; }
		EXCEPTION_POINTERS*							GetInformation()		{ return m_pExceptionInformation; }
		nMogi::eErrorCode							GetErrorCode()			{ return nMogi::eErrorCode(m_ExceptionCode); }
		int											GetLastError()			{ return m_GetLastError; }

	private:
		unsigned int								m_ExceptionCode			= 0;
		EXCEPTION_POINTERS*							m_pExceptionInformation = nullptr;
		int											m_GetLastError			= 0;
	};
}