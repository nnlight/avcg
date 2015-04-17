/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         alloc.h                                            */
/*   version:      1.00.00                                            */
/*   creation:     14.4.1993                                          */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */  
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Memory Management                                  */
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


#ifndef ALLOC_H 
#define ALLOC_H 

#include "globals.h"
#include "gdl_defs.h"
#include "vcg_defs.h"


#ifdef CHECKNODE
extern GNODE debug_checknode;
#endif


/* Prototypes
 * ----------
 * See alloc.c for more information.
 */

char *myalloc(int x);
void free_memory(void);
void *libc_malloc(int size);

GNODE nodealloc		_PP((GNODE refnode));
GNODE graphalloc	_PP((GNODE refnode));
void  nodedefaults	_PP((GNODE node));
void  foldnodedefaults	_PP((GNODE node));
void  inherit_foldnode_attributes	_PP((GNODE fn, GNODE y));
void  copy_nodeattributes		_PP((GNODE fn, GNODE y));

GNODE tmpnodealloc	_PP((int textm,int width,int height,int borderw,int fold,int color,int textc,int borderc, int shrink,int stretch,int horder));
void 	free_node	_PP((GNODE v));
void 	free_tmpnodes   _PP((void));
GNODE   search_xy_node	_PP((long x,long y));
void check_graph_consistency _PP((void));


GNLIST  nodelist_alloc		_PP((GNODE v));
GNLIST  tmpnodelist_alloc 	_PP((void));
GNLIST  foldnodelist_alloc 	_PP((void));
void free_regionnodelist	_PP(( GNLIST r));
void free_foldnodelists		_PP((void));

GEDGE edgealloc 	_PP((GEDGE refedge));
void  edgedefaults	_PP((GEDGE edge));
void  foldedgedefaults	_PP((GEDGE edge));
void  inherit_foldedge_attributes	_PP((GEDGE fn, GEDGE y));
void  copy_edgeattributes		_PP((GEDGE fn, GEDGE y));

GEDGE tmpedgealloc _PP((int lstyle,int thick,int xclass,int prio,int ecolor,int elcol,int arrows,int barrows,int arrowsty,int barrowsty,int arrowc,int barrowc,int horder));
void near_edge_insert       _PP((GEDGE e));
void bentnear_edge_insert   _PP((GEDGE e));
void back_edge_insert       _PP((GEDGE e));
ADJEDGE edgelist_alloc(void);
ADJEDGE prededgealloc 	    _PP((GNODE node,GEDGE edge));
ADJEDGE succedgealloc 	    _PP((GNODE node,GEDGE edge));

CONNECT	connectalloc	_PP((GNODE node));

DLLIST 	dllist_alloc	_PP((GNODE  node, DLLIST pred));
void    dllist_free	_PP((DLLIST x));
void    dllist_free_all	_PP((DLLIST x));

void	free_all_lists 	 _PP((void));
void    reinit_all_lists _PP((void));

/*--------------------------------------------------------------------*/

#endif /* ALLOC_H  */

