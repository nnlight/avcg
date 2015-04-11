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

#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "graph.h"
#include "alloc.h"


GEDGE FirstPred(GNODE v)
{
    GEDGE e = NADJFIRST(v,GD_PRED);
    if (e) {
        assert(NPRED(v));
        assert(NPRED(v) == EADJENTRY(e,GD_PRED));
        assert(AKANTE(NPRED(v)) == e);
        assert(PrevPred(e) == NULL);
    } else {
        assert(!NPRED(v));
        assert(!LastPred(v));
    }
    return e;
}
GEDGE FirstSucc(GNODE v)
{
    GEDGE e = NADJFIRST(v,GD_SUCC);
    if (e) {
        assert(NSUCC(v));
        assert(NSUCC(v) == EADJENTRY(e,GD_SUCC));
        assert(AKANTE(NSUCC(v)) == e);
        assert(PrevSucc(e) == NULL);
    } else {
        assert(!NSUCC(v));
        assert(!LastSucc(v));
    }
    return e;
}
GEDGE NextPred(GEDGE edge)
{
    GEDGE e = EADJNEXT(edge, GD_PRED);
    assert(AKANTE(EADJENTRY(edge,GD_PRED)) == edge);
    if (e) {
        assert(AKANTE(ANEXT(EADJENTRY(edge,GD_PRED))) == e);
        assert(AKANTE(EADJENTRY(e,GD_PRED)) == e);
    } else {
        assert( !ANEXT(EADJENTRY(edge,GD_PRED)) );
        assert(LastPred(EDST(edge)) == edge);
    }
    if (EADJPREV(edge,GD_PRED)) {
        GEDGE prev_edge = EADJPREV(edge,GD_PRED);
        assert(AKANTE(EADJENTRY(prev_edge,GD_PRED)) == prev_edge);
    } else {
        assert(FirstPred(EDST(edge)) == edge);
        assert(AKANTE(NPRED(EDST(edge))) == edge);
    }
    return e;
}
GEDGE NextSucc(GEDGE edge)
{
    GEDGE e = EADJNEXT(edge, GD_SUCC);
    assert(AKANTE(EADJENTRY(edge,GD_SUCC)) == edge);
    if (e) {
        assert(AKANTE(ANEXT(EADJENTRY(edge,GD_SUCC))) == e);
        assert(AKANTE(EADJENTRY(e,GD_SUCC)) == e);
    } else {
        assert( !ANEXT(EADJENTRY(edge,GD_SUCC)) );
        assert(LastPred(ESRC(edge)) == edge);
    }
    if (EADJPREV(edge,GD_SUCC)) {
        GEDGE prev_edge = EADJPREV(edge,GD_SUCC);
        assert(AKANTE(EADJENTRY(prev_edge,GD_SUCC)) == prev_edge);
    } else {
        assert(FirstSucc(ESRC(edge)) == edge);
        assert(AKANTE(NSUCC(ESRC(edge))) == edge);
    }
    return e;
}

void init_node_graph_fields_as_dead(GNODE v)
{
    NADJFIRST(v,GD_PRED) = DEAD_GEDGE;
    NADJFIRST(v,GD_SUCC) = DEAD_GEDGE;
    NADJLAST(v,GD_PRED) = DEAD_GEDGE;
    NADJLAST(v,GD_SUCC) = DEAD_GEDGE;
    NNEXT(v) = DEAD_GNODE;
    NPREV(v) = DEAD_GNODE;
}

void init_edge_graph_fields_as_dead(GEDGE e)
{
    EADJNEXT(e, GD_PRED) = DEAD_GEDGE;
    EADJNEXT(e, GD_SUCC) = DEAD_GEDGE;
    EADJPREV(e, GD_PRED) = DEAD_GEDGE;
    EADJPREV(e, GD_SUCC) = DEAD_GEDGE;
    ENEXT(e) = DEAD_GEDGE;
    EPREV(e) = DEAD_GEDGE;
}

void init_node_adj_fields(GNODE v)
{
    assert(NADJFIRST(v,GD_PRED) == DEAD_GEDGE);
    assert(NADJFIRST(v,GD_SUCC) == DEAD_GEDGE);
    assert(NADJLAST(v,GD_PRED) == DEAD_GEDGE);
    assert(NADJLAST(v,GD_SUCC) == DEAD_GEDGE);

    NADJFIRST(v,GD_PRED) = NULL;
    NADJFIRST(v,GD_SUCC) = NULL;
    NADJLAST(v,GD_PRED) = NULL;
    NADJLAST(v,GD_SUCC) = NULL;

    NSUCC(v) = NPRED(v) = NULL;
}

