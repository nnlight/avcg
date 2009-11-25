/* SCCS-info %W% %E% */

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
 *
 *  The software is available per anonymous ftp at ftp.cs.uni-sb.de.
 *  Contact  sander@cs.uni-sb.de  for additional information.
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


/*--------------------------------------------------------------------*/

/* Prototypes
 * ==========
 */

/*  These functions are device dependent. Instead including all external
 *  device dependent h-files, we declare them here as external. This
 *  simplifies the selection of the device.
 *  Depending on the device, these functions are implemented in sunvdev.c 
 *  or X11dev.c.
 */

extern void display_part	_PP((void));
extern void setScreenSpecifics  _PP((void));
extern void gs_exit             (int x);

void	gs_exit(int x)
{
	exit(x);
}

static int   f_is_writable	_PP((char *fname));


/* Global variables
 * ================
 */


char short_banner[128];
char version_str[24]  = "V.1.3";
char date_str[48]     = "$Date: 1995/02/08 11:11:14 $";
char revision_str[48] = "$Revision: 3.17 $";


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
}


/*--------------------------------------------------------------------*/


/*  The main program
 *  ================
 */


void vcg_Parse( FILE *input_file)
{
	char testvar;
	int i;

	testvar = -1;
	if (testvar != -1) {
		FPRINTF(stderr,"Warning: On this system, chars are unsigned.\n");
		FPRINTF(stderr,"This may yield problems with the graph folding operation.\n");
	}

	for (i=0; i<48; i++) {
		if (date_str[i]=='$')     date_str[i]=' ';	
		if (revision_str[i]=='$') revision_str[i]=' ';	
	}
	
	SPRINTF(short_banner,"USAAR Visualization Tool VCG/XVCG %s %s", 
			version_str, revision_str);


	G_xmax = G_ymax = -1;

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

	if (!silent) { FPRINTF(stdout,"Wait "); FFLUSH(stdout); }
	parse_part( input_file);
/*TODO	visualize_part();*/

	if (exfile) 
	{
		/* ... output to some devices, ps... */
	}
	else {  /* Display part calls the display device driver. This is a 
		 * device dependent function !!!
		 * The device driver is basically a loop that draws the graph and
		 * reacts on interaction, until the FINISHING interaction is 
		 * selected.
		 */
		/*display_part(); */
	}

	return;
}


/*--------------------------------------------------------------------*/

/* Check whether file is writable
 * ==============================
 * Returns 1 if yes. .
 */

#ifdef ANSI_C
static int f_is_writable(char *fname)
#else
static int f_is_writable(fname)
char *fname;
#endif
{
        FILE *f;
        char *c;

        f = NULL;
        c = fname;
        while (*c) c++;
        if (c>fname) c--;
        if (*c=='/')  return(0); 
        if (( strcmp(fname,"-")==0 ) || (*fname==0))  return(0); 
        f = fopen(fname,"r");
        if (f != NULL) { fclose(f); return(0); }
        return(1);
}


/*--------------------------------------------------------------------*/

/*  Fatal Errors 
 *  ============
 *  Note: the parser uses internally the function fatal_error which is
 *  different.
 */


#ifdef ANSI_C
void Fatal_error(char *x,char *y)
#else
void Fatal_error(x,y)
char *x;
char *y;
#endif
{
      	FPRINTF(stderr,"Fatal error: %s %s !\n",x,y);
      	FPRINTF(stderr,"Aborted !\n");
        gs_exit(-1);
}



/*--------------------------------------------------------------------*/





#ifdef use_unused
/* Relayout the graph 
 * ------------------
 */


#ifdef ANSI_C
void relayout(void)
#else
void relayout()
#endif
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
}
#endif
/*--------------------------------------------------------------------*/
