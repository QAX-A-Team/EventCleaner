#pragma once

#include "windows.h"
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <aclapi.h>
#include "GetFileHandle.h"
#include <winevt.h>

#pragma comment(lib,"wevtapi.lib")

using namespace std;
typedef long NTSTATUS;


typedef struct _THREAD_BASIC_INFORMATION
{
	NTSTATUS                ExitStatus;
	PVOID                   TebBaseAddress;
	int						ClientId;
	KAFFINITY               AffinityMask;
	int						Priority;
	int						BasePriority;
	int						v;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;



typedef enum _SC_SERVICE_TAG_QUERY_TYPE
{
	ServiceNameFromTagInformation = 1,
	ServiceNameReferencingModuleInformation,
	ServiceNameTagMappingInformation,
} SC_SERVICE_TAG_QUERY_TYPE, *PSC_SERVICE_TAG_QUERY_TYPE;

typedef struct _SC_SERVICE_TAG_QUERY
{
	ULONG   processId;
	ULONG   serviceTag;
	ULONG   reserved;
	PVOID   pBuffer;
} SC_SERVICE_TAG_QUERY, *PSC_SERVICE_TAG_QUERY;


typedef struct _MIMI_CLIENT_ID {
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENTID;


typedef NTSTATUS(WINAPI* NtQueryInformationThread)(HANDLE, THREAD_INFORMATION_CLASS, PVOID, ULONG, PULONG);
typedef ULONG(WINAPI* I_QueryTagInformation)(PVOID, SC_SERVICE_TAG_QUERY_TYPE, PSC_SERVICE_TAG_QUERY);
typedef NTSTATUS(WINAPI * PRtlCreateUserThread)(HANDLE, PSECURITY_DESCRIPTOR, BOOL, ULONG, SIZE_T, SIZE_T, PTHREAD_START_ROUTINE, PVOID, PHANDLE, CLIENTID*);
typedef BOOL(*pFuncSwitch)(vector<INT>& threads);


BOOL fn_check_process_priv();
BOOL fn_parse_param();
BOOL fn_adjust_token_privilege(HANDLE& hNewThreadToken);
DWORD fn_seek_logservice_pid();
DWORD fn_enum_process_module(DWORD dwProcessId);
BOOL fn_enum_process_thread(DWORD dwProcessId, vector<INT>& threads);
BOOL fn_query_thread_information(DWORD dwProcessId, HANDLE hThread);
BOOL fn_get_service_name(DWORD dwProcessId, ULONG tag);
BOOL fn_suspend_threads(vector<INT>& threads);
BOOL fn_recover_threads(vector<INT>& threads);
BOOL fn_check_file_state();
BOOL fn_migrate_logservice_proc(DWORD dwProcessId);
BOOL fn_receive_unlocker_signal(DWORD dwHandleValue);
BOOL fn_closehandle(vector<INT>& eventid);
BOOL fn_delete_eventid(vector<INT>& eventid);
BOOL fn_recover_handle();
VOID fn_stop_dependent_services();
VOID fn_create_security_attributes(SECURITY_ATTRIBUTES& sa);

