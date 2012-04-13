#ifndef H_DRIVER8LOADER_H_
# define H_DRIVER8LOADER_H_
# if !defined (NULL)
#  define		NULL		((void*)0)
# endif

int		loader_LoadDriver(const char* driverName, const char* driverFileName);
int		loader_UnloadDriver(const char* driverName);

#endif
