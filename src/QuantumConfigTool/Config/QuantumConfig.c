#include "QuantumConfig.h"

bool QueryRegValue(DWORD* value, DWORD* dwErrorCode)
{
	HKEY phWin32PrioritySeparation;
	DWORD dwValueType = REG_DWORD;
	DWORD bufferSize = sizeof(*value);
	*dwErrorCode = RegOpenKeyEx(CONTROL_KEY, REG_CONTROL_SUBKEY, 0, KEY_READ, &phWin32PrioritySeparation);
	if (*dwErrorCode != ERROR_SUCCESS)
	{
		RegCloseKey(phWin32PrioritySeparation);
		return false;;
	}
	*dwErrorCode = RegQueryValueEx(phWin32PrioritySeparation, CONTROL_VALUE_NAME, 0, &dwValueType, (LPBYTE)value, &bufferSize);
	if (*dwErrorCode != ERROR_SUCCESS)
		return false;
	RegCloseKey(phWin32PrioritySeparation);
	return true;
}

bool SetRegValue(DWORD* value, DWORD* dwErrorCode)
{
	HKEY phWin32PrioritySeparation;
	*dwErrorCode = RegOpenKeyEx(CONTROL_KEY, REG_CONTROL_SUBKEY, 0, KEY_WRITE, &phWin32PrioritySeparation);
	if (*dwErrorCode != ERROR_SUCCESS)
		return false;
	*dwErrorCode = RegSetValueEx(phWin32PrioritySeparation, CONTROL_VALUE_NAME, 0, REG_DWORD, (LPBYTE)value, sizeof(*value));
	if (*dwErrorCode != ERROR_SUCCESS)
	{
		RegCloseKey(phWin32PrioritySeparation);
		return false;
	}
	RegCloseKey(phWin32PrioritySeparation);
	return true;
}

bool SetConfig(struct CONFIG_INFORMATION* ci, DWORD* dwErrorCode)
{
	DWORD value = 0;
	switch ((*ci).cLength)
	{
		case QUANTUM_LENGTH_SHORT:
			value |= MKHITWBITS(0x2);
			break;
		case QUANTUM_LENGTH_LONG:
			value |= MKHITWBITS(0x1);
			break;
		case QUANTUM_VARIBLE_DEFAULT:
			value |= MKHITWBITS(0x0);
			break;
	}
	switch ((*ci).qvVal)
	{
		case QUANTUM_VARIBLE_ENABLED:
			value |= MKMLTWBITS(0x1);
			break;
		case QUANTUM_VARIBLE_DISABLED:
			value |= MKMLTWBITS(0x2);
			break;
		case QUANTUM_VARIBLE_DEFAULT:
			value |= MKMLTWBITS(0x0);
			break;
	}
	value |= MKLOTWBITS((*ci).unRaw);
	if (!SetRegValue(&value, dwErrorCode))
		return FALSE;
	return true;
}

bool GetConfig(struct CONFIG_INFORMATION* ci, DWORD* dwErrorCode)
{
	DWORD dwRegValue;
	if (!QueryRegValue(&dwRegValue, dwErrorCode))
		return false;

	switch (HITWBITS(dwRegValue))
	{
		case 0:
		case 3:
			(*ci).cLength = QUANTUM_VARIBLE_DEFAULT;
			break;
		case 1:
			(*ci).cLength = QUANTUM_LENGTH_LONG;
			break;
		case 2:
			(*ci).cLength = QUANTUM_LENGTH_SHORT;
			break;
	}
	switch (MLTWBITS(dwRegValue))
	{
		case 0:
		case 3:
			(*ci).qvVal = QUANTUM_VARIBLE_DEFAULT;
			break;
		case 1:
			(*ci).qvVal = QUANTUM_VARIBLE_ENABLED;
			break;
		case 2:
			(*ci).qvVal = QUANTUM_VARIBLE_DISABLED;
			break;
	}
	(*ci).unRaw = LOTWBITS(dwRegValue);
	return true;
}
