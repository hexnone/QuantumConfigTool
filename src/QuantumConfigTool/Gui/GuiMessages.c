#include "framework.h"
#include <VersionHelpers.h>
#include "Gui/GuiMessages.h"
#include "Config/QuantumConfig.h"
#include "Resources/resource.h"
#include "RtlSupport/RtlSupportTypes.h"

#define MAX_STRING 60

static TCHAR g_szErrorText[MAX_STRING];
static TCHAR g_szInformationText[MAX_STRING];
static TCHAR g_szBoostIndexQLengthText[MAX_STRING];
static TCHAR g_szNewQText[MAX_STRING];

static HINSTANCE g_hInst;
static HWND g_hDlg;

void LoadResourceString(UINT uId, PTCHAR buffer, int cBufferLength)
{
	int cb = LoadString(g_hInst, uId, buffer, cBufferLength);
	if (!cb)
	{
		MessageBox(NULL, TEXT("Failed to load application resources"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
}

void InitMessages(HINSTANCE hInst, HWND hDlg)
{
	g_hInst = hInst;
	g_hDlg = hDlg;
	LoadResourceString(IDS_ERROR, g_szErrorText, sizeof(g_szErrorText));
	LoadResourceString(IDS_INFORMATION,g_szInformationText, sizeof(g_szInformationText));
	LoadResourceString(IDS_BOOSTINDEXQLENGTH, g_szBoostIndexQLengthText, sizeof(g_szBoostIndexQLengthText));
	LoadResourceString(IDS_NEWQUANTUMSETTINGS, g_szNewQText, sizeof(g_szNewQText));
}

void ShowSystemDecodedError(DWORD dwErrorCode)
{
	LCID langId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
	PTCHAR message = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, langId,
				  (PTCHAR)&message, 0, NULL);
	MessageBox(g_hDlg, message, g_szErrorText, MB_OK | MB_ICONERROR);
	LocalFree(message);
};

void ShowNewConfiguration(struct CONFIG_INFORMATION ci)
{
	TCHAR pszDest[150];
	unsigned short const (*foregroundTable);
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
		TCHAR pszTmp[50];
		wsprintf(pszTmp, psFormat, i, *(foregroundTable + i));
		lstrcat(pszDest, pszTmp);
	}
	MessageBox(g_hDlg, pszDest, g_szInformationText, MB_OK | MB_ICONINFORMATION);
}