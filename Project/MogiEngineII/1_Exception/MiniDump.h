#pragma once

namespace nMogi
{
	class cMiniDump
	{
	public:
		static void									InitMiniDump();
		static void									SetFolder(IN const char* _strFolderName);
		static void									TransFunc(IN unsigned int _ExceptionCode, IN EXCEPTION_POINTERS* _pExceptionPointers);
		static void									PrintLastError(IN DWORD _Error = GetLastError());
		static long									UnHandledExceptionFilter(IN nMogi::cException& _ExceptionInfo);
		static long 								OnExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);

		static void									DumpTest();

	private:
		static char									m_strFolder[MAX_PATH];
	};
}