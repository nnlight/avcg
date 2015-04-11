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






