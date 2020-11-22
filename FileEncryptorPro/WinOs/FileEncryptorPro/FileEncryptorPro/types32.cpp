#include"stdafx.h"
#include"types32.h"

UINT8_T hostIsBigEndian()
{
	static UINT8_T hadCmp = 0;
	static UINT8_T isBig = 0;
	if (hadCmp == 0)
	{
		hadCmp = 1;

		UINT32_T pnum = 0x01020304;
		UINT8_T low = pnum & 0x0ff;
		UINT8_T * cpnum = (UINT8_T*)(void *)&pnum;
		INT32_T nsize = sizeof(UINT32_T);
		if (cpnum[nsize - 1] == low)
		{
			isBig = 1;
		}
		else
		{
			isBig = 0;
		}
	}
	return isBig;
}
UINT8_T hostIsLittleEndian()
{
	static UINT8_T hadCmp = 0;
	static UINT8_T isLittle = 0;
	if (hadCmp == 0)
	{
		hadCmp = 1;

		UINT32_T pnum = 0x01020304;
		UINT8_T low = pnum & 0x0ff;
		UINT8_T * cpnum = (UINT8_T*)(void *)&pnum;
		INT32_T nsize = sizeof(UINT32_T);
		if (cpnum[0] == low)
		{
			isLittle = 1;
		}
		else
		{
			isLittle = 0;
		}
	}
	return isLittle;
}
void * swapEndianSort(void * val, INT32_T size)
{
	UINT8_T * cpval = (UINT8_T *)val;
	for (INT32_T i = 0; i < size / 2; i++)
	{
		/*UINT8_T tmp = cpval[i];
		cpval[i] = cpval[size - 1 - i];
		cpval[size - 1 - i] = tmp;*/

		//equals code method
		cpval[i] = cpval[i] ^ cpval[size - 1 - i];
		cpval[size - 1 - i] = cpval[i] ^ cpval[size - 1 - i];
		cpval[i] = cpval[i] ^ cpval[size - 1 - i];
	}

	return val;
}
void * host2BigEndian(void * val, INT32_T size)
{
	if (hostIsBigEndian())
		return val;

	return swapEndianSort(val, size);
}

void * host2LittleEndian(void * val, INT32_T size)
{
	if (hostIsLittleEndian())
		return val;

	return swapEndianSort(val, size);
}


void * bigEndian2Host(void * val, INT32_T size)
{
	if (hostIsBigEndian())
		return val;
	return swapEndianSort(val, size);
}
void * littleEndian2Host(void * val, INT32_T size)
{
	if (hostIsLittleEndian())
		return val;
	return swapEndianSort(val, size);
}