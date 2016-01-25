/*****************************************************************************
 FILE BEGIN: type.h
*****************************************************************************/
#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

/*
 * Symbol Definition
 */
#ifndef NULL
#define NULL    0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef ERROR
#define ERROR   -1
#endif

#ifndef OK
#define OK      0
#endif

/*
 * Data Type Declaration
 */
#ifndef uint64
typedef unsigned long long  uint64;
#endif

#ifndef int64
typedef signed long long    int64;
#endif

#ifndef uint32
typedef unsigned int        uint32;
#endif

#ifndef int32
typedef signed int          int32;
#endif

#ifndef uint16
typedef unsigned short      uint16;
#endif

#ifndef int16
typedef signed short        int16;
#endif

#ifndef uint8
typedef unsigned char       uint8;
#endif

#ifndef int8
typedef signed char         int8;
#endif

#define isblank(c)	(c == ' ' || c == '\t')

//#define __DEBUG__

#ifdef __DEBUG__
#define DEBUG_LOG(fmt, args...) \
    printf("DEBUG: [%s:%d] DbgMsg:"fmt"\r\n", \
        __FUNCTION__, __LINE__, ##args)
#define DEBUG_ERR(errno, fmt, args...) \
    printf("ERROR: [%s:%d] ErrNum:%d ErrMsg:"fmt"\r\n", \
        __FUNCTION__, __LINE__, errno, ##args)
#else
#define DEBUG_LOG(fmt, args...)
#define DEBUG_ERR(errno, fmt, args...)
#endif

//参数检查宏:如果arg==lmt则返回rtn
#define ARG_CHK_EQ(arg, lmt, rtn) \
do{if((arg) == (lmt)){DEBUG_LOG("ARG_CHK_EQ"); return(rtn);}}while(0)

//参数检查宏:如果arg!=lmt则返回rtn
#define ARG_CHK_NEQ(arg, lmt, rtn) \
do{if((arg) != (lmt)){DEBUG_LOG("ARG_CHK_NEQ"); return(rtn);}}while(0)

//参数检查宏:如果arg>lmt则返回rtn
#define ARG_CHK_GT(arg, lmt, rtn) \
do{if((arg) > (lmt)){DEBUG_LOG("ARG_CHK_GT"); return(rtn);}}while(0)

//参数检查宏:如果arg<lmt则返回rtn
#define ARG_CHK_LT(arg, lmt, rtn) \
do{if((arg) < (lmt)){DEBUG_LOG("ARG_CHK_LT"); return(rtn);}}while(0)

//参数检查宏:如果arg>=lmt则返回rtn
#define ARG_CHK_GEQ(arg, lmt, rtn) \
do{if((arg) >= (lmt)){DEBUG_LOG("ARG_CHK_GEQ"); return(rtn);}}while(0)

//参数检查宏:如果arg<=lmt则返回rtn
#define ARG_CHK_LEQ(arg, lmt, rtn) \
do{if((arg) <= (lmt)){DEBUG_LOG("ARG_CHK_LEQ"); return(rtn);}}while(0)

#endif /* __COMMON_TYPE_H__ */
/*****************************************************************************
 FILE END: type.h
*****************************************************************************/
