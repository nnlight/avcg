/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         options.h                                          */
/*   version:      1.00.00                                            */
/*   creation:     1.4.1993                                           */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Option handling                                    */
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


#ifndef OPTIONS_H
#define OPTIONS_H

/*--------------------------------------------------------------------*/

/* Global Variables
 * ----------------
 */

extern int silent;
extern int nr_max_errors;
extern int fastflag;
extern int supress_nodes;
extern int supress_edges;
extern int summarize_double_edges;
extern int hide_single_nodes;
extern int layout_flag;

#define TREE_LAYOUT 20

extern int crossing_heuristics;
extern int local_unwind;
extern int near_edge_layout;
extern int fine_tune_layout;
extern int edge_label_phase;
extern int min_baryiterations;
extern int max_baryiterations;
extern int skip_baryphase2;
extern int max_mediumshifts;
extern int min_mediumshifts;
extern int nwdumping_phase;
extern int max_centershifts;
extern int min_centershifts;
extern int prio_phase;
extern int straight_phase;
extern int max_straighttune;
extern int max_edgebendings;
extern int manhatten_edges;
extern int one_line_manhatten;
extern int layout_downfactor; 
extern int layout_upfactor; 
extern int layout_nearfactor; 

extern int G_spline;
extern int G_flat_factor;


extern long V_xmin;
extern long V_xmax;
extern long V_ymin;
extern long V_ymax;
extern long V_xmin_initial;
extern long V_ymin_initial;



extern char filename[];

#define MAXCLASS  16

extern int G_timelimit;
extern int G_xymax_final;
extern char *G_title;
extern long G_x, G_y;
extern int G_width, G_height;
extern int G_width_set, G_height_set;
extern int G_xmax, G_ymax;
extern int G_xbase, G_ybase;
extern int G_xspace, G_yspace;
extern int G_orientation;
extern int G_folding;
extern int G_invisible[];
extern int G_color;
extern int G_displayel;
extern int G_dirtyel;
extern int G_shrink, G_stretch;
extern int G_yalign;
extern int G_portsharing;
extern int G_arrowmode;
extern int G_dspace;

extern int G_xraster;
extern int G_yraster;
extern int G_dxraster;


extern char    *fold_elabel;
extern int     fold_lstyle;
extern int     fold_thick;
extern int     fold_ecolor;
extern int     fold_elcolor;
extern int     fold_arrows;
extern int     fold_barrows;
extern int     fold_arrowc;
extern int     fold_barrowc;
extern int     fold_arrsty;
extern int     fold_barrsty;

extern int 	max_eprio;

extern int  info_name_available;
extern char *info_names[3];
extern int  max_nr_classes;
extern int  class_name_available;
extern char **class_names;

extern int locFlag;



/*  Prototypes
 *  ----------
 */

int  scanOptions		(int argc, char *argv[]);
void print_basic_help	(void);
void print_help			(void);


/*--------------------------------------------------------------------*/
 
#endif /* OPTIONS_H */


