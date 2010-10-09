// stdafx.cpp : source file that includes just the standard includes
// GtkGr.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file




/* В заданной строке (rStrText) заменить все вхождения заданной подстроки
(rcStrFind) на заданную строку (rcStrReplace). */
void 
replace( string &rStrText, const string &rcStrFind, const string &rcStrReplace)
{
	const string::size_type
	ncSizeFind = rcStrFind.length(),
	ncSizeReplace = rcStrReplace.length();
	string::size_type nPos = rStrText.find(rcStrFind);
	while (nPos != string::npos)
	{
		rStrText.replace(nPos, ncSizeFind, rcStrReplace);
		nPos = rStrText.find(rcStrFind, nPos + ncSizeReplace);
	}
}

void InternalError()
{
	printf("FATAL ERROR: InternalError\n");
	/* выходим */
	exit(-1);
}
