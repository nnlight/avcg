#ifndef _VCG_IFACE_H_
#define _VCG_IFACE_H_

#include <stdio.h>
#include "vcg_defs.h"
#include "gdl_defs.h"

#ifdef __cplusplus
extern "C" {
#endif



void vcg_Parse( FILE *input_file, const char *filename);
void vcg_ParseFile( const char *filename);

Color_t vcg_GetBgColor();
int            vcg_GetCmapSize();
unsigned char *vcg_GetCmapRed();
unsigned char *vcg_GetCmapGreen();
unsigned char *vcg_GetCmapBlue();



#ifdef __cplusplus
}
#endif

#endif /* _VCG_IFACE_H_ */

