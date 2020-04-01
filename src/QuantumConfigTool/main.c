#include "framework.h"
#include "Gui/MainDlg.h"

extern IMAGE_DOS_HEADER __ImageBase;


WORD GetShowWindowMode()
{
	STARTUPINFOW startup_info;
	GetStartupInfoW(&startup_info);

	return startup_info.dwFlags & STARTF_USESHOWWINDOW
		? startup_info.wShowWindow
		: SW_SHOWDEFAULT;
}

int main()
{
	int iExitCode;
	BOOL bIsWow64;
	HINSTANCE hInst = (HINSTANCE)(&__ImageBase);
	IsWow64Process(GetCurrentProcess(), &bIsWow64);
	InitMessages(hInst);
	if (!InitMainDlg(hInst,
		0,
		0,
		GetShowWindowMode()))
	{
		iExitCode = -1;
	} else
	{
		iExitCode = EnterMessageLoop();
	}
	ExitProcess(iExitCode);
	return iExitCode;
}
