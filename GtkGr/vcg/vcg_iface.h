#ifndef _VCG_IFACE_H_
#define _VCG_IFACE_H_

#include <stdio.h>
#include "vcg_defs.h"
#include "gdl_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "graph.h"



void vcg_Parse( FILE *input_file, const char *filename);
void vcg_ParseFile( const char *filename);
void vcg_Relayout();

Color_t vcg_GetBgColor();
int            vcg_GetCmapSize();
unsigned char *vcg_GetCmapRed();
unsigned char *vcg_GetCmapGreen();
unsigned char *vcg_GetCmapBlue();
int *vcg_GetHideClass(int *len_p);



#ifdef __cplusplus
}
#endif

#endif /* _VCG_IFACE_H_ */

