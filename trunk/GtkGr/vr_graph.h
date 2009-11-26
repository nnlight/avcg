#ifndef _VR_GRAPH_H_
#define _VR_GRAPH_H_

#include "stdafx.h"
#include "draw_buffer.h"

using namespace std;

enum Color_t
{
	WHITE        = 0,
	BLUE         = 1,
	RED          = 2,
	GREEN        = 3,
	YELLOW       = 4,
	MAGENTA      = 5,
	CYAN         = 6,
	DARKGREY     = 7,
	DARKBLUE     = 8,
	DARKRED      = 9,
	DARKGREEN    = 10,
	DARKYELLOW   = 11,
	DARKMAGENTA  = 12,
	DARKCYAN     = 13,
	GOLD         = 14,
	LIGHTGREY    = 15,
	LIGHTBLUE    = 16,
	LIGHTRED     = 17,
	LIGHTGREEN   = 18,
	LIGHTYELLOW  = 19,
	LIGHTMAGENTA = 20,
	LIGHTCYAN    = 21,
	LILAC        = 22,
	TURQUOISE    = 23,
	AQUAMARINE   = 24,
	KHAKI        = 25,
	PURPLE       = 26,
	YELLOWGREEN  = 27,
	PINK         = 28,
	ORANGE       = 29,
	ORCHID       = 30,
	BLACK        = 31,
	/* number of colors */
	BASECMAPSIZE = 32,
	CMAPSIZE     = 256
};

/**
 * Узел
 */
class VRNode
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
	VRNode( const char *title, int x, int y);
	~VRNode();
private:
	VRNode( const VRNode &a);
};

/**
 * Visual Repersentation of Graph
 * Хранит граф с позициями узлов и т.п., т.е. описывает то, как должен выглядеть граф. 
 * Линейные размеры не привязаны к пикселям и тому, в каком масшабе сейчас он отображается.
 */
class VRGraph
{
public:
	VRGraph();
	/* не предназначен для иcпользования в качестве базового класса */
	~VRGraph();

	void AddNode( DrawBuffer *draw_buffer, int x, int y, const char *title, const char *label);
	void Expose( DrawBuffer *draw_buffer, int x, int y, int width, int height);
private:
	typedef list<VRNode*> nodes_t;
	list<VRNode*> nodes_;
};




#endif /* _VR_GRAPH_H_ */