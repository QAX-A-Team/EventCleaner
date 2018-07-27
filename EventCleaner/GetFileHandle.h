#pragma once


#include <windows.h>
#include <iostream>
#include <algorithm>

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

using namespace std;

//-----------------------------------------------------------------------------
// Query system information

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation,                 // 0x00 SYSTEM_BASIC_INFORMATION
	SystemProcessorInformation,             // 0x01 SYSTEM_PROCESSOR_INFORMATION
	SystemPerformanceInformation,           // 0x02
	SystemTimeOfDayInformation,             // 0x03
	SystemPathInformation,                  // 0x04
	SystemProcessInformation,               // 0x05
	SystemCallCountInformation,             // 0x06
	SystemDeviceInformation,                // 0x07
	SystemProcessorPerformanceInformation,  // 0x08
	SystemFlagsInformation,                 // 0x09
	SystemCallTimeInformation,              // 0x0A
	SystemModuleInformation,                // 0x0B SYSTEM_MODULE_INFORMATION
	SystemLocksInformation,                 // 0x0C
	SystemStackTraceInformation,            // 0x0D
	SystemPagedPoolInformation,             // 0x0E
	SystemNonPagedPoolInformation,          // 0x0F
	SystemHandleInformation,                // 0x10


} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;




typedef struct _SYSTEM_HANDLE
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;


typedef struct _IO_STATUS_BLOCK
{
	union
	{
		NTSTATUS Status;
		PVOID Pointer;
	};

	ULONG_PTR Information;

} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;


typedef struct _FILE_NAME_INFORMATION {
	ULONG FileNameLength;
	WCHAR FileName[MAX_PATH];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;


typedef enum _FILE_INFORMATION_CLASS
{
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,   // 2
	FileBothDirectoryInformation,   // 3
	FileBasicInformation,           // 4  wdm
	FileStandardInformation,        // 5  wdm
	FileInternalInformation,        // 6
	FileEaInformation,              // 7
	FileAccessInformation,          // 8
	FileNameInformation,            // 9
	FileRenameInformation,          // 10
	FileLinkInformation,            // 11
	FileNamesInformation,           // 12
	FileDispositionInformation,     // 13
	FilePositionInformation,        // 14 wdm
	FileFullEaInformation,          // 15
	FileModeInformation,            // 16
	FileAlignmentInformation,       // 17
	FileAllInformation,             // 18
	FileAllocationInformation,      // 19
	FileEndOfFileInformation,       // 20 wdm
	FileAlternateNameInformation,   // 21
	FileStreamInformation,          // 22
	FilePipeInformation,            // 23
	FilePipeLocalInformation,       // 24
	FilePipeRemoteInformation,      // 25
	FileMailslotQueryInformation,   // 26
	FileMailslotSetInformation,     // 27
	FileCompressionInformation,     // 28
	FileObjectIdInformation,        // 29
	FileCompletionInformation,      // 30
	FileMoveClusterInformation,     // 31
	FileQuotaInformation,           // 32
	FileReparsePointInformation,    // 33
	FileNetworkOpenInformation,     // 34
	FileAttributeTagInformation,    // 35
	FileTrackingInformation,        // 36
	FileIdBothDirectoryInformation, // 37
	FileIdFullDirectoryInformation, // 38
	FileValidDataLengthInformation, // 39
	FileShortNameInformation,       // 40
	FileIoCompletionNotificationInformation, // 41
	FileIoStatusBlockRangeInformation,       // 42
	FileIoPriorityHintInformation,           // 43
	FileSfioReserveInformation,              // 44
	FileSfioVolumeInformation,               // 45
	FileHardLinkInformation,                 // 46
	FileProcessIdsUsingFileInformation,      // 47
	FileMaximumInformation                   // 48
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;


typedef NTSTATUS(NTAPI *NtQuerySystemInformation)(
	ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength);

typedef NTSTATUS(NTAPI *NtQueryInformationFile)(
	IN HANDLE FileHandle,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass);



LPVOID fn_get_system_handle();
DWORD WINAPI fn_get_filename_threadproc(LPVOID lpParam);
BOOL fn_get_filename_byhandle(HANDLE hFile, LPSTR lpAscStr);
HANDLE fn_trav_proc_handle(DWORD dwProcessId);
