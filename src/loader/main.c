#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "loader.h"

int			__cdecl main(int argc, char** argv)
{
	int		retVal = EXIT_SUCCESS;
	
	if (argc < 3) {
		printf("Not enought arguments: loader.exe load driver_file.sys driverName\n"
				"\t\t       loadex.exe unload driver_name\n");
		return (EXIT_FAILURE);
	}
	
	if (_stricmp(argv[1], "LOAD") == 0) {
		if (argc < 4) {
			printf("Not enought arguments: loader.exe load driver_file.sys driverName\n");
			return (EXIT_FAILURE);
		}
		if ((retVal = loader_LoadDriver(argv[3], argv[2])) == 0)
			printf("DriverName    : %s\nDriverFileName: %s\nStatus        : Registered and started with success !\n", argv[3], argv[2]);
	} else if (_stricmp(argv[1], "UNLOAD") == 0) {
		if ((retVal = loader_UnloadDriver(argv[2])) == 0)
			printf("DriverName    : %s\nStatus        : Unregistered with success !\n", argv[2]);
	} else {
		printf("Unknow Command switch: loader.exe (load | unload) driver_file.sys\n");
		return (EXIT_FAILURE);
	}
	
	return (retVal);
}
