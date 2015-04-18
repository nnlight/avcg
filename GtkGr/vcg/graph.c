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
#include "vcg_defs.h"


#if !VCG_USE_MACROS
int forward_connection1(CONNECT c) {
	GEDGE e = CEDGE(c);
	if (e) {
		assert(CTARGET(c) == ESTART(e) || CTARGET(c) == EEND(e));
	}
	return ((CEDGE(c))&& (EEND(CEDGE(c)) ==CTARGET(c)));
}
int forward_connection2(CONNECT c) {
	GEDGE e = CEDGE2(c);
	if (e) {
		assert(CTARGET2(c) == ESTART(e) || CTARGET2(c) == EEND(e));
	}
	return ((CEDGE2(c))&&(EEND(CEDGE2(c))==CTARGET2(c)));
}
int backward_connection1(CONNECT c) {
	GEDGE e = CEDGE(c);
	if (e) {
		assert(CTARGET(c) == ESTART(e) || CTARGET(c) == EEND(e));
	}
	return ((CEDGE(c))&& (EEND(CEDGE(c)) !=CTARGET(c)));
}
int backward_connection2(CONNECT c) {
	GEDGE e = CEDGE2(c);
	if (e) {
		assert(CTARGET2(c) == ESTART(e) || CTARGET2(c) == EEND(e));
	}
	return ((CEDGE2(c))&&(EEND(CEDGE2(c))!=CTARGET2(c)));
}
#endif /* !VCG_USE_MACROS */

#if !VCG_USE_MACROS
GEDGE FirstPred(GNODE v)
{
    GEDGE e = NADJFIRST(v,GD_PRED);
    if (e) {
        assert(NPRED(v));
        assert(NPRED(v) == EADJENTRY(e,GD_PRED));
        assert(AKANTE(NPRED(v)) == e);
        assert(PrevPred(e) == NULL);
        assert(ETARGET(e) == v);
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
        assert(ESOURCE(e) == v);
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
        assert(ETARGET(e) == ETARGET(edge));
    } else {
        assert( !ANEXT(EADJENTRY(edge,GD_PRED)) );
        assert(LastPred(ETARGET(edge)) == edge);
    }
    if (EADJPREV(edge,GD_PRED)) {
        GEDGE prev_edge = EADJPREV(edge,GD_PRED);
        assert(AKANTE(EADJENTRY(prev_edge,GD_PRED)) == prev_edge);
    } else {
        assert(FirstPred(ETARGET(edge)) == edge);
        assert(AKANTE(NPRED(ETARGET(edge))) == edge);
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
        assert(ESOURCE(e) == ESOURCE(edge));
    } else {
        assert( !ANEXT(EADJENTRY(edge,GD_SUCC)) );
        assert(LastSucc(ESOURCE(edge)) == edge);
    }
    if (EADJPREV(edge,GD_SUCC)) {
        GEDGE prev_edge = EADJPREV(edge,GD_SUCC);
        assert(AKANTE(EADJENTRY(prev_edge,GD_SUCC)) == prev_edge);
    } else {
        assert(FirstSucc(ESOURCE(edge)) == edge);
        assert(AKANTE(NSUCC(ESOURCE(edge))) == edge);
    }
    return e;
}
#endif /* !VCG_USE_MACROS */

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

void check_node_no_adj_edges(GNODE v)
{
    assert(NADJFIRST(v,GD_PRED) == NULL);
    assert(NADJFIRST(v,GD_SUCC) == NULL);
    assert(NADJLAST(v,GD_PRED) == NULL);
    assert(NADJLAST(v,GD_SUCC) == NULL);

    assert(NSUCC(v) == NULL);
    assert(NPRED(v) == NULL);
}

static void link_node_edge(GNODE v, GEDGE e, Graphdir_t dir)
{
    GEDGE first = NADJFIRST(v, dir);

    if (dir == GD_PRED) {
        assert(ETARGET(e) == v);
    } else {
        assert(ESOURCE(e) == v);
    }
    assert(EADJPREV(e, dir) == DEAD_GEDGE);
    assert(EADJNEXT(e, dir) == DEAD_GEDGE);
    assert(NADJFIRST(v, dir) != DEAD_GEDGE);
    assert(NADJLAST(v, dir) != DEAD_GEDGE);

#if 0
    EADJPREV(e,dir) = NULL;
    EADJNEXT(e,dir) = NADJFIRST(v,dir);
    if (NADJFIRST(v,dir)) EADJPREV(NADJFIRST(v,dir),dir) = e;
    else                  NADJLAST(v,dir) = e;
    NADJFIRST(v,dir) = e;
#else
	/* добавляем в начало списка дуг (в направлении dir) */
    if (first)
    {
        EADJPREV(first, dir) = e;
        EADJPREV(e, dir) = NULL;
        EADJNEXT(e, dir) = first;
        NADJFIRST(v, dir) = e;
    } else
    {
        /* тогда и последний элемент списка должен быть нулевым, т.е. список пустой */
        assert(NADJLAST(v, dir) == NULL);
        EADJPREV(e, dir) = NULL;
        EADJNEXT(e, dir) = NULL;
        NADJFIRST(v, dir) = e;
        NADJLAST(v, dir) = e;
    }
#endif
}

