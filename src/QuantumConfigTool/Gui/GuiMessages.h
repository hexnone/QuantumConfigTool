#pragma once
#include "framework.h"
#include <VersionHelpers.h>
#include "Config/QuantumConfig.h"
#include "Resources/resource.h"

void ShowSystemDecodedError(HWND hDlg, DWORD dwErrorCode);
void ShowError(HWND hDlg, LPCWSTR caption, LPCWSTR mesage);
void ShowNewConfiguration(HWND hDlg, struct CONFIG_INFORMATION ci);
void InitMessages(HINSTANCE hInst);