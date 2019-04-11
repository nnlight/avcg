#ifndef _STDAFX_H_
#define _STDAFX_H_
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <assert.h>
#include <string.h>
#include <math.h>
#if defined(_MSC_VER) && _MSC_VER < 1600/*VS2010*/
typedef unsigned char uint8_t;
#else
#include <stdint.h>
#endif

//using namespace std;
typedef std::string string;
using std::vector;
using std::pair;

#if __cplusplus < 201103L
#define unique_ptr auto_ptr
#endif


#ifdef WIN32
/* warning C4312: 'type cast' : conversion from 'volatile gsize' to 'gpointer' of greater size ... */
#pragma warning ( disable : 4312)

/*
 * далее идет кусок из glib-2.26.1/msvc_recommended_pragmas.h
 */
#ifndef _MSC_VER
#pragma error "This header is for Microsoft VC only."
#endif /* _MSC_VER */

/* Make MSVC more pedantic, this is a recommended pragma list
 * from _Win32_Programming_ by Rector and Newcomer.
 */
#pragma warning(error:4002) /* too many actual parameters for macro */
#pragma warning(error:4003) /* not enough actual parameters for macro */
#pragma warning(1:4010)     /* single-line comment contains line-continuation character */
#pragma warning(error:4013) /* 'function' undefined; assuming extern returning int */
#pragma warning(1:4016)     /* no function return type; using int as default */
#pragma warning(error:4020) /* too many actual parameters */
#pragma warning(error:4021) /* too few actual parameters */
#pragma warning(error:4027) /* function declared without formal parameter list */
#pragma warning(error:4029) /* declared formal parameter list different from definition */
#pragma warning(error:4033) /* 'function' must return a value */
#pragma warning(error:4035) /* 'function' : no return value */
#pragma warning(error:4045) /* array bounds overflow */
#pragma warning(error:4047) /* different levels of indirection */
#pragma warning(error:4049) /* terminating line number emission */
#pragma warning(error:4053) /* An expression of type void was used as an operand */
#pragma warning(error:4071) /* no function prototype given */
//# p ragma warning(disable:4101) /* unreferenced local variable */
#pragma warning(error:4150)

#pragma warning(disable:4244)   /* No possible loss of data warnings */
#pragma warning(disable:4305)   /* No truncation from int to char warnings */

/* work around Microsoft's premature attempt to deprecate the C-Library */
#pragma warning(disable:4996)   /* This function or variable may be unsafe. */
#endif /* WIN32 */


#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif


/**
 * В заданной строке (rStrText) заменить все вхождения заданной подстроки
 * (rcStrFind) на заданную строку (rcStrReplace).
 */
extern void
replace( string &rStrText, const string &rcStrFind, const string &rcStrReplace);

static inline int around(double d) { return (int)(d + 0.5); }
extern int calc_str_lines_num( const char *str);

template <typename T>
static inline T min3( T v1, T v2, T v3) { return std::min( v1, std::min( v2, v3)); }
template <typename T>
static inline T max3( T v1, T v2, T v3) { return std::max( v1, std::max( v2, v3)); }
template <typename T>
static inline T min4( T v1, T v2, T v3, T v4) { return std::min( std::min( v1, v2), std::min( v3, v4)); }
template <typename T>
static inline T max4( T v1, T v2, T v3, T v4) { return std::max( std::max( v1, v2), std::max( v3, v4)); }

/**
 * Фатальная ошибка, по которой приложение аварийно завершается.
 * Вызывается в случаях внутренних ошибок (вместо assert, если хочется, чтобы проверка
 * работала и в быстрой версии) или в случае попытки использования критической нереализованной
 * функциональности, без использования которой (непонятно, что делать)/(невозможно обойтись)
 */
extern void InternalError();


#endif /* _STDAFX_H_ */
