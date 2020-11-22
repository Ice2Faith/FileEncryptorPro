#ifndef _EVERYTHING_LOCKER_HPP_
#define _EVERYTHING_LOCKER_HPP_
#include"types32.h"
/*
具有历史特征码的加密解密类
	其中，支持历史特征码进行参与加密,并且和密码具有一定的关系（也就是说，密码差一点儿不同，都会解密出基本完全错误的数据），这将会增加被反向解密的困难度，也就是更加安全
	有了历史特征码的存在，如果一段数据使用了历史特征码加密，那么想要解密，就必须拥有对应字节之前的历史特征码，这样才能解密成功
	也正是如此，如果两段数据，基本一致，只是多了一两个字节，那么如果这两个字节在前面，将会导致两段数据加密后完全不同，如果这两个自己在后面，那么只会影响这两个字节而已
	这也就是历史特征码的含义
	但是，其中有两个标志位进行控制
	历史特征码:
		采用了类似Hash的原理，此特征码将会记录和密码的值与长度相关的值
		特征码参数加密解密计算
		并且和被加密解密数据的长度也有一些关系【和数据的值没有关系】
continueLast：继续使用上一次加密之后的特征码，
	此位值为BOOL_TRUE时，适用于将一段数据分为许多小段数据进行加密的情况，比如【不断读取文件数据进行加密】
	（当然，如果你加密和解密时使用的段长度一致，这一个位设置与否，将对解密没有影响，仅仅对加密后的数据有影响，毕竟历史特征码不是闹着玩的）
cleanLast：此位为BOOL_TRUE时，直接清空历史特征码，
	也就是一个新的开始，并直接函数返回值为BOOL_FALSE,其他的参数值都是无效参数，不会被使用，因此会如下调用：
	lock(NULL, 0, NULL, 0, BOOL_FALSE, BOOL_TRUE);
你也可以看到，continueLast和cleanLast标志位都是有默认值的，为了什么呢？
	就是为了，大多数的时候，都是直接加密一段数据即可，不需要什么分段，不需要使用历史特征码，因此有了默认参数，你对一段数据进行处理的时候
	便可以不用管那两个标志位，直接使用即可
	【当然涉及到特殊要求，分段或者清除时，就需要注意了】
*/
class EverythingLocker
{
public:
	EverythingLocker()
	{
		m_innearKey = makeInearKey(&m_sizeInnearKey);
	}
	~EverythingLocker()
	{
		delete[] m_innearKey;
		m_innearKey = NULL;
		m_sizeInnearKey = 0;
	}
	/*
	函数简介（unlock函数也一致）：
		对sizeData字节大小的data数据用sizePass字节大小的pass密码加密
		参数说明：
			data:数据地址,你可以使用技巧将任意数据类型转换过来:(UINT8_T *)(void *)
			sizeData:数据大小（字节）,可以使用技巧：sizeof()
			pass:密码地址
			sizePass:密码大小(字节)
			continueLast:是否继续上一次执行后的特征码，因为会有历史特征码参与加密
			cleanLast:是否清楚上一次执行的特征码
		返回值：
			加密是否成功，当cleanLast标志位为BOOL_TRUE，则直接清除上一次执行并返回BOOL_FALSE
	*/
	BOOL_T lock(UINT8_T * data, INT32_T sizeData, UINT8_T * pass, INT32_T sizePass, BOOL_T continueLast = BOOL_FALSE,BOOL_T cleanLast=BOOL_FALSE)
	{
		static INT32_T lastIndexKey = 0;
		static UINT8_T lastFactor = 0;
		if (cleanLast == BOOL_TRUE)
		{
			lastIndexKey = 0;
			lastFactor = 0;
			return BOOL_FALSE;
		}

		if (data == NULL || pass == NULL || sizeData <= 0 || sizePass <= 0)
			return BOOL_FALSE;

		UINT8_T * password = getTrulyPassword(pass, sizePass);

		if (continueLast == BOOL_FALSE)
		{
			lastIndexKey = 0;
			lastFactor = 0;
		}
		for (INT32_T i = 0; i < sizeData; i++)
		{
			data[i] = data[i] ^ ((UINT8_T)(password[lastIndexKey]) ^ lastFactor);
			data[i] = ((data[i] & 0xf) << 4) | ((data[i] >> 4) & 0xf);

			lastFactor = (UINT8_T)(password[lastIndexKey] + lastFactor * 13);
			lastIndexKey = (lastIndexKey + 1) % sizePass;
		}
		delete[] password;
		return BOOL_TRUE;
	}
	/*
	参考lock函数介绍
	*/
	BOOL_T unlock(UINT8_T * data, INT32_T sizeData, UINT8_T * pass, INT32_T sizePass, BOOL_T continueLast = BOOL_FALSE, BOOL_T cleanLast = BOOL_FALSE)
	{
		
		static INT32_T lastIndexKey = 0;
		static UINT8_T lastFactor = 0;
		if (cleanLast == BOOL_TRUE)
		{
			lastIndexKey = 0;
			lastFactor = 0;
			return BOOL_FALSE;
		}

		if (data == NULL || pass == NULL || sizeData <= 0 || sizePass <= 0)
			return BOOL_FALSE;

		
		UINT8_T * password=getTrulyPassword(pass, sizePass);
		if (continueLast == BOOL_FALSE)
		{
			lastIndexKey = 0;
			lastFactor = 0;
		}
		for (INT32_T i = 0; i < sizeData; i++)
		{
			data[i] = ((data[i] & 0xf) << 4) | ((data[i] >> 4) & 0xf);
			data[i] = data[i] ^ ((UINT8_T)(password[lastIndexKey]) ^ lastFactor);

			lastFactor = (UINT8_T)(password[lastIndexKey] + lastFactor * 13);
			lastIndexKey = (lastIndexKey + 1) % sizePass;
		}
		delete[] password;
		return BOOL_TRUE;
	}
	/*
	前面说了，要直接清空历史特征码，参数要写很多，而且都是不用的参数
	因此，为了简化这个过程，我们使用几个函数来封装一下
	方便调用
	以后，你可以这样链式调用：
	EverythingLocker locker;
	locker.cleanLockCode().lock(data,......);
	*/
	EverythingLocker& cleanLockCode()
	{
		lock(NULL, 0, NULL, 0, BOOL_FALSE, BOOL_TRUE);
		return *this;
	}
	EverythingLocker& cleanUnlockCode()
	{
		unlock(NULL, 0, NULL, 0, BOOL_FALSE, BOOL_TRUE);
		return *this;
	}
	EverythingLocker& cleanLockAndUnlockCode()
	{
		lock(NULL, 0, NULL, 0, BOOL_FALSE, BOOL_TRUE);
		unlock(NULL, 0, NULL, 0, BOOL_FALSE, BOOL_TRUE);
		return *this;
	}
private:
	UINT8_T * m_innearKey;
	INT32_T m_sizeInnearKey;