static unlink_node_edge(GNODE v, GEDGE e, Graphdir_t dir)
{
    if (dir == GD_PRED) {
        assert(EDST(e) == v);
    } else {
        assert(ESRC(e) == v);
    }
    assert(EADJPREV(e, dir) != DEAD_GEDGE);
    assert(EADJNEXT(e, dir) != DEAD_GEDGE);
    assert(NADJFIRST(v, dir) != DEAD_GEDGE);
    assert(NADJLAST(v, dir) != DEAD_GEDGE);

    if (EADJPREV(e, dir)) EADJPREV(e, dir) = EADJNEXT(e, dir);
    else                  NADJFIRST(v, dir)= EADJNEXT(e, dir);
    if (EADJNEXT(e, dir)) EADJNEXT(e, dir) = EADJPREV(e, dir);
    else                  NADJLAST(v, dir) = EADJPREV(e, dir);

    EADJPREV(e, dir) = DEAD_GEDGE;
    EADJNEXT(e, dir) = DEAD_GEDGE;
    EADJENTRY(e, dir) = NULL;
}

void unlink_node_edges(GNODE v)
{
    GEDGE e, nxt_e;

#if 0
    for (e = FirstSucc(v); e; e = nxt_e)
    {
        nxt_e = NextSucc(e);
        unlink_node_edge(v, e, GD_SUCC);
    }
    for (e = FirstPred(v); e; e = nxt_e)
    {
        nxt_e = NextPred(e);
        unlink_node_edge(v, e, GD_PRED);
    }
#endif
    NSUCC(v) = NPRED(v) = NULL;
}

static void link_node_edge(GNODE v, GEDGE e, Graphdir_t dir)
{
    if (dir == GD_PRED) {
        assert(EDST(e) == v);
    } else {
        assert(ESRC(e) == v);
    }
    assert(EADJPREV(e, dir) == DEAD_GEDGE);
    assert(EADJNEXT(e, dir) == DEAD_GEDGE);
    assert(NADJFIRST(v, dir) != DEAD_GEDGE);
    assert(NADJLAST(v, dir) != DEAD_GEDGE);

    EADJPREV(e,dir) = NULL;
    EADJNEXT(e,dir) = NADJFIRST(v,dir);
    if (NADJFIRST(v,dir)) EADJPREV(NADJFIRST(v,dir),dir) = e;
    else                  NADJLAST(v,dir) = e;
    NADJFIRST(v,dir) = e;
}

/*   Create an adjacency
 *   -------------------
 *   i.e. insert an edge into the adjacency lists of its source and
 *   target node.
 */
void link_edge(GEDGE edge)
{
	ADJEDGE a;
    GNODE v;

    assert(ESTART(edge));
    assert(EEND(edge));

    for (a = NSUCC(ESTART(edge)); a; a = ANEXT(a))
    {
        assert(AKANTE(a) != edge);
    }
    for (a = NPRED(EEND(edge)); a; a = ANEXT(a))
    {
        assert(AKANTE(a) != edge);
    }

    a = prededgealloc(EEND(edge), edge);
    EADJENTRY(edge, GD_PRED) = a;
    link_node_edge(EEND(edge), edge, GD_PRED);

    a = succedgealloc(ESTART(edge), edge);
    EADJENTRY(edge, GD_SUCC) = a;
    link_node_edge(ESTART(edge), edge, GD_SUCC);
}

/*   Delete an adjacency
 *   -------------------
 *   i.e. delete an edge e from the adjacency lists of its source
 *   and target node.
 */
void unlink_edge(GEDGE edge)
{
    ADJEDGE a,b,*ap,*abp;

    assert(edge);
    assert(ESTART(edge));
    assert(EEND(edge));
    a = NSUCC(ESTART(edge));
    ap = &(NSUCC(ESTART(edge)));
    while (a) {
        abp = &ANEXT(a);
        b   = ANEXT(a);
        if (AKANTE(a)==edge) *ap = ANEXT(a); 
        a = b;
        ap = abp;
    }
    a = NPRED(EEND(edge));
    ap = &(NPRED(EEND(edge)));
    while (a) {
        abp = &(ANEXT(a));
        b = ANEXT(a);
        if (AKANTE(a)==edge) *ap = ANEXT(a);
        a = b;
        ap = abp;
    }

    unlink_node_edge(ESTART(edge), edge, GD_SUCC);
    unlink_node_edge(EEND(edge), edge, GD_PRED);
}




