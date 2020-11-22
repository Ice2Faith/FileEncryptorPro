#ifndef _TYPES_32_H_
#define _TYPES_32_H_
//base type bit len not equals on int and long,else is equals
#define COMPLIER_BIT_WIN32

#if defined COMPLIER_BIT_WIN32
typedef		char				INT8_T;
typedef		unsigned char		UINT8_T;
typedef		short				INT16_T;
typedef		unsigned short		UINT16_T;
typedef		int					INT32_T;//long
typedef		unsigned int		UINT32_T;//unsigned int
typedef		long long			INT64_T;
typedef		unsigned long long	UINT64_T;

typedef		INT32_T				INT_T;

typedef		float				FLOAT32_T;
typedef		double				FLOAT64_T;

typedef		char				BOOL_T;
#define		BOOL_TRUE			1
#define		BOOL_FALSE			0
#endif // COMPLIER_BIT_WIN32

#if defined COMPLIER_BIT_WIN64
typedef		char				INT8_T;
typedef		unsigned char		UINT8_T;
typedef		short				INT16_T;
typedef		unsigned short		UINT16_T;
typedef		int					INT32_T;
typedef		unsigned int		UINT32_T;
typedef		long				INT64_T;
typedef		unsigned long		UINT64_T;

typedef		INT32_T				INT_T;

typedef		float				FLOAT32_T;
typedef		double				FLOAT64_T;

typedef		char				BOOL_T;
#define		BOOL_TRUE			1
#define		BOOL_FALSE			0
#endif // COMPLIER_BIT_WIN64

#if defined COMPLIER_BIT_AS64_JNI
typedef 	char				INT8_T;
typedef 	unsigned char		UINT8_T;
typedef		int16_t				INT16_T;
typedef		uint16_t			UINT16_T;
typedef		int32_t				INT32_T;
typedef		uint32_t			UINT32_T;
typedef		int64_t				INT64_T;
typedef		uint64_t			UINT64_T;

typedef		INT32_T				INT_T;

typedef		float				FLOAT32_T;
typedef		double				FLOAT64_T;

typedef		char				BOOL_T;
#define		BOOL_TRUE			1
#define		BOOL_FALSE			0
#endif // COMPLIER_BIT_AS64_JNI

#define TOBOOL(a) (((a))==0?BOOL_FALSE:BOOL_TRUE)

UINT8_T hostIsBigEndian();
UINT8_T hostIsLittleEndian();
void * swapEndianSort(void * val, INT32_T size);
void * host2BigEndian(void * val, INT32_T size);
void * host2LittleEndian(void * val, INT32_T size);
void * bigEndian2Host(void * val, INT32_T size);
void * bigEndian2Host(void * val, INT32_T size);
#endif // _TYPES_32_H_