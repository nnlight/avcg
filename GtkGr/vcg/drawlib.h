/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         drawlib.h                                          */
/*   version:      1.00.00                                            */
/*   creation:     14.4.93                                            */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Library of drawing routines                        */
/*                 of edges                                           */
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


#ifndef DRAWLIB_H
#define DRAWLIB_H

/*--------------------------------------------------------------------*/

/* See drawlib.c for explanation
 * -----------------------------
 */


/* Global Variables
 * ----------------
 */

extern int  gs_actbackground;
extern int  gs_stringw;
extern int  gs_stringh;
extern int  gs_boxw;
extern int  gs_boxh;


/* Prototypes
 * ----------
 */

void    gs_printstr     _PP((char *s,int c));
void    gs_calcstringsize   _PP((char *s));
void    gs_setto        _PP((int x,int y));
void    gs_setshrink        _PP((int a,int b));
void    gs_calctextboxsize  _PP((GNODE v));
void    gs_textbox      _PP((GNODE v));
void    gs_reverttextbox    _PP((GNODE v));
void    gs_halfreverttextbox    _PP((GNODE v));
void    gs_stringbox        _PP((GNODE v));
void    gs_calcrhombsize    _PP((GNODE v));
void    gs_rhomb        _PP((GNODE v));
void    gs_revertrhomb      _PP((GNODE v));
void    gs_halfrevertrhomb  _PP((GNODE v));
void    gs_calctrianglesize _PP((GNODE v));
void    gs_triangle     _PP((GNODE v));
void    gs_reverttriangle   _PP((GNODE v));
void    gs_halfreverttriangle   _PP((GNODE v));
void    gs_calcellipsesize  _PP((GNODE v));
void    gs_ellipse      _PP((GNODE v));
void    gs_revertellipse    _PP((GNODE v));
void    gs_halfrevertellipse    _PP((GNODE v));
void    gs_anchornode       _PP((GNODE v));
void    gs_solidarrow       _PP((GEDGE e));
void    gs_dashedarrow      _PP((GEDGE e));
void    gs_dottedarrow      _PP((GEDGE e));

#ifdef X11
void x11_followedge_arrow   _PP((GEDGE e));
void x11_followedge_anchors _PP((GEDGE e));
#endif


/*--------------------------------------------------------------------*/

#endif /* DRAWLIB_H  */


