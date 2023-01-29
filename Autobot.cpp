#include <iostream>

// the order of including is important
#define WIN32_DEFAULT_LIBS

#include <Windows.h>

#include <stdio.h>
//#include <conio.h>
//#include <stdarg.h>

#include <odbcinst.h>

//#include <WinNT.h>

int main()
{
	SetConsoleTitle(L"UTM Connector 1.0");

	LPCTSTR hDriver = L"Excel Files (*.xls)";
	LPCTSTR hAttributes = L"DSN=New Excel Data Source\0"
		L"Description=New Excel Data Source\0"
		L"FileType = Excel\0"
		L"DataDirectory=D:\\EXCELDIR\0"
		L"MaxScanRows=20\0";
	
	std::cout << "The connection has started\n";

	BOOL isConfigurated = SQLConfigDataSource(
		NULL,
		ODBC_ADD_DSN,       // ODBC_ADD_SYS_DSN
		hDriver,
		hAttributes
	);

	if (isConfigurated)
	{
		std::cout << "SUCCESS: The connection has configurated\n";
		system("pause");
		return 0;
	}

	std::cout << "FAIL: The connection has not configurated\n";
	system("pause");
	return 1;
}

// Mode: debug x86
// C++/General/Additional Include Dependencies: C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.34.31933\include;$(SolutionDir)Dependencies\ODBC\include;<different options>
// Linker/Input/Additional Denepdencies: odbccp32.lib;legacy_stdio_definitions.lib;User32.Lib
// Library Source: C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0