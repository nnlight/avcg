/* ex: set sw=4 expandtab tabstop=4: */
/*--------------------------------------------------------------------*/
/*              VCG : Visualization of Compiler Graphs                */
/*--------------------------------------------------------------------*/
/*
 * Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                             the Compare Consortium
 * Copyright (C) 2015 Nikita S <nnlight@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/* Global definitions and setup */

/*   This file contains the setup of the tool for the C compiler.
 *   It is included into every other C file.
 *   Please adapt corresponding to your configuration.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

/*------------- Please change according to your configuration --------*/

/* Good quicksort available or not ?
 * If there exist a good randomized quicksort `qsort' in the library,
 * please use it. But if the quicksort in the library is slow, or not
 * randomized, define OWN_QUICKSORT. We often have to sort presorted
 * arrays.
 * Note: If OWN_QUICKSORT is defined, not all calls of `qsort' are
 * exchanged, but only the critical ones.
 */

/* #undef OWN_QUICKSORT */
#define OWN_QUICKSORT



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



/*   Debugging On or Off ?
 *   We have three kinds of debugging:
 *      a) Behaviour tracing: a lot of messages nearly on every
 *         function entry are produced if DEBUG is switched on.
 *      b) Assertion checking: only if an assertion fails, an
 *         appropriate message is produced.
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
/* for assertion checking at runtime */
#define CHECK_ASSERTIONS


/*---------------------------- End of Changes ------------------------*/


/* Prototype support */

#define _PP(x) x


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
#define MAXINT  INT_MAX
#define MININT  INT_MIN


/* Check whether the percentual time limit is reached
 * --------------------------------------------------
 * Return true (1) if yes.
 */
#define test_timelimit(perc) 0

/*--------------------------------------------------------------------*/
/**
 * Значание для отладочных целей.
 * Указатель с таким значеним никогда не должен разименовываться.
 */
#define DEAD_PTR    ((void*)0xDEAD0003DEAD0001ll)
#define DEAD_GNODE  ((GNODE)DEAD_PTR)
#define DEAD_GEDGE  ((GEDGE)DEAD_PTR)
#define DEAD_GELIST ((ADJEDGE)DEAD_PTR)
#define DEAD_GNLIST ((GNLIST)DEAD_PTR)

/**
 * Использовать ли для некоторых функций макрос-версии?
 */
#define VCG_USE_MACROS 0

/*--------------------------------------------------------------------*/
#ifdef WIN32
#pragma warning (disable: 4996)     // disable deprecated functions warning
#endif

#endif /* GLOBALS_H */

