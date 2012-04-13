#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "loader.h"

int		loader_LoadDriver(const char* driverName, const char* driverFileName)
{
	SC_HANDLE	hSCManager = NULL;
	SC_HANDLE	hService = NULL;
	int			retVal = 0;
	
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		printf("loader_LoadDriver::OpenSCManager::Unable_to_Open\n");
		return (-1);
	}
	hService = CreateService(hSCManager,
							driverName,				/* Service Name */
                            driverName,				/* Display Name */
                            SERVICE_ALL_ACCESS,		/* Desired Access */
                            SERVICE_KERNEL_DRIVER,	/* Service Type */
                            SERVICE_DEMAND_START,	/* Start Type */
                            SERVICE_ERROR_NORMAL,	/* Error Controle */
                            driverFileName,			/* Binary Path Name */
                            NULL, 					/* Load OrderGroup */
                            NULL, 					/* Tag Id */
                            NULL, 					/* Dependencies */
                            NULL, 					/* Service Start Name */
                            NULL); 					/* Password */
	
	if (!hService) {
		if (GetLastError() == ERROR_SERVICE_EXISTS) { /* Already exist, try to run it */
			printf("loader_LoadDriver::CreateService::Service_already_created\n");
			hService = OpenService(hSCManager, driverName, SERVICE_ALL_ACCESS);
			if (!hService) {
				printf("loader_LoadDriver::OpenService::Service_not_Found\n");
				CloseServiceHandle(hSCManager);
				return (-1);
			}
		} else {
			printf("loader_LoadDriver::CreateService::Error_Create_Service\n");
			retVal = -1;
		}
	} else {
		if (StartService(hService, 0, NULL) == 0) {
			if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
				printf("loader_LoadDriver::StartService::Already_Run");
			else {
				printf("loader_LoadDriver::StartService::Error");
				retVal = -1;
			}
		}
		CloseServiceHandle(hService);
	}
	
	CloseServiceHandle(hSCManager);
	return (retVal);
}

int		loader_UnloadDriver(const char* driverName)
{
	SERVICE_STATUS	ServiceStatus;
	SC_HANDLE		hSCManager = NULL;
	SC_HANDLE		hService = NULL;
	
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		printf("loader_UnloadDriver::OpenSCManager::Unable_to_Open\n");
		return (-1);
	}
	if (!(hService = OpenService(hSCManager, driverName, SERVICE_ALL_ACCESS))) {
		printf("loader_UnloadDriver::OpenService::Open_Service_Fail\n");
		CloseServiceHandle(hSCManager);
		return (-1);
	}
	if (!(ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus))) {
		printf("loader_UnloadDriver::ControlService::Stop_Fail_Maybe_Already_Stopped\n");
	}
	if (!(DeleteService(hService))) {	
		printf("loader_UnloadDriver::DeleteService::Unable_to_Delete\n");
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return (-1);
	}
	
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return (0);
}
