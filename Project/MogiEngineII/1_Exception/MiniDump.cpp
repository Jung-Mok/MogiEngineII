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

	long cMiniDump::UnHandledExceptionFilter(IN nMogi::cException& _ExceptionInfo)
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

		OnExceptionFilter(_ExceptionInfo.GetInformation());

		//SetUnhandledExceptionFilter(PreviousExceptionFilter);
		CloseHandle(hFileHandle);
		//덤프 기록 설공시 수행
		if (FALSE == Success)
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}

		return EXCEPTION_EXECUTE_HANDLER;
	}

	long cMiniDump::OnExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
	{
		if (NULL == pExceptionInfo)
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}

		HANDLE const hCurrentProcess = ::GetCurrentProcess();
		//    HANDLE const hCurrentThread = ::GetCurrentThread();

		char FileName[MAX_PATH];
		if (GetModuleFileNameA(NULL, FileName, MAX_PATH))
		{
			PathStripPathA(FileName);
			PathRemoveExtensionA(FileName);
		}

		// Initialize symbols..
		::SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);
		BOOL const bInit = ::SymInitialize(hCurrentProcess, NULL, TRUE);
		if (bInit)
		{
			// Create StackDump File
			char szFileName[MAX_PATH] = { 0, };
			{
				SYSTEMTIME t;
				::GetLocalTime(&t);

				::sprintf_s(szFileName, MAX_PATH, "%s %04d-%02d-%02d %02d-%02d-%02d.txt",
					FileName,
					t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
			}

			FILE* pFile = nullptr;
			errno_t const err = ::fopen_s(&pFile, szFileName, "at+");
			if (!err && pFile)
			{
				char szDate[MAX_PATH] = { 0, };
				char szTime[MAX_PATH] = { 0, };
				::_strdate_s(szDate, sizeof(szDate));
				::_strtime_s(szTime, sizeof(szTime));

				::fprintf(pFile, "*============================================================*\n");
				::fprintf(pFile, "\tDate / Time : [%s %s]\n", szDate, szTime);
				::fprintf(pFile, "\tProcessID / ThreadID : [0x%08X] / [0x%08X]\n", GetCurrentProcessId(), GetCurrentThreadId());
				::fprintf(pFile, "\tExceptionCode : [0x%08X]\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
				::fprintf(pFile, "\tCommandLine : %s\n", GetCommandLineA());
				::fprintf(pFile, "*============================================================*\n");

				::printf("*============================================================*\n");
				::printf("\tUnhandled excetpion triggerd!\n");
				::printf("\tDate / Time : [%s %s]\n", szDate, szTime);
				::printf("\tProcessID / ThreadID : [0x%08X] / [0x%08X]\n", GetCurrentProcessId(), GetCurrentThreadId());
				::printf("\tExceptionCode : [0x%08X]\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
				::printf("\tCommandLine : %s\n", GetCommandLineA());


				StackWalkerToFile sw(pFile);  // output to console
				sw.ShowCallstack(GetCurrentThread(), pExceptionInfo->ContextRecord);

				::fprintf(pFile, "\n");
			}

			if (pFile)
			{
				fclose(pFile);
			}

			::SymCleanup(hCurrentProcess);
		}

		//if (ms_kDumpLevel)
		//{
		//	::printf("\tWriting Dump....\n");
		//	::printf("*============================================================*\n");
		//	return WriteDump(pExceptionInfo);
		//}

		::printf("*============================================================*\n");
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void cMiniDump::DumpTest()
	{
		nMogi::cMiniDump::InitMiniDump();
		nMogi::cMiniDump::SetFolder("Exception");

		//assert(false && "Test");

		for (int i = 0; i < 5; ++i)
		{
			try
			{
			//	int* p = nullptr;
			//	*p = 1;
			}
			catch (nMogi::cException e)
			{
				printf("Caught a __try exception with SE_Exception.\n");
				printf("nSE = 0x%x\n", e.GetErrorCode());
			}
			Sleep(1000);
		}

	}

}