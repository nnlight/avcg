/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         main.h                                             */
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

#ifndef MAIN_H
#define MAIN_H

/*--------------------------------------------------------------------*/

/* See main.c for explanation
 * --------------------------
 */


/* from parser -------------------------------------------*/
extern FILE     *yyin;

void   line_directive _PP((char *text));
void   syntaxerror    _PP((int line,int pos,char *mesge));
void   warning        _PP((int line,int pos,char *mesge));

#define SYERR(x,m) syntaxerror(xfirst_line(x),xfirst_column(x),m)

/* from device --------------------------------------------*/
void gs_wait_message	_PP((int c));

/*---------------------------------------------------------*/

/*  Prototypes
 *  ----------
 */

void Fatal_error	_PP((char *x, char *y));
void relayout		_PP((void));

/*--------------------------------------------------------------------*/
 
#endif /* MAIN_H */


