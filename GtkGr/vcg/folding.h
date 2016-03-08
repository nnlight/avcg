/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         folding.h                                          */
/*   version:      1.00.00                                            */
/*   creation:     17.9.1993                                          */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Folding and Unfolding of the graph                 */
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


#ifndef FOLDING_H
#define FOLDING_H

/*--------------------------------------------------------------------*/

/* See folding.c for explanation
 * -----------------------------
 */


/* Global variables
 * ----------------
 */

extern int      *hide_class;

/* Prototypes
 * ----------
 */

void    folding(void);

void    init_folding_keepers_globals();
void    clear_folding_keepers();
void    add_sgfoldstart       (GNODE v);
void    add_sgunfoldstart     (GNODE v);

void    create_adjedge   (GEDGE edge);
void    delete_adjedge   (GEDGE edge);
void    clear_hide_class (void);
GNODE   create_labelnode (GEDGE e);


/*--------------------------------------------------------------------*/

#endif /* FOLDING_H */

