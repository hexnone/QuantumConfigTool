#pragma once
#include "Resources/resource.h"
#include "Config/QuantumConfig.h"

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL InitMainDlg(_In_ HINSTANCE g_hInst,
		  _In_opt_ HINSTANCE hPrevInstance,
		  _In_ LPTSTR    lpCmdLine,
		  _In_ int       m_nCmdShow);
int EnterMessageLoop();

