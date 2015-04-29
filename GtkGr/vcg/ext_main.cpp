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
    printf("1st ok (%s)\n", argv[1]);

    vcg_ParseFile( argv[1]);
    printf("2nd ok (%s)\n", argv[1]);

    return 0;
}
