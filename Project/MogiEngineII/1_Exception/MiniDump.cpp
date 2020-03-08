#include "pch.h"

namespace nMogi
{
	char cMiniDump::m_strFolder[MAX_PATH] = { 0, };

	typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)
		(
			HANDLE	hProcess,
			DWORD	dwPid,
			HANDLE	hFile,
			MINIDUMP_TYPE DumpType,
			CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam,
			CONST PMINIDUMP_USER_STREAM_INFORMATION	UserStreamParam,
			CONST PMINIDUMP_CALLBACK_INFORMATION	CallbackParam
			);

	void cMiniDump::InitMiniDump()
	{
		_set_se_translator(cMiniDump::TransFunc);

		// 잘못된 연산 메시지 창이 나오지 않도록 설정
#ifndef _DEBUG
		SetErrorMode(SEM_NOGPFAULTERRORBOX);
#endif
		static LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = nullptr;
		PreviousExceptionFilter = SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)cMiniDump::UnHandledExceptionFilter);
	}

	void cMiniDump::SetFolder(IN const char* _strFolderName)
	{
		sprintf_s(m_strFolder, "%s", _strFolderName);
	}

	void cMiniDump::TransFunc(IN unsigned int _ExceptionCode, IN EXCEPTION_POINTERS* _pExceptionPointers)
	{
		nMogi::cException Exception(_ExceptionCode, _pExceptionPointers);// = Mogi::cException(_ErrorCode, _pExceptionPointers);
		nMogi::cMiniDump::UnHandledExceptionFilter(Exception);
		throw Exception;
	}

	void cMiniDump::PrintLastError(IN DWORD _Error /*= GetLastError()*/)
	{
		char* strMsg;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
			, nullptr
			, _Error
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
			, (LPSTR)&strMsg
			, 0
			, nullptr);

		assert(false);

		LocalFree(strMsg);
	}

	LONG WINAPI cMiniDump::UnHandledExceptionFilter(IN nMogi::cException& _ExceptionInfo)
	{
		//반드시 로딩
		HMODULE DllHandle = LoadLibraryA("DBGHELP.DLL");
		if (DllHandle == nullptr)
		{
			PrintLastError();
			return EXCEPTION_CONTINUE_SEARCH;
		}

		//DBGHELP.DLL에서 MiniDumpWriteDump를 불러와 Dump라고 정의하며 이걸로 덤프 파일을 생성합니다.
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");
		if (Dump == nullptr)
		{
			PrintLastError();
			return EXCEPTION_CONTINUE_SEARCH;
		}

		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);	//현재시간 획득

		//덤프 파일 이름 설정
		char strDumpFullPath[MAX_PATH] = { 0, };
		char strModulePath[MAX_PATH] = { 0, };
		char strTempFileName[MAX_PATH] = { 0, };
		char strFolderName[MAX_PATH] = { 0, };
		char strFileName[MAX_PATH] = { 0, };

		// 파일 이름 얻기
		::GetModuleFileNameA(nullptr, strModulePath, MAX_PATH);
		_splitpath_s(strModulePath, nullptr, 0, nullptr, 0, strTempFileName, MAX_PATH, nullptr, 0);

		sprintf_s(strFileName, "%s_%d%02d%02d_%02d%02d_%02d.dmp", strTempFileName, SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

		//폴더 만들기
		if (strlen(m_strFolder))
		{
			sprintf_s(strFolderName, "./%s/", m_strFolder);
			int Return = _mkdir(strFolderName);
			UNREFERENCED_PARAMETER(Return);
		}

		sprintf_s(strDumpFullPath, "%s%s", strFolderName, strFileName);

		//덤프 파일 생성 
		HANDLE hFileHandle = CreateFileA(strDumpFullPath, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hFileHandle == INVALID_HANDLE_VALUE)
		{
			PrintLastError();
			return EXCEPTION_CONTINUE_SEARCH;
		}

		//MiniDump 예외 정보 저장 구조체
		_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;
		MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
		MiniDumpExceptionInfo.ExceptionPointers = _ExceptionInfo.GetInformation();
		MiniDumpExceptionInfo.ClientPointers = FALSE;

		//현재 프로세스에 대한 덤프 기록을 실행합니다.
		BOOL Success = Dump(GetCurrentProcess(), GetCurrentProcessId(), hFileHandle, MiniDumpNormal, &MiniDumpExceptionInfo, nullptr, nullptr);

		//SetUnhandledExceptionFilter(PreviousExceptionFilter);
		CloseHandle(hFileHandle);
		//덤프 기록 설공시 수행
		if (FALSE == Success)
		{

			return EXCEPTION_CONTINUE_SEARCH;
		}
			

		return EXCEPTION_EXECUTE_HANDLER;
	}

	void cMiniDump::DumpTest()
	{
		nMogi::cMiniDump::InitMiniDump();
		nMogi::cMiniDump::SetFolder("Exception");

		//assert(false && "Test");

		for (int i = 0; i < 5; ++i)
		{
			//try
			//{
			//	int* p = nullptr;
			//}
			//catch (nMogi::cException e)
			//{
			//	printf("Caught a __try exception with SE_Exception.\n");
			//	printf("nSE = 0x%x\n", e.GetErrorCode());
			//}
			//Sleep(1000);
		}

	}

}