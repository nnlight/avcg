#ifndef _GR_H_
#define _GR_H_

#include <stdlib.h>
#include <assert.h>

/* направление (в топологии графа) */
enum GrDir_t
{
	GR_DIR_UP = 0,
	GR_DIR_DOWN = 1,
	GR_DIR_LAST
};

/* направление (в списках) */
enum GrListDir_t
{
	GR_LIST_DIR_LEFT = 0,
	GR_LIST_DIR_RIGHT = 1,
	GR_LIST_DIR_LAST
};

inline GrDir_t GrReverseDir( GrDir_t dir)
{
	assert( dir == 0 || dir == 1 );
	return (GrDir_t)(1 - dir);
}

typedef unsigned GrMarker_t;
typedef unsigned short GrMarkerValueType_t;
#define GR_MARKER_VALUE_BITSIZE  16
#define GR_MARKER_VALUE_MASK     ((1u << GR_MARKER_VALUE_BITSIZE) - 1)
#define GR_MARKER_VALUE_MAX      ((1u << GR_MARKER_VALUE_BITSIZE) - 1)
#define GR_MARKERS_COUNT	4 // можно увеличивать по мере необходимости
#define GR_MARKER_IDX(marker)  ((marker) >> GR_MARKER_VALUE_BITSIZE)
#define GR_MARKER_VAL(marker)  ((marker) & GR_MARKER_VALUE_MASK)
#define GR_MARKER( idx, val)   ( ((idx) << GR_MARKER_VALUE_BITSIZE) | (val) )


class GrNode;
class GrEdge;
class GrGraph;
/** 
 * Узел
 */
class GrNode
{
public:
	GrNode( GrGraph *graph);
	virtual ~GrNode(void);

	/* обход узлов графа */
	GrNode *GrGetNextNode() { return adjnode[GR_LIST_DIR_RIGHT]; };

	/* обход входящих-выходящих дуг узла */
	GrEdge *GrGetFirstEdge( GrDir_t dir) { return edges[dir][GR_LIST_DIR_LEFT]; };
	GrEdge *GrGetFirstSucc() { return GrGetFirstEdge( GR_DIR_DOWN); };
	GrEdge *GrGetFirstPred() { return GrGetFirstEdge( GR_DIR_UP); };

	/* работа с маркером */
	bool IsMarked( GrMarker_t mar) { return markers_val[GR_MARKER_IDX(mar)] == GR_MARKER_VAL(mar); };
	void SetMarker( GrMarker_t mar) { markers_val[GR_MARKER_IDX(mar)] = GR_MARKER_VAL(mar); };
	void ResetMarker( GrMarker_t mar) { markers_val[GR_MARKER_IDX(mar)] = 0; };

	/* проверка, что это технический узел */
	bool IsDummy();

private:
	GrGraph *graph_;
	GrEdge *edges[GR_DIR_LAST][GR_LIST_DIR_LAST]; // инцидентные дуги
	GrNode *adjnode[GR_LIST_DIR_LAST]; // список узлов графа
	GrMarkerValueType_t markers_val[GR_MARKERS_COUNT];
private:
	void IncludeEdgeInList( GrEdge *node, GrDir_t dir);
	void ExcludeEdgeFromList( GrEdge *node, GrDir_t dir);
private:
	friend class GrGraph;
	friend class GrEdge;
};


/** 
 * Дуга
 */
class GrEdge
{
public:
	GrEdge( GrNode *src, GrNode *dst);
	virtual ~GrEdge(void);

	/* получение (инцидентных)узлов дуги */
	GrNode *GrGetNode( GrDir_t dir) { return node[dir]; };
	GrNode *GrGetSucc() { return GrGetNode( GR_DIR_DOWN); };
	GrNode *GrGetPred() { return GrGetNode( GR_DIR_UP); };

	/* обход входящих-выходящих дуг узла */
	GrEdge *GrGetNextEdge( GrDir_t dir) { return adjedge[dir][GR_LIST_DIR_RIGHT]; };
	GrEdge *GrGetNextSucc() { return GrGetNextEdge( GR_DIR_DOWN); };
	GrEdge *GrGetNextPred() { return GrGetNextEdge( GR_DIR_UP); };

	/* перенаправление дуг */
	void GrChangeNode( GrDir_t dir, GrNode *new_node);
	void GrChangeSucc( GrNode *new_succ) { GrChangeNode( GR_DIR_DOWN, new_succ); };
	void GrChangePred( GrNode *new_pred) { GrChangeNode( GR_DIR_UP, new_pred); };

	/* работа с маркером */
	bool IsMarked( GrMarker_t mar) { return markers_val[GR_MARKER_IDX(mar)] == GR_MARKER_VAL(mar); };
	void SetMarker( GrMarker_t mar) { markers_val[GR_MARKER_IDX(mar)] = GR_MARKER_VAL(mar); };
	void ResetMarker( GrMarker_t mar) { markers_val[GR_MARKER_IDX(mar)] = 0; };

private:
	GrNode *node[GR_DIR_LAST];              // инцидентные узлы
	GrEdge *adjedge[GR_DIR_LAST][GR_LIST_DIR_LAST]; // список входных-выходных дуг
	GrMarkerValueType_t markers_val[GR_MARKERS_COUNT];
private:
	friend class GrNode;
};


/** 
 * Граф
 */
class GrGraph
{
public:
	GrGraph(void);
	virtual ~GrGraph(void);

	/* для возможности реализации произвольной стратегии аллокации в пользовательском графе
	 * (т.к. сам граф аллоцированием не занимается) */
	virtual void DeleteNode( GrNode *node) { delete node; };
	virtual void DeleteEdge( GrEdge *edge) { delete edge; };

	/* обход узлов графа */
	GrNode *GrGetFirstNode() { return nodes[GR_LIST_DIR_LEFT]; };

	/* выделение освобождение маркеров */
	GrMarker_t AllocMarker();
	void FreeMarker( GrMarker_t marker);

	/* получение техничекого узла, если хочется создать дугу "в никуда" */
	GrNode *GrGetDummyNode() { return &dummy_node_; };

private:
	GrNode *nodes[GR_LIST_DIR_LAST];  // первый-последний узел графа
	bool busy_markers[GR_MARKERS_COUNT];
	GrMarkerValueType_t markers_init_val[GR_MARKERS_COUNT];
	GrNode dummy_node_;  // для привязки дуг "в никуда"
private:
	void IncludeNodeInList( GrNode *node);
	void ExcludeNodeFromList( GrNode *node);
private:
	friend class GrNode;
};




#endif /* _GR_H_ */
