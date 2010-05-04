#ifndef _STDAFX_H_
#define _STDAFX_H_
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <tchar.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string>
#include <list>
#include <assert.h>
#include <string.h>


/** 
 * Фатальная ошибка, по которой приложение аварийно завершается.
 * Вызывается в случаях внутренних ошибок (вместо assert, если хочется, чтобы проверка
 * работала и в быстрой версии) или в случае попытки использования критической нереализованной 
 * функциональности, без использования которой (непонятно, что делать)/(невозможно обойтись)
 */
extern void InternalError();


#endif /* _STDAFX_H_ */
