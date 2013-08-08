#ifndef _VCG_IFACE_H_
#define _VCG_IFACE_H_

#include <stdio.h>
#include "vcg_defs.h"
#include "gdl_defs.h"

#ifdef __cplusplus
extern "C" {
#endif



void vcg_Parse( FILE *input_file);

Color_t vcg_GetBgColor();



#ifdef __cplusplus
}
#endif

#endif /* _VCG_IFACE_H_ */

