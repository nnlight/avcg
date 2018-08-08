/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         main.c                                             */
/*   version:      1.00.00                                            */
/*   creation:     1.4.1993                                           */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Top level program                                  */
/*   status:       in work                                            */
/*                                                                    */
/*--------------------------------------------------------------------*/


/*
 *   Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                               the Compare Consortium
 *
 *  This program and documentation is free software; you can redistribute
 *  it under the terms of the  GNU General Public License as published by
 *  the  Free Software Foundation;  either version 2  of the License,  or
 *  (at your option) any later version.
 *
 *  This  program  is  distributed  in  the hope that it will be useful,
 *  but  WITHOUT ANY WARRANTY;  without  even  the  implied  warranty of
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the
 *  GNU General Public License for more details.
 *
 *  You  should  have  received a copy of the GNU General Public License
 *  along  with  this  program;  if  not,  write  to  the  Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "grammar.h"
#include "alloc.h"
#include "steps.h"
#include "main.h"
#include "options.h"
#include "vcg_iface.h"


static void relayout(void);
static void add_dirty_labels();

/*--------------------------------------------------------------------*/
/*  Main routines                             */
/*--------------------------------------------------------------------*/


/*  Call of the parser
 *  ==================
 */
static void parse_part( FILE *f, const char *fname)
{
    int     errs,i;

    /* We start from the scratch */

    for (i=0; i<3; i++) info_names[i] = NULL;

    free_memory();


    init_lex( f, fname);
    errs = parse();


    if (errs>0) Fatal_error("Syntax error","");
    assert((Syntax_Tree!=NULL));
} /* parse_part */



/*--------------------------------------------------------------------*/

/*  Fatal Errors
 *  ============
 *  Note: the parser uses internally the function fatal_error which is
 *  different.
 */

void Fatal_error(const char *x, const char *y)
{
        FPRINTF(stderr,"Fatal error: %s %s !\n",x,y);
        FPRINTF(stderr,"Aborted !\n");
        exit(-1);
}



/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/

/*  Call of the visualizer
 *  ======================
 */
static void visualize_part(void)
{
    /* Init of the default values */

    G_title         = "G_title";
    G_x             = -1L;
    G_y             = -1L;
    G_width = G_height = 700; /* setScreenSpecifics(); (this sets G_width, G_height)*/
    G_xbase         = 5;
    G_ybase         = 5;
    G_xspace        = 20;
    G_yspace        = 70;
    G_dspace        = 0;
    G_orientation   = TOP_TO_BOTTOM;
    G_color         = WHITE;
    G_displayel     = NO;
    G_dirtyel       = NO;
    G_shrink        = 1;
    G_stretch       = 1;
    G_yalign        = AL_CENTER;
    G_portsharing   = YES;
    G_arrowmode     = AMFIXED;
    G_xraster   = 1;
    G_yraster   = 1;
    G_dxraster  = 1;


    /*  Analyze specification and allocate graph */

    step0_main();
    if (nr_errors!=0) Fatal_error("Wrong specification","");
    check_graph_consistency();


    /* Set drawing area */

    V_xmin = 0;
    V_xmax = V_xmin + G_width+10;
    V_ymin = 0;
    V_ymax = V_ymin + G_height+10;

    relayout();
} /* visualize_part */




/* Relayout the graph
 * ------------------
 */
static void relayout(void)
{
    free_all_tmplists();
    folding();

    if (!locFlag) {

        if (min_mediumshifts>max_mediumshifts)
            max_mediumshifts = min_mediumshifts;
        if (min_centershifts>max_centershifts)
            max_centershifts = min_centershifts;
        if (min_baryiterations>max_baryiterations)
            max_baryiterations = min_baryiterations;
        if (one_line_manhatten==1) manhatten_edges = 1;
        if ((manhatten_edges==1)&&(prio_phase==0)) prio_phase = 1;
        if ((prio_phase==1)&&(straight_phase==0)) straight_phase = 1;
        if ((prio_phase==0)&&(straight_phase==1)) prio_phase = 1;
        if (prio_phase==1) {
            min_centershifts = 0;
            max_centershifts = 0;
        }
        if (G_dxraster<=0) G_dxraster = 1;
        if (G_xraster<=0)  G_xraster  = 1;
        if (G_xraster % G_dxraster) {
            G_xraster = (G_xraster/G_dxraster) * G_dxraster;
        }

        /* Calculate new layout */

        step1_main();

        /* step1_main calls tree_main, if TREE_LAYOUT.
         */

        if (layout_flag != TREE_LAYOUT) {
            step2_main();
            step3_main();
        }
        step4_main();
    }
    else {
        /* Prepare given layout: calculate co-ordinate of edges
         * width and height of nodes etc.
         */

        prepare_nodes();
    }
    if (G_displayel && (locFlag || G_dirtyel)) {
        add_dirty_labels();
    }
} /* relayout */