static void link_node_edge_as_last(GNODE v, GEDGE e, Graphdir_t dir)
{
    GEDGE last = NADJLAST(v, dir);

    if (dir == GD_PRED) {
        assert(ETARGET(e) == v);
    } else {
        assert(ESOURCE(e) == v);
    }
    assert(EADJPREV(e, dir) == DEAD_GEDGE);
    assert(EADJNEXT(e, dir) == DEAD_GEDGE);
    assert(NADJFIRST(v, dir) != DEAD_GEDGE);
    assert(NADJLAST(v, dir) != DEAD_GEDGE);

	/* добавляем в конец списка дуг (в направлении dir) */
    if (last)
    {
        EADJNEXT(last, dir) = e;
        EADJPREV(e, dir) = last;
        EADJNEXT(e, dir) = NULL;
        NADJLAST(v, dir) = e;
    } else
    {
        /* тогда и первый элемент списка должен быть нулевым, т.е. список пустой */
        assert(NADJFIRST(v, dir) == NULL);
        EADJPREV(e, dir) = NULL;
        EADJNEXT(e, dir) = NULL;
        NADJFIRST(v, dir) = e;
        NADJLAST(v, dir) = e;
    }
}

static void unlink_node_edge(GNODE v, GEDGE e, Graphdir_t dir)
{
    GEDGE prev = EADJPREV(e, dir);
    GEDGE next = EADJNEXT(e, dir);

    if (dir == GD_PRED) {
        assert(ETARGET(e) == v);
    } else {
        assert(ESOURCE(e) == v);
    }
    assert(EADJPREV(e, dir) != DEAD_GEDGE);
    assert(EADJNEXT(e, dir) != DEAD_GEDGE);
    assert(NADJFIRST(v, dir) != DEAD_GEDGE);
    assert(NADJLAST(v, dir) != DEAD_GEDGE);

#if 0
    if (EADJPREV(e, dir)) EADJNEXT(EADJPREV(e, dir),dir) = EADJNEXT(e, dir);
    else                  NADJFIRST(v, dir)= EADJNEXT(e, dir);
    if (EADJNEXT(e, dir)) EADJPREV(EADJNEXT(e, dir),dir) = EADJPREV(e, dir);
    else                  NADJLAST(v, dir) = EADJPREV(e, dir);
#else
	if (prev)
	{
        EADJNEXT(prev, dir) = next;
	} else
	{
        assert(NADJFIRST(v, dir) == e);
        NADJFIRST(v, dir) = next;
	}
	if (next)
	{
        EADJPREV(next, dir) = prev;
	} else
	{
        assert(NADJLAST(v, dir) == e);
        NADJLAST(v, dir) = prev;
	}
#endif

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
#else
    for (e = NADJFIRST(v,GD_SUCC); e; e = nxt_e)
    {
        nxt_e = EADJNEXT(e, GD_SUCC);
        unlink_node_edge(v, e, GD_SUCC);
    }
    for (e = NADJFIRST(v,GD_PRED); e; e = nxt_e)
    {
        nxt_e = EADJNEXT(e, GD_PRED);
        unlink_node_edge(v, e, GD_PRED);
    }
#endif
    NSUCC(v) = NPRED(v) = NULL;
}

/*   Create an adjacency
 *   -------------------
 *   i.e. insert an edge into the adjacency lists of its source and
 *   target node.
 */
void link_edge(GEDGE edge)
{
    ADJEDGE a;

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
    int i = 0;

    assert(edge);
    assert(ESTART(edge));
    assert(EEND(edge));
    a = NSUCC(ESTART(edge));
    ap = &(NSUCC(ESTART(edge)));
    while (a) {
        abp = &ANEXT(a);
        b   = ANEXT(a);
        if (AKANTE(a)==edge) { *ap = ANEXT(a); i++; }
        a = b;
        ap = abp;
    }
    assert(i == 1);
    i = 0;
    a = NPRED(EEND(edge));
    ap = &(NPRED(EEND(edge)));
    while (a) {
        abp = &(ANEXT(a));
        b = ANEXT(a);
        if (AKANTE(a)==edge) { *ap = ANEXT(a); i++; }
        a = b;
        ap = abp;
    }
    assert(i == 1);

    unlink_node_edge(ESTART(edge), edge, GD_SUCC);
    unlink_node_edge(EEND(edge), edge, GD_PRED);
}

/**
 * Перенос дуги на другой источник.
 */
void change_edge_src(GEDGE e, GNODE v, GNODE new_v)
{
    ADJEDGE a,b,*ap,*abp;
    int i = 0;

    assert(v == ESOURCE(e));

    a = NSUCC(v);
    ap = &(NSUCC(v));
    while (a) {
        abp = &ANEXT(a);
        b   = ANEXT(a);
        if (AKANTE(a)==e) { *ap = ANEXT(a); i++; }
        a = b;
        ap = abp;
    }
    assert(i == 1);
    unlink_node_edge(v, e, GD_SUCC);

    ESOURCE(e) = new_v;
    a = succedgealloc(new_v, e);
    EADJENTRY(e, GD_SUCC) = a;
    link_node_edge(new_v, e, GD_SUCC);
}

