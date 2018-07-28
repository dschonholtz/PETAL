#include "ApplicationLauncher.h"
#include <windows.h>

public ref class ApplicationLauncher
{

	ApplicationLauncher::ApplicationLauncher()
	{
		// Eventually we will have to add in some modular code here to allow differing apps 
		// to be launched easily and interchangably
	}


	VOID startupOptiKey()
	{
		// additional information
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// start the program up
		CreateProcess(L"C:\Program Files (x86)\OptiKey\OptiKey.exe",   // the path
			NULL,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		);
		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

};
