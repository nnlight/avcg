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
#include "folding.h"
#include "steps.h"
#include "timelim.h"
#include "main.h"
#include "options.h"
#include "grprint.h"
#include "timing.h"
#include "vcg_iface.h"


/*--------------------------------------------------------------------*/
/*  Main routines 						      */
/*--------------------------------------------------------------------*/


/*  Call of the parser
 *  ==================
 */

extern int yy_flex_debug;

static void parse_part( FILE *f)
{
	int 	errs,i;
	char c;

	start_time();
	debugmessage("parse_part","");
	
#if 0
	while ((c = getc(stdin)) != EOF) {
	  putc(c, stderr);
	}
#endif
	/* We start from the scratch */

	info_name_available = 0;
	for (i=0; i<3; i++) info_names[i]=NULL;

	free_memory();
	yyin = f;


	free_timelimit();
	if (G_timelimit>0) init_timelimit(G_timelimit);
	debugmessage("start_parsing", "");

#if 0
	while ((c = getc(yyin)) != EOF) {
	  putc(c, stderr);
	}
#endif
	/* Turn yy_flex_debug on if you want to debug the scanner */
	yy_flex_debug = 0;
	errs = parse();
	debugmessage("finished_parsing", "");
	

	if (errs>0) Fatal_error("Syntax error","");
	assert((Syntax_Tree!=NULL));

	stop_time("parse_part");
} /* parse_part */



/*--------------------------------------------------------------------*/

/*  Fatal Errors 
 *  ============
 *  Note: the parser uses internally the function fatal_error which is
 *  different.
 */

void Fatal_error(char *x,char *y)
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

static void	visualize_part(void)
{
	debugmessage("visualize_part","");

	/* Init of the default values */

        G_title         = myalloc(256);
		strcpy( G_title, "G_title");
	G_title[255] = 0;
        G_x             = -1L;
        G_y             = -1L;

	/* Check the output device */
#if 0
	if (!exfile) {
        	setScreenSpecifics(); 	/* this sets G_width, G_height */
	} else
#endif
	{
		G_width = G_height = 700;
	}
	G_width_set  = 0;	/* because they are not set by */
	G_height_set = 0;	/* the specification           */
	if (!G_xymax_final) G_xmax = G_width+10;
        if (!G_xymax_final) G_ymax = G_height+10;
        G_xbase         = 5;
        G_ybase         = 5;
        G_dspace        = 0;
        G_xspace        = 20;
        G_yspace        = 70;
        G_orientation   = TOP_TO_BOTTOM;
        G_folding       = 0;
        G_color         = WHITE;
        G_displayel     = NO;
        G_dirtyel       = NO;
        G_shrink        = 1;
        G_stretch       = 1;
	G_yalign        = AL_CENTER;
	G_portsharing   = YES;
	G_arrowmode     = AMFIXED;
	G_xraster	= 1;
 	G_yraster	= 1;
 	G_dxraster	= 1;

	/* No edge class is hidden: initialize this */

	clear_hide_class();


	/*  Analyze specification and allocate graph */

	step0_main();
	if (nr_errors!=0) Fatal_error("Wrong specification","");
	check_graph_consistency();


	/* Set drawing area */

	G_xymax_final = 1;
        V_xmin = V_xmin_initial;
        V_xmax = V_xmin + (long)G_xmax;
        V_ymin = V_ymin_initial;
        V_ymax = V_ymin + (long)G_ymax;

	relayout();
} /* visualize_part */




/* Relayout the graph 
 * ------------------
 */

void relayout(void)
{
	debugmessage("relayout","");
        start_time();

	if (G_timelimit>0) init_timelimit(G_timelimit);
	free_all_lists();
        if (nr_errors==0) folding();
        stop_time("folding");

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
		if (prio_phase==1)     {
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
		if (nr_errors!=0) Fatal_error("Wrong specification","");

		/* step1_main calls tree_main, if TREE_LAYOUT.
		 */

		if (layout_flag != TREE_LAYOUT) {
                	step2_main();
			if (nr_errors!=0) Fatal_error("Wrong specification","");
                	step3_main();
			if (nr_errors!=0) Fatal_error("Wrong specification","");
		}
        	step4_main();
		if (nr_errors!=0) Fatal_error("Wrong specification","");
	}
	else {
		/* Prepare given layout: calculate co-ordinate of edges 
 		 * width and height of nodes etc.
		 */ 

		prepare_nodes();
	}
	free_timelimit();
} /* relayout */

/*--------------------------------------------------------------------*/




/*--------------------------------------------------------------------*/


/*  The main program
 *  ================
 */

void vcg_Parse( FILE *input_file)
{
	char testvar = -1;

	if (testvar != -1) {
		FPRINTF(stderr,"Warning: On this system, chars are unsigned.\n");
		FPRINTF(stderr,"This may yield problems with the graph folding operation.\n");
	}


	G_xmax = G_ymax = -1;

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

	/*if (!silent) { FPRINTF(stdout,"Wait "); FFLUSH(stdout); }*/
	parse_part( input_file);
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


Color_t vcg_GetBgColor()
{
	return G_color;
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





