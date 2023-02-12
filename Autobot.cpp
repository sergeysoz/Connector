#include <iostream>

// the order of including is important
#define WIN32_DEFAULT_LIBS

// SQLInstallDriver
// SQLConfigDriver
// SQLConfigDataSource

// The definition must precede #include <windows.h>
#define UNICODE
#define _UNICODE
#include <Windows.h>
#include <tchar.h>
//#include <conio.h>
//#include <stdarg.h>
#include <odbcinst.h>

//#include <WinNT.h>
#include <UserEnv.h>

using namespace std;

// sizeof(TCHAR)

#ifdef UNICODE
#define TCHAR WCHAR
#else
#define TCHAR CHAR
#endif

//typedef wchar_t* LPWSTR, * PWSTR;

int main()
{
	SetConsoleTitle(L"UTM Connector 1.0");
	// 

	/*enum requests
	{
		ODBC_ADD_DSN = 0,
		ODBC_CONFIG_DSN = 1,
		ODBC_REMOVE_DSN = 2,

	};*/

	WCHAR path[MAX_PATH];
	::GetSystemDirectory(path, MAX_PATH);
	printf("System directory: %ws\n", path);

	/*TCHAR path[MAX_PATH];
	DWORD Length;
	Length = ARRAYSIZE(path);
	::GetUserProfileDirectoryW(
		nullptr,
		path,
		&Length
	);
	printf("System directory: %ws\n", path);*/

	// ERROR CHECKS
	RETCODE rc;

	LPCTSTR hDriver = L"Excel Files (*.xls)";
	LPCTSTR hAttributes = L"DSN=New Excel Data Source\0"
		L"Description=New Excel Data Source\0"
		L"FileType = Excel\0"
		L"DataDirectory=D:\\EXCELDIR\0"
		L"MaxScanRows=20\0\0";

	cout << "The connection has started\n";

	// We pass the NULL window handle 
	// to manage a data source silently 
	BOOL isConfigurated = ::SQLConfigDataSource(
		NULL,
		ODBC_ADD_DSN,       // ODBC_ADD_SYS_DSN
		hDriver,
		hAttributes
	);

	if (isConfigurated)
	{
		cout << "SUCCESS: The connection has configurated\n";
		//system("pause");
		return 0;
	}

	WORD iError, cbErrorMsg;
	DWORD fErrorCode;

	// LPWSTR buffer[1024];
	// LPWSTR - pointer to LPSTR
	// LPSTR - character type
	LPWSTR buffer2 = (LPWSTR)malloc(512 * sizeof(LPSTR));
	//printf("%s\n", pszOperation);
	for (int iError = 1; iError <= 8; iError++)
	{
		rc = ::SQLInstallerError(
			iError,
			&fErrorCode,
			//(LPWSTR)&buffer,
			buffer2,
			512 * sizeof(LPSTR),
			&cbErrorMsg
		);
		if (rc == SQL_NO_DATA || rc == SQL_ERROR)
		{
			break;
		}
		printf("%d: Error code = %d, Message = %ws\n",
			iError,
			fErrorCode,
			buffer2
		);
		//cout << fErrorCode << ": " << *buffer << endl;
	}

	//system("pause");
	return 1;
}

// Разработка программного обеспечения для удаленного доступа к базам данных. 

// Mode: debug x86
// C++/General/Additional Include Dependencies: C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\include;$(SolutionDir)Dependencies\ODBC\include;<different options>
// Linker/Input/Additional Denepdencies: odbccp32.lib;legacy_stdio_definitions.lib;User32.Lib
// Library Source: C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0

// ODBC
// Data Source
// Connection String
// The application identifies the target driver and data source with a CONNECTION HANDLE.
// Driver connects/disconnects to the Data Source.

// Call -> Driver Manager -> Driver -> Data Source

// Driver acts as a Client
// Data Source acts as a Server

// The purpose of the data source is to gather all 
// of the technical information needed to access the data 
// the driver name, network address, network software, etc.
// into a single place and hide it from the user.

// Information about data sources stored in the system registry.

// ODBC Data Source Administrator

// SQLManageDataSource
// SQLCreateDataSource
// SQLConfigDataSource

// ODBC Data Source - the database that user wants to access 
// and its associated operating system, DBMS and network protocol (if any).