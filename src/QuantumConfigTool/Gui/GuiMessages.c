#include "Gui/GuiMessages.h"

#define MAX_STRING 60

static TCHAR g_szErrorText[MAX_STRING];
static TCHAR g_szInformationText[MAX_STRING];
static TCHAR g_szBoostIndexQLengthText[MAX_STRING];
static TCHAR g_szNewQText[MAX_STRING];

void LoadResourceString(HINSTANCE hInst, UINT uId, PTCHAR buffer, int cBufferLength)
{
	int cb = LoadString(hInst, uId, buffer, cBufferLength);
	if (!cb)
	{
		MessageBox(NULL, TEXT("Failed to load application resources"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
}

void InitMessages(HINSTANCE hInst)
{
	LoadResourceString(hInst, IDS_ERROR, g_szErrorText, sizeof(g_szErrorText));
	LoadResourceString(hInst, IDS_INFORMATION,g_szInformationText, sizeof(g_szInformationText));
	LoadResourceString(hInst, IDS_BOOSTINDEXQLENGTH, g_szBoostIndexQLengthText, sizeof(g_szBoostIndexQLengthText));
	LoadResourceString(hInst, IDS_NEWQUANTUMSETTINGS, g_szNewQText, sizeof(g_szNewQText));
}

void ShowSystemDecodedError(HWND hDlg, DWORD dwErrorCode)
{
	LCID langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
	PTCHAR message = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, langId,
				  (PTCHAR)&message, 0, NULL);
	MessageBox(hDlg, message, g_szErrorText, MB_OK | MB_ICONERROR); //FIX
	LocalFree(message);
};

void ShowError(HWND hDlg, LPCWSTR caption, LPCWSTR mesage)
{
	MessageBox(hDlg, mesage, caption, MB_OK | MB_ICONERROR);
}

void ShowNewConfiguration(HWND hDlg, struct CONFIG_INFORMATION ci)
{
	TCHAR pszDest[MAX_STRING * 4];
	unsigned short const *foregroundTable;
	if (ci.qvVal == QUANTUM_VARIBLE_ENABLED ||
		(ci.qvVal == QUANTUM_VARIBLE_DEFAULT &&
		!IsWindowsServer()))
	{
		foregroundTable = &VaribleQuantumsTable[0];
	} else
	{
		foregroundTable = &FixedQuantumsTable[0];
	}
	if (ci.cLength == QUANTUM_LENGTH_LONG ||
		(ci.cLength == QUANTUM_LENGTH_DEFAULT &&
		IsWindowsServer()))
	{
		foregroundTable = foregroundTable + 3;
	}
	LPCTSTR psHeader = g_szNewQText;
	PCTSTR psFormat = g_szBoostIndexQLengthText;
	lstrcpy(pszDest, psHeader);
	for (unsigned short i = 0; i <= ci.unRaw; i++)
	{
		TCHAR pszTmp[MAX_STRING];
		wsprintf(pszTmp, psFormat, i, *(foregroundTable + i));
		lstrcat(pszDest, pszTmp);
	}
	MessageBox(hDlg, pszDest, g_szInformationText, MB_OK | MB_ICONINFORMATION);
}