static void add_dirty_labels()
{
    GNODE node, v;
    GEDGE e;

    /* В случае грязных лейблов дуг, специальные узлы не создаются,
     * т.о. лейблы никак не участвуют при лейауте.
     * Но признак ELABEL протаскивается и здесь, в самом конце,
     * мы им пользуемся, чтобы создать узлы.
     */
    /* все-таки могут появлятся технические лейбл узлы */
    /*assert(labellist == NULL);*/
#if 0
    /* на cfg.vcg  появляются лишние лейблы при dirty_edge_labels: yes */
    for (e = edgelist; e; e = ENEXT(e))
    {
        if (!EINVISIBLE(e) && ELABEL(e)) {
            v = create_labelnode(e);
            NX(v) = (ETBENDX(e)+EBBENDX(e))/2;
            NY(v) = (ETBENDY(e)+EBBENDY(e))/2;
            NX(v) = NX(v) - NWIDTH(v)/2;
            NY(v) = NY(v) - NHEIGHT(v)/2;
        }
    }
    for (e = tmpedgelist; e; e = ENEXT(e))
    {
        if (!EINVISIBLE(e) && ELABEL(e)) {
            v = create_labelnode(e);
            NX(v) = (ETBENDX(e)+EBBENDX(e))/2;
            NY(v) = (ETBENDY(e)+EBBENDY(e))/2;
            NX(v) = NX(v) - NWIDTH(v)/2;
            NY(v) = NY(v) - NHEIGHT(v)/2;
        }
    }
#else
    for (node = nodelist; node; node = NNEXT(node))
    {
        for (e = FirstSucc(node); e; e = NextSucc(e))
        {
            if (!EINVISIBLE(e) && ELABEL(e)) {
                v = create_labelnode(e);
                NX(v) = (ETBENDX(e)+EBBENDX(e))/2;
                NY(v) = (ETBENDY(e)+EBBENDY(e))/2;
                NX(v) = NX(v) - NWIDTH(v)/2;
                NY(v) = NY(v) - NHEIGHT(v)/2;
            }
        }
    }
    for (node = dummylist; node; node = NNEXT(node))
    {
        for (e = FirstSucc(node); e; e = NextSucc(e))
        {
            if (!EINVISIBLE(e) && ELABEL(e)) {
                v = create_labelnode(e);
                NX(v) = (ETBENDX(e)+EBBENDX(e))/2;
                NY(v) = (ETBENDY(e)+EBBENDY(e))/2;
                NX(v) = NX(v) - NWIDTH(v)/2;
                NY(v) = NY(v) - NHEIGHT(v)/2;
            }
        }
    }
#endif
} /* add_dirty_labels */


/*--------------------------------------------------------------------*/


/*  The main program
 *  ================
 */
void vcg_Parse( FILE *input_file, const char *filename)
{
    char testvar = -1;

    if (testvar != -1) {
        FPRINTF(stderr,"Warning: On this system, chars are unsigned.\n");
        FPRINTF(stderr,"This may yield problems with the graph folding operation.\n");
    }

    initOptions();

    if (fastflag) {
        min_baryiterations = 0;
        max_baryiterations = 2;
        min_mediumshifts = 0;
        max_mediumshifts = 2;
        min_centershifts = 0;
        max_centershifts = 2;
        max_edgebendings = 2;
        max_straighttune = 2;
    }

    parse_part( input_file, filename);
    
    visualize_part();


    {  /* Display part calls the display device driver. This is a
         * device dependent function !!!
         * The device driver is basically a loop that draws the graph and
         * reacts on interaction, until the FINISHING interaction is
         * selected.
         */
        /*display_part(); */
        /* see draw_main() for draw algorithm details! */
    }

    return;
} /* vcg_Parse */

void vcg_ParseFile( const char *filename)
{
    FILE *f = fopen( filename, "r");
    if (!f)
    {
        printf("Cant open file: %s\n", filename);
        exit(-1);
    }
    vcg_Parse( f, filename);
    fclose(f);
    return;
} /* vcg_ParseFile */

void vcg_Relayout()
{
#if 0
    visualize_part();
#else
    relayout();
#endif
}


Color_t vcg_GetBgColor()
{
    return (Color_t)G_color;
} /* vcg_GetBgColor */

int vcg_GetCmapSize()
{
    return G_cmap_size;
}
unsigned char *vcg_GetCmapRed()
{
    return G_redmap;
}
unsigned char *vcg_GetCmapGreen()
{
    return G_greenmap;
}
unsigned char *vcg_GetCmapBlue()
{
    return G_bluemap;
}
int *vcg_GetHideClass(int *len_p)
{
    *len_p = max_nr_classes;
    return hide_class;
}





