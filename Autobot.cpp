#include <iostream>

// the order of including is important
#define WIN32_DEFAULT_LIBS

#undef _WIN32
#if !defined(_WIN32)
#error _WIN32 must be defined   // C1189
#endif

// The definition must precede #include <windows.h>
#define UNICODE
#define _UNICODE
#define BUILD_WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <odbcinst.h>
#include <UserEnv.h>

using namespace std;

// sizeof(TCHAR):
#ifdef UNICODE
#define TCHAR WCHAR
#else
#define TCHAR CHAR
#endif

int _tmain()
{
	SetConsoleTitle(L"UTM Configurator 1.0");

	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return GetLastError();
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return GetLastError();
	}

	LPCTSTR hDriver = L"Excel Files (*.xls)";
	LPCTSTR hAttributes = L"DSN=New Excel Data Source\0"
		L"Description=New Excel Data Source\0"
		L"FileType = Excel\0"
		L"DataDirectory=D:\\EXCELDIR\0"
		L"MaxScanRows=20\0\0";

	wprintf(L"Process started . . .\n\n");

	// We pass the NULL window handle 
	// to manage a data source silently 
	BOOL isConfigurated = ::SQLConfigDataSource(
		NULL,
		ODBC_ADD_DSN,			// ODBC_ADD_SYS_DSN
		(const WCHAR*)hDriver,	// Is there need to CAST?
		hAttributes
	);

	if (isConfigurated)
	{
		wprintf(L"\x1b[32mSUCCESS: the connection has been established and configured.\x1b[39;49m\n");
		system("pause");
		return 0;
	}

	WORD iError, cbErrorMsg;
	DWORD fErrorCode;

	RETCODE rc;
	for (int iError = 1; iError <= 8; ++iError)
	{
		// absollutely correct:
		// and mutch for 32 and 64 bit compiling
		LPWSTR buffer = (LPWSTR)malloc(SQL_MAX_MESSAGE_LENGTH * sizeof(LPSTR));
		rc = ::SQLInstallerErrorW(
			iError,
			&fErrorCode,
			buffer,
			SQL_MAX_MESSAGE_LENGTH - sizeof(LPSTR),
			&cbErrorMsg
		);
		if (rc == SQL_NO_DATA || rc == SQL_ERROR)
		{
			break;
		}
		wprintf(L"\x1b[31m%d: ERROR %d: %ws\x1b[39;49m\n\n",
			iError,
			fErrorCode,
			buffer
		);
		free(buffer);
	}

	wprintf(L"\x1b[31mFAIL: The connection has not been established.\x1b[39;49m\n\n");

	system("pause");
	return 0;
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

// Driver is an implementation of an interface.

// We need the following tasks:
// [*] Selecting a data source and connecting to it.
// [*] Process errors

// The Driver Manager exists mainly as a convenience to application writersand 
// solves a number of problems common to all applications. 
// These include determining which driver to load based on a DATA SOURCE NAME, 
// loadingand unloading drivers, and calling functions in drivers.

// Specific tasks performed by drivers include:
// [*] Connecting and disconnecting from the data source.
// [*] Checking for function errors not checked by the Driver Manager
// [*] Submitting SQL statements to the data source for execution.
// [*] Sending data to and retrieving data from the data source, 
//     including converting data types as specified by the application

// A DATA SOURCE is simply the source of the data. It can be a file,
// a particular database on a DBMS, or even a live data feed. 
// The purpose of the data source is to gather all of the technical
// information needed to access the data - the driver name, network address, 
// network software, etc. - into a single place and hide it from the user. 

// Machine data sources are stored on the system with a user-defined name.
// Associated with the data source name is all of the information the
// Driver Manager and driver need to connect to the data source.

// [*] name of the driver
// [*] server where the DBMS resides
// 
// CONNECTION STRING
// WE NEED TO CREATE A DATA SOURCE
// Data sources usually created by the end user or a technician 
// with a program called the ODBC Administrator
// 
// The ODBC Administrator prompts the user for the driver to use
// and then calls that driver.

// The driver displays a dialog box that requests the information
// it needs to connect to the data source.

// After the user enters the information, the driver stores it 
// on the sustem.

// Later, the application calls the Driver Manager and passes it the 
// name of a machine data source or the path of a file containing 
// a file data source.

// When passed a machine data source name, the Driver Manager searches 
// the system to find the driver used by the data source.

// It then loads the driver and passes the data source name to it.

// The driver uses the data source name to find the information
// it needs to connect to the data source.

// Finally, it connects to the data source, typically prompting
// the user for a user ID and password, which generally are not stored

// ----
// On computers running Microsoft, machine data source information
// is stored in the registry.

// Depending on which registry key the information is stored under,
// the data source is knows as a USER DATA SOURCE
// or a SYSTEM DATA SOURCE.

// User data sources are stored under the HKEY_CURRENT_USER key
// and are available noly to the current user.

// System data sources are stored under the HKEY_LOCAL_MACHINE key
// and can be used by more than one user on one machine.

// DSN - data source name.

// HKEY_CURRENT_USER / SOFTWARE / ODBC / Odbc.ini / ODBC Data Sources / ...

// 
