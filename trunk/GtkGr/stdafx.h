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
#include <assert.h>
#include <string.h>
#include <math.h>


//using namespace std;
typedef std::string string;


#ifdef WIN32
/* warning C4312: 'type cast' : conversion from 'volatile gsize' to 'gpointer' of greater size ... */
#pragma warning ( disable : 4312)
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

/** 
 * Фатальная ошибка, по которой приложение аварийно завершается.
 * Вызывается в случаях внутренних ошибок (вместо assert, если хочется, чтобы проверка
 * работала и в быстрой версии) или в случае попытки использования критической нереализованной 
 * функциональности, без использования которой (непонятно, что делать)/(невозможно обойтись)
 */
extern void InternalError();


#endif /* _STDAFX_H_ */