/**
 * Перенос дуги на другой приемник.
 */
void change_edge_dst(GEDGE e, GNODE v, GNODE new_v)
{
    ADJEDGE a,b,*ap,*abp;
    int i = 0;

    assert(v == ETARGET(e));

    a = NPRED(v);
    ap = &(NPRED(v));
    while (a) {
        abp = &(ANEXT(a));
        b = ANEXT(a);
        if (AKANTE(a)==e) { *ap = ANEXT(a); i++; }
        a = b;
        ap = abp;
    }
    assert(i == 1);
    unlink_node_edge(v, e, GD_PRED);

    ETARGET(e) = new_v;
    a = prededgealloc(new_v, e);
    EADJENTRY(e, GD_PRED) = a;
    link_node_edge(new_v, e, GD_PRED);
}

void relink_node_edge_as_first(GNODE v, GEDGE e, Graphdir_t dir)
{
    if (dir == GD_PRED) {
        assert(ETARGET(e) == v);
        change_edge_dst(e, v, v);
    } else {
        assert(ESOURCE(e) == v);
        change_edge_src(e, v, v);
    }
}

void relink_node_edge_as_last(GNODE v, GEDGE e, Graphdir_t dir)
{
    ADJEDGE a,b,*ap,*abp;
    int i = 0;
    ADJEDGE last_a = NULL;

    if (dir == GD_PRED) {
        assert(ETARGET(e) == v);

        a = NPRED(v);
        ap = &(NPRED(v));
        while (a) {
            abp = &(ANEXT(a));
            b = ANEXT(a);
            if (AKANTE(a)==e) { *ap = ANEXT(a); i++; }
            a = b;
            ap = abp;
        }
        assert(i == 1);
        unlink_node_edge(v, e, GD_PRED);

        for (a = NPRED(v); a; a = ANEXT(a)) { last_a = a; }
    	a = edgelist_alloc();
        if (last_a) {
            ANEXT(last_a) = a;
        } else {
            NPRED(v) = a;
        }
    	AKANTE(a) = e;
	    ANEXT(a) = NULL;
        EADJENTRY(e, GD_PRED) = a;
        link_node_edge_as_last(v, e, GD_PRED);

    } else {
        assert(ESOURCE(e) == v);

        a = NSUCC(v);
        ap = &(NSUCC(v));
        while (a) {
            abp = &ANEXT(a);
            b   = ANEXT(a);
            if (AKANTE(a)==e) { *ap = ANEXT(a); i++; }
            a = b;
            ap = abp;
        }
        assert(i == 1);
        unlink_node_edge(v, e, GD_SUCC);

        for (a = NSUCC(v); a; a = ANEXT(a)) { last_a = a; }
    	a = edgelist_alloc();
        if (last_a) {
            ANEXT(last_a) = a;
        } else {
            NSUCC(v) = a;
        }
    	AKANTE(a) = e;
	    ANEXT(a) = NULL;
        EADJENTRY(e, GD_SUCC) = a;
        link_node_edge_as_last(v, e, GD_SUCC);
    }
}

ADJEDGE save_node_adjlist(GNODE v, Graphdir_t dir)
{
    GEDGE e;
    ADJEDGE a;
    ADJEDGE prev_a = NULL;
    ADJEDGE res = NULL;

    if (dir == GD_PRED)
    {
        for (e = FirstPred(v); e; e = NextPred(e))
        {
            a = edgelist_alloc();
            AKANTE(a) = e;
            ANEXT(a) = NULL;
            if (prev_a) {
                ANEXT(prev_a) = a;
            } else {
                assert(res == NULL);
                res = a;
            }
            prev_a = a;
        }
    } else
    {
        for (e = FirstSucc(v); e; e = NextSucc(e))
        {
            a = edgelist_alloc();
            AKANTE(a) = e;
            ANEXT(a) = NULL;
            if (prev_a) {
                ANEXT(prev_a) = a;
            } else {
                assert(res == NULL);
                res = a;
            }
            prev_a = a;
        }
    }
    return res;
}

ADJEDGE reverse_adjlist(ADJEDGE list)
{
    ADJEDGE a, prev_a, next_a;

    prev_a = NULL;
    for (a = list; a; a = next_a)
    {
        next_a = ANEXT(a);
        ANEXT(a) = prev_a;
        prev_a = a;
    }
    return prev_a;
}

void delete_adjlist(ADJEDGE list)
{
    /* TODO! */
}

void print_node_succs(GNODE v)
{
	GEDGE e;

	printf("===========\n");
	for (e = FirstSucc(v); e; e = NextSucc(e))
	{
		printf("(%s) -> (%s)\n", NTITLE(ESOURCE(e)), NTITLE(ETARGET(e)));
	}
	printf("============\n");
}


