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

	FILE *f = fopen( argv[1], "r");
	if ( !f )
	{
		printf("cant open file:%s\n", argv[1]);
		exit(1);
	}
	vcg_Parse( f);
	fclose(f);
	printf("\n1st ok\n");

	f = fopen( argv[1], "r");
	if ( !f )
	{
		printf("cant open file:%s\n", argv[1]);
		exit(1);
	}
	vcg_Parse( f);
	fclose(f);
	printf("\n2st ok\n");
	return 0;
}