	UINT8_T * getTrulyPassword(UINT8_T * pass, INT32_T sizePass)
	{
		UINT8_T * password = new UINT8_T[sizePass];
		for (INT32_T i = 0; i < sizePass; i++)
		{
			password[i] = pass[i];
		}
		inearLockKey(password, sizePass);
		return password;
	}
	void inearLockKey(UINT8_T * data, INT32_T size)
	{
		INT32_T inSize = 0;
		UINT8_T * inKey = makeInearKey(&inSize);
		INT32_T j = 0;
		for (INT32_T i = 0; i < size; i++)
		{
			data[i] ^= inKey[j];
			j = (j + 1) % inSize;
		}
		delete[] inKey;
	}
	UINT8_T * makeInearKey(INT32_T * size)
	{
		INT32_T maxSize = 1024;
		UINT8_T * ret = new UINT8_T[maxSize];
		UINT8_T arr[] = { '#', 'C', 'o', 'p', 'y', 'r', 'i', 'g', 'h', 't', '@', 'U', 'g', 'e', 'x', '.', 'S', 'a', 'v', 'e', 'l', 'a', 'r', '-', '2', '0', '2', '0',0 };
		INT32_T sizeArr = sizeof(arr);

		
		arr[0] = getChechSum(&arr[1],sizeArr-1);

		UINT8_T fac = 0;
		INT32_T i = 0;
		INT32_T j = 0;
		while (i < maxSize)
		{
			ret[i] = arr[j];
			arr[j] = (UINT8_T)(arr[j] * 31) ^ fac;

			fac = fac * 7 + arr[j];

			i++;
			j = (j + 1) % sizeArr;
		}
		*size = i;
		return ret;
	}
	UINT8_T getChechSum(UINT8_T * data, INT32_T sizeData)
	{
		UINT8_T checkSum = 0;
		for (INT32_T k = 0; k < sizeData; k++)
		{
			checkSum = (UINT8_T)(checkSum * 17 + data[k] * 31);
			checkSum = ((checkSum & 0xf) << 4) | ((checkSum >> 4) & 0xf);
		}
		return checkSum;
	}
};


#endif // _EVERYTHING_LOCKER_HPP_