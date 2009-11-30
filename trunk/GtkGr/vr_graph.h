#ifndef _VR_GRAPH_H_
#define _VR_GRAPH_H_

#include "stdafx.h"
#include "draw_buffer.h"
#include "gr.h"
#include "vcg/vcg_iface.h"

using namespace std;





class VRGraph;
/**
 * Узел
 */
class VRNode : public GrNode
{
public:
	string title_;
	string label_;
	int x_,y_;
	int width_, height_;
	Color_t color_;
	Color_t bcolor_; // цвет границы
	int borderw_;    // ширина границы
	int stretch_;
	int shrink_;
public:
	VRNode( VRGraph *graph, const char *title, int x, int y);
	~VRNode();
private:
	VRNode( const VRNode &a);
};

#define VREDGE_DOT_COUNT 4
/**
 * Дуга
 */
class VREdge : public GrEdge
{
public:
	string label_;
	int dots_;
	int x_[VREDGE_DOT_COUNT];
	int y_[VREDGE_DOT_COUNT];
	Linestyle_t linestyle_;
	int thickness_;
	Color_t color_;
	int arrowsize_;
	Arrowstyle_t arrowstyle_;
	Color_t arrowcolor_;
public:
	VREdge( VRGraph *graph);
	~VREdge();
private:
	VREdge( const VRNode &a);
};

/**
 * Visual Repersentation of Graph
 * Хранит граф с позициями узлов и т.п., т.е. описывает то, как должен выглядеть граф. 
 * Линейные размеры не привязаны к пикселям и тому, в каком масшабе сейчас он отображается.
 */
class VRGraph : public GrGraph
{
public:
	VRGraph();
	/* не предназначен для иcпользования в качестве базового класса */
	~VRGraph();

	void AddNode( DrawBuffer *draw_buffer, int x, int y, const char *title, const char *label);
	VRNode *AddSizedNode( int x, int y, int width, int height, const char *title, const char *label);
	void Expose( DrawBuffer *draw_buffer, int x, int y, int width, int height);

	/* загрузка графа из vcg */
	void LoadGDL();
private:
	void LoadVcgEdge( GEDGE e);

	void DrawEdge( DrawBuffer *draw_buffer, VREdge *edge);

};




#endif /* _VR_GRAPH_H_ */
