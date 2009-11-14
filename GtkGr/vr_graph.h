#ifndef _VR_GRAPH_H_
#define _VR_GRAPH_H_

#include "stdafx.h"


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
private:
};




#endif /* _VR_GRAPH_H_ */