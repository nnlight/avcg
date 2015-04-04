#include <stdio.h>
#include <stdlib.h>
#include "vcg_iface.h"




int main(int argc , char *argv[])
{
	if ( argc <2 )
	{
		printf( "usage: %s input filename\n", argv[0]);
		exit(1);
	}

	vcg_ParseFile( argv[1]);
	printf("\n1st ok\n");

	vcg_ParseFile( argv[1]);
	printf("\n2nd ok\n");

	return 0;
}
