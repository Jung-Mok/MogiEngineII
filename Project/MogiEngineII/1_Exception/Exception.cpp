#include "pch.h"

namespace nMogi
{
	cException::cException(unsigned int _ErrorCode, EXCEPTION_POINTERS* _pExceptionPointers, int _GetLastError)
		: m_ExceptionCode(_ErrorCode), m_pExceptionInformation(_pExceptionPointers), m_GetLastError(_GetLastError)
	{

	}

	cException::~cException()
	{

	}

}


