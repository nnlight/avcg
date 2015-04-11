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

#ifndef GRAPH_H
#define GRAPH_H

#include "globals.h"
#include "gdl_defs.h"
#include "vcg_defs.h"

/*--------------------------------------------------------------------*/

GEDGE FirstPred(GNODE v);
GEDGE FirstSucc(GNODE v);
GEDGE NextPred(GEDGE e);
GEDGE NextSucc(GEDGE e);

void init_node_graph_fields_as_dead(GNODE v);
void init_edge_graph_fields_as_dead(GEDGE e);
void init_node_adj_fields(GNODE v);

void unlink_node_edges(GNODE v);
void link_edge(GEDGE e);
void unlink_edge(GEDGE e);

/*--------------------------------------------------------------------*/
/**
 * Abbreviation to delete and insert nodes in double linked lists.
 */
#define del_node_from_dl_list(v,l,le) \
{ \
    if (NPREV(v)) NNEXT(NPREV(v)) = NNEXT(v); \
    else          l               = NNEXT(v); \
    if (NNEXT(v)) NPREV(NNEXT(v)) = NPREV(v); \
    else          le              = NPREV(v); \
    NPREV(v) = DEAD_GNODE;                    \
    NNEXT(v) = DEAD_GNODE;                    \
}

#define ins_node_in_dl_list(v,l,le) \
{ \
    assert(NPREV(v) == DEAD_GNODE); \
    assert(NNEXT(v) == DEAD_GNODE); \
    NPREV(v) = le;                  \
    NNEXT(v) = NULL;                \
    if (le) NNEXT(le) = v;          \
    le = v;                         \
    if (!l) l = v;                  \
}

#define ins_node_in_dl_list_before(v,l,le,h) \
{ \
    assert(NPREV(v) == DEAD_GNODE); \
    assert(NNEXT(v) == DEAD_GNODE); \
                                    \
    if (h)  NPREV(v) = NPREV(h);        \
    else    NPREV(v) = le;              \
    NNEXT(v) = h;                       \
    {                                   \
    GNODE prev_h = h ? NPREV(h) : le;   \
 \
    if (prev_h) NNEXT(prev_h) = v;      \
    else        l = v;                  \
    if (h)  NPREV(h) = v;               \
    else    le       = v;               \
    }                                   \
}

#define ins_edge_in_dl_list(e,l,le) \
{ \
    assert(EPREV(e) == DEAD_GEDGE); \
    assert(ENEXT(e) == DEAD_GEDGE); \
    EPREV(e) = le;                  \
    ENEXT(e) = NULL;                \
    if (le) ENEXT(le) = e;          \
    le = e;                         \
    if (!l) l = e;                  \
}

/*--------------------------------------------------------------------*/

#endif /* GRAPH_H */

