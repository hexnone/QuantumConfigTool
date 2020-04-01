#pragma once
#include "framework.h"
#include <stdbool.h>

#define HITWBITS(value)	(((value >> 4) & 0x3))
#define MLTWBITS(value)	((value >> 2) & 0x3)
#define LOTWBITS(value)	((value) & 0x3)

#define MKHITWBITS(value) ((value & 0x3) << 4)
#define MKMLTWBITS(value) ((value & 0x3) << 2)
#define MKLOTWBITS(value) (value & 0x3)

#define CONTROL_KEY HKEY_LOCAL_MACHINE 
#define REG_CONTROL_SUBKEY TEXT("SYSTEM\\CurrentControlSet\\Control\\PriorityControl")
#define CONTROL_VALUE_NAME TEXT("Win32PrioritySeparation")

static unsigned short VaribleQuantumsTable[6] = { 6, 12, 18, 12, 24, 36 };
static unsigned short FixedQuantumsTable[6] = { 18, 18, 18, 36, 36, 36 };

enum QuantumLength
{
	QUANTUM_LENGTH_SHORT, QUANTUM_LENGTH_LONG, QUANTUM_LENGTH_DEFAULT
};
enum QuantumVaribleOption
{
	QUANTUM_VARIBLE_ENABLED, QUANTUM_VARIBLE_DISABLED, QUANTUM_VARIBLE_DEFAULT
};

struct CONFIG_INFORMATION
{
	unsigned short unRaw;
	enum QuantumLength cLength;
	enum QuantumVaribleOption qvVal;
};

bool SetConfig(struct CONFIG_INFORMATION* ci, DWORD* dwErrorCode);
bool GetConfig(struct CONFIG_INFORMATION* ci, DWORD* dwErrorCode);

