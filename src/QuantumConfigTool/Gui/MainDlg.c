#include "framework.h"
#include "Config/QuantumConfig.h"
#include "GuiMessages.h"
#include "Resources/resource.h"
#include "MainDlg.h"


static struct CONFIG_INFORMATION g_ci;

struct PRIORITY_SEPARATION_LEVEL
{
	int iIndex;
	LPCTSTR szItem;
};

static struct PRIORITY_SEPARATION_LEVEL g_pslvlTable[3] =
{
	{0, TEXT("0")},
	{1, TEXT("1")},
	{2, TEXT("2")},
};

BOOL InitMainDlg(_In_ HINSTANCE hInst,
				 _In_opt_ HINSTANCE hPrevInstance,
				 _In_ LPTSTR    lpCmdLine,
				 _In_ int       nCmdShow)
{
	HWND hDlgWnd = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_MAIN_DIALOG), 0, (DLGPROC)MainDialogProc, 0);
	if (!hDlgWnd)
	{
		return FALSE;
	}
	InitMessages(hInst, hDlgWnd);
	ShowWindow(hDlgWnd, nCmdShow);
	return TRUE;
}

int EnterMessageLoop()
{
	MSG msg;
	BOOL ret;
	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (ret == -1)
			return -1;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			HANDLE hIcon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDI_ICONMAIN), IMAGE_ICON, 0, 0, 0);
			SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG)lParam);
			HWND hPrioritySeparationComboBox = GetDlgItem(hDlg, IDC_PRIORITYSEPARATIONCOMBO);
			for (int i = 0; i < 3; i++)
			{
				SendMessage(hPrioritySeparationComboBox, CB_ADDSTRING, g_pslvlTable[i].iIndex, (LPARAM)(g_pslvlTable[i].szItem));
			}
			SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(ID_RELOAD, BN_CLICKED), (LPARAM)GetDlgItem(hDlg, IDC_PRIORITYSEPARATIONCOMBO));
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDC_PRIORITYSEPARATIONCOMBO:
					if (HIWORD(wParam) == CBN_SELCHANGE)
					{
						g_ci.unRaw = (unsigned short)SendMessage(GetDlgItem(hDlg, IDC_PRIORITYSEPARATIONCOMBO), CB_GETCURSEL, 0, 0);
					}
					return TRUE;
				case ID_SAVE:
				{
					DWORD dwErrorCode;
					if (!SetConfig(&g_ci, &dwErrorCode))
					{
						ShowSystemDecodedError(dwErrorCode);
					} else
					{
						ShowNewConfiguration(g_ci);
					}
					return TRUE;
				}
				case IDC_SHORTQUANTUMSRADIO:
					g_ci.cLength = QUANTUM_LENGTH_SHORT;
					return TRUE;
				case IDC_LONGQUANTUMSRADIO:
					g_ci.cLength = QUANTUM_LENGTH_LONG;
					return TRUE;
				case IDC_DEFAULTQUANTUMSRADIO:
					g_ci.cLength = QUANTUM_LENGTH_DEFAULT;
					return TRUE;
				case IDC_VARIBLEQUANTUMSENABLEDRADIO:
					g_ci.qvVal = QUANTUM_VARIBLE_ENABLED;
					return TRUE;
				case IDC_VARIBLEQUANTUMSDISABLEDRADIO:
					g_ci.qvVal = QUANTUM_VARIBLE_DISABLED;
					return TRUE;
				case IDC_VARIBLEQUANTUMSDEFAULTRADIO:
					g_ci.qvVal = QUANTUM_VARIBLE_DEFAULT;
					return TRUE;
				case ID_RELOAD:
				{
					int iQuantumsLengthComboBoxId;
					int iVaribleQuantumsComboBoxId;
					DWORD dwErrorCode;
					struct CONFIG_INFORMATION configInfo;
					if (!GetConfig(&configInfo, &dwErrorCode))
					{
						ShowSystemDecodedError(dwErrorCode);
						return TRUE;
					}
					switch (configInfo.cLength)
					{
						case QUANTUM_LENGTH_SHORT:
							iQuantumsLengthComboBoxId = IDC_SHORTQUANTUMSRADIO;
							break;
						case QUANTUM_LENGTH_LONG:
							iQuantumsLengthComboBoxId = IDC_LONGQUANTUMSRADIO;
							break;
						default:
							iQuantumsLengthComboBoxId = IDC_DEFAULTQUANTUMSRADIO;
							break;
					}
					switch (configInfo.qvVal)
					{
						case QUANTUM_VARIBLE_ENABLED:
							iVaribleQuantumsComboBoxId = IDC_VARIBLEQUANTUMSENABLEDRADIO;
							break;
						case QUANTUM_VARIBLE_DISABLED:
							iVaribleQuantumsComboBoxId = IDC_VARIBLEQUANTUMSDISABLEDRADIO;
							break;
						default:
							iVaribleQuantumsComboBoxId = IDC_VARIBLEQUANTUMSDEFAULTRADIO;
							break;
					}

					CheckRadioButton(hDlg, IDC_SHORTQUANTUMSRADIO, IDC_DEFAULTQUANTUMSRADIO, iQuantumsLengthComboBoxId);
					SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(iQuantumsLengthComboBoxId, BN_CLICKED), (LPARAM)(GetDlgItem(hDlg, iQuantumsLengthComboBoxId)));
					CheckRadioButton(hDlg, IDC_VARIBLEQUANTUMSENABLEDRADIO, IDC_VARIBLEQUANTUMSDEFAULTRADIO, iVaribleQuantumsComboBoxId);
					SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(iVaribleQuantumsComboBoxId, BN_CLICKED), (LPARAM)(GetDlgItem(hDlg, iVaribleQuantumsComboBoxId)));
					HWND hPrioritySeparationComboBox = GetDlgItem(hDlg, IDC_PRIORITYSEPARATIONCOMBO);
					SendMessage(hPrioritySeparationComboBox, CB_SETCURSEL, configInfo.unRaw, 0);
					SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDC_PRIORITYSEPARATIONCOMBO, CBN_SELCHANGE), (LPARAM)hPrioritySeparationComboBox);
					return TRUE;
				}
				default:
					break;
			}
			return TRUE;
		}
		case WM_CLOSE:
			DestroyWindow(hDlg);
			return TRUE;
		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;
	}
	return FALSE;
}






