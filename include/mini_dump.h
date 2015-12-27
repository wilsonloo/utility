#ifdef _PF_WINDOWS_

#pragma once
#include <windows.h>
#include <DbgHelp.h>
#include <stdlib.h>
#include <boost/assert.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#pragma comment(lib, "dbghelp.lib")

#ifndef _M_IX86
#error "The following code only works for x86!"
#endif

namespace evl
{
	namespace utility
	{
			
		inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)
		{
			if (pModuleName == 0)
			{
				return FALSE;
			}

			WCHAR szFileName[_MAX_FNAME] = L"";
			_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

			if (_wcsicmp(szFileName, L"ntdll") == 0)
				return TRUE;

			return FALSE;
		}

		inline BOOL CALLBACK MiniDumpCallback(
			PVOID pParam,
			const PMINIDUMP_CALLBACK_INPUT   pInput,
			PMINIDUMP_CALLBACK_OUTPUT        pOutput)
		{
			if (pInput == 0 || pOutput == 0)
				return FALSE;

			switch (pInput->CallbackType)
			{
			case ModuleCallback:
				if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
					if (!IsDataSectionNeeded(pInput->Module.FullPath))
						pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
			case IncludeModuleCallback:
			case IncludeThreadCallback:
			case ThreadCallback:
			case ThreadExCallback:
				return TRUE;
			default:;
			}

			return FALSE;
		}

		inline void CreateMiniDump(PEXCEPTION_POINTERS pep, LPCTSTR strFileName)
		{
			HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
			{
				MINIDUMP_EXCEPTION_INFORMATION mdei;
				mdei.ThreadId = GetCurrentThreadId();
				mdei.ExceptionPointers = pep;
				mdei.ClientPointers = NULL;

				MINIDUMP_CALLBACK_INFORMATION mci;
				mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
				mci.CallbackParam = 0;


				::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, (pep != 0) ? &mdei : 0, NULL, &mci);

				CloseHandle(hFile);
			}
		}

		LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
		{
			std::string chars(
				"abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"1234567890");
			boost::random::random_device rng;
			boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
			std::string dump_file_name;
			for(int i = 0; i < 8; ++i) 
			{
				dump_file_name.append(chars.at(index_dist(rng)), 1);
			}
			dump_file_name.append(".dmp");

			CreateMiniDump(pExceptionInfo, dump_file_name.c_str());

			return EXCEPTION_EXECUTE_HANDLER;
		}

		// 此函数一旦成功调用，之后对 SetUnhandledExceptionFilter 的调用将无效
		void DisableSetUnhandledExceptionFilter()
		{
			void* addr = (void*)GetProcAddress(LoadLibrary("kernel32.dll"),
				"SetUnhandledExceptionFilter");

			if (addr)
			{
				unsigned char code[16];
				int size = 0;

				code[size++] = 0x33;
				code[size++] = 0xC0;
				code[size++] = 0xC2;
				code[size++] = 0x04;
				code[size++] = 0x00;

				DWORD dwOldFlag, dwTempFlag;
				VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
				WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
				VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
			}
		}

		void InitMinDump()
		{
			//注册异常处理函数
			SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

			//使SetUnhandledExceptionFilter
			DisableSetUnhandledExceptionFilter();
		}
	} // namespace utility
} // namespace evl

#else 

namespace evl
{
	namespace utility
	{
		void InitMinDump()
		{
			BOOST_ASSERT_MSG(false, "not implemented in this platform type.");
		}
	}
}

#endif 