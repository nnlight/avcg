/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         globals.h                                          */
/*   version:      1.00.00                                            */
/*   creation:     1.4.1993                                           */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Global definitions and setup                       */
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




/*   This file contains the setup of the tool for the C compiler.
 *   It is included into every other C file.
 *   Please adapt corresponding to your configuration.
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

/*------------- Please change according to your configuration --------*/

/*   Ansi C compiler or K&R C compiler ?
 *   Mainly, this causes to use prototypes or not. Note that the Suntool
 *   include files on some systems are Non-Ansi and may cause confusion.
 *   See the corresponding comments in the Makefile.
 *   Ansi C compiler set __STDC__, but Non-Ansi compilers may set this, too.
 *   Thus we use our own flag ANSI_C.
 */

/* #ifdef __STDC__==1
 * #define ANSI_C
 * #endif
 */

/* #define ANSI_C */
/* #undef  ANSI_C */
#define ANSI_C



/*   User Signals available or not ?
 *   Unix provides signals SIGUSR1 and SIGUSR2 (30, 31), which are used
 *   to control animations from external. If signal() or free signals are
 *   not available (e.g. on Non-Unix systems), then undefine USR_SIGNAL.
 */

#define USR_SIGNAL
/* #undef  USR_SIGNAL */


/* Good quicksort available or not ?
 * If there exist a good randomized quicksort `qsort' in the library,
 * please use it. But if the quicksort in the library is slow, or not
 * randomized, define OWN_QUICKSORT. We often have to sort presorted
 * arrays.
 * Note: If OWN_QUICKSORT is defined, not all calls of `qsort' are
 * exchanged, but only the critical ones.
 */

/* #undef OWN_QUICKSORT  */
#define OWN_QUICKSORT


/*   Window system Sunview or X11 ?
 *   Sunview is fast, but does not support remote window displaying.
 *   X11 (R5) is a little bit slower, but supports remote display.
 *   Do not define both !!!
 */

#define X11

#ifdef X11
#define FAST_X11_DRAWING
#endif


/*   Required alignment for structs (power of 2). IALIGN is the appropriate
 *   bit mask to generate the alignment. Because we use floats in structs,
 *   mostly an alignment of 8 is appropriate.
 */

#ifndef ALIGN
/* #define ALIGN  8 */
#define ALIGN  8
#define IALIGN (ALIGN-1)
#endif


/*   Memory block size for memory allocation in bytes. The allocation
 *   mechanism allocates blocks of this size, and dynamically increases
 *   the number of blocks, if more memory is necessary.
 *   Do not set the blocksize too small. For instance the tool does not
 *   work with blockssize less than 1 KB. Propose for good performance:
 *   256 KB minimal.
 */

/* Blocks of 1 Megabyte */

/* #define MEMBLOCKSIZE 1048576 */
#define MEMBLOCKSIZE 1048576


/*   The default font for X11. Not needed at the Sunview installation.
 *   Please insert an appropriate font of about 14 to 17 dot size.
 */

#ifdef X11
#define VCG_DEFAULT_FONT "-*-courier-*-*-*--14-*-*-*-*-*-*-*"
#endif


/*   Whether the input focus grab is actively or not.
 *   If NOINPUTFOCUS, the VCG tool does not grab actively the input
 *   focus. Depending on the window manager, this may cause that
 *   the keypresses are ignored afterwards, unless the mouse pointer
 *   is once moved out of the window and back into it.
 *   On the other side, some installations cause a fatal error or
 *   strange behaviour, if the VCG tool actively grabs the input
 *   focus.
 *   The conservative definement is #define NOINPUTFOCUS.
 */

#ifdef X11
#define NOINPUTFOCUS
#endif


/*   Debugging On or Off ?
 *   We have three kinds of debugging:
 *      a) Behaviour tracing: a lot of messages nearly on every
 *         function entry are produced if DEBUG is switched on.
 *      b) Assertion checking: only if an assertion fails, an
 *         appropriate message is produced.
 *      c) Node tracing: This is basically the same as the behaviour
 *	   tracing. At each function entry, the status of a node
 *	   will be printed. This happens if CHECKNODE is defined.
 *	   See step0 for the selection of the node to be traced.
 *   Behaviour or node tracing is only needed in very serious cases.
 *   The macros "debuggingmessage" can be redefined to do some special
 *   tests on every entry.
 *   Assertion checking can always be done, because it is silent
 *   as long as no error occurs. But it consumes runtime.
 *   This all can also be defined in the head of a module before
 *   #include "globals.h", to debug only this module.
 */

/* for internal debugging */
#undef DEBUG
#undef CHECKNODE
/* for assertion checking at runtime */
#define CHECK_ASSERTIONS


/*
 *  If you prefer the system assert.h, then define ASSERT_AVAIL.
 */

/* #define ASSERT_AVAIL   */
#undef  ASSERT_AVAIL

/*  Check and printout the timing of phases.
 *  If CHECK_TIMING is defined, time measurement is done at some
 *  critical points. This is only for me, to fine tune the
 *  preformance. (GS)
 */

#undef CHECK_TIMING

/*---------------------------- End of Changes ------------------------*/


/* Prototype support */

#ifdef ANSI_C
#define	_PP(x) x
#else
#define	_PP(x) ()
#endif


/* Debugging messages */

#ifdef DEBUG
#define debugmessage(a,b) {FPRINTF(stderr,"Debug: %s %s\n",a,b);}
#else
#define debugmessage(a,b) /**/
#endif

#ifdef CHECKNODE
#define DEBUG
#undef debugmessage
#define debugmessage(a,b) { \
	FPRINTF(stderr,"Debug: %s %s &",a,b); \
	if (debug_checknode)  \
		FPRINTF(stderr,"Checknode: |%s|\n",NTITLE(debug_checknode)); \
	FPRINTF(stderr,"\n"); \
}
#endif

/* Assertions */

#ifndef CHECK_ASSERTIONS
#define NDEBUG
#endif
#ifdef DEBUG
#undef NDEBUG
#endif

#include <assert.h>


/* To make lint happy */

#define PRINTF  (void)printf
#define FPRINTF (void)fprintf
#define SPRINTF (void)sprintf
#define FCLOSE  (void)fclose
#define FFLUSH  (void)fflush
#define FREE    (void)free



#define gstoint(x) ((int)(x))

#include <limits.h>
#define MAXINT INT_MAX
#define MININT  INT_MIN

/*--------------------------------------------------------------------*/

#endif /* GLOBALS_H */

