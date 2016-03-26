/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         steps.h                                            */
/*   version:      1.00.00                                            */
/*   creation:     10.4.1993                                          */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Readin and Layout steps                            */
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

#ifndef STEPS_H
#define STEPS_H

/*--------------------------------------------------------------------*/

/* step0 - построение графа
 * folding - adj cтроятся
 * step1 - топ. сортировка узлов, создание dummy, разброс по уровням, (трюк с connect узлами)
 * step2 - bary centering, cross calculation, horder, упор. на каждом уровне (дуги на conn узлы возвращаются)
 * step3 - calculate coordinates of nodes
 * step4 - коорд. дуг
 */

/* Global Variables
 * ----------------
 */

/* from step0.c */
extern struct gnode foldnode;
extern struct gedge foldedge;

/* from step1.c */
extern  DEPTH   *layer;     /* This is an array ! */
extern  int     maxdepth;

/* from step2.c */
extern int     max_nodes_per_layer;

/* from step3.c */
extern GNLIST *tpred_connection1;
extern GNLIST *tpred_connection2;

/* from step4.c */
extern int maximal_xpos;
extern int maximal_ypos;
extern int st_nr_vis_nodes;
extern int st_nr_vis_edges;
extern int st_nr_vis_nearedges;
extern int st_nr_vis_dummies;
extern int st_nr_vis_labels;
extern int st_nr_invis_graphs;
extern int st_nr_invis_nodes;
extern int st_nr_invis_edges;
extern int st_max_indeg;
extern int st_max_outdeg;
extern int st_max_degree;


/* Prototypes
 * ----------
 */

/* from step0.c */
void    step0_main          (void);
GNODE   lookup_hashnode     (char *title);


/* from folding.c */
void    folding(void);

void    init_folding_keepers_globals();
void    add_sgfoldstart     (GNODE v);
void    add_sgunfoldstart   (GNODE v);
void    add_foldstart       (GNODE v);
void    add_unfoldstart     (GNODE v);
void    add_foldstop        (GNODE v);

void    create_adjedge  (GEDGE edge);
void    delete_adjedge  (GEDGE edge);
GNODE   create_labelnode(GEDGE e);


/* from step1.c */
void    step1_main();
GEDGE   revert_edge(GEDGE edge);
GEDGE   create_edge(GNODE start, GNODE end, GEDGE edge, int arrow);
void    prepare_back_edges();
void    insert_anchor_edges();
#ifdef DEBUG
void    db_output_graph         (void);
void    db_output_adjacencies   (void);
void    db_output_adjacency     (GNODE node, int f);
void    db_output_layer         (void);
void    db_output_vcglayer      (char *fn);
#endif

/* from step2.c */
void    step2_main              (void);
#ifdef DEBUG
void    db_output_all_layers    (void);
void    db_output_tmp_layer     (int i);
int     db_check_proper         (GNODE v,int level);
#endif

/* from step3.c */
void    step3_main              (void);
void    calc_all_node_sizes     (void);
void    alloc_levelshift        (void);


/* from step4.c */
void    step4_main              (void);
void    calc_all_ports          (int xya);
void    calc_node_ports         (GNODE v, int xya);
void    calc_edge_xy            (GNODE v);
void    calc_edgearrow          (GNODE v);
void    calc_max_xy_pos         (void);
void    statistics              (void);

/* from tree.c */
int tree_main       (void);
void sort_all_adjacencies();

/* from prepare.c */
void    prepare_nodes           (void);
void    calc_node_size          (GNODE v);

/*--------------------------------------------------------------------*/

#endif /* STEPS_H  */

