#ifndef _VR_GRAPH_H_
#define _VR_GRAPH_H_

#include "stdafx.h"
#include "draw_buffer.h"
#include "gr.h"
#include "vcg/vcg_iface.h"



class VRNode;
class VREdge;
class VRGraph;
#define VRNODE_INFO_COUNT 3
#define NODE_LABEL_MARGIN 3
#define EDGE_LABEL_MARGIN 1

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
    Color_t textcolor_;   //!< цвет текста
    Color_t bcolor_;      //!< цвет границы
    int borderw_;         //!< ширина границы
    Nodeshape_t shape_;   //!< форма узла
    Textmode_t textmode_; //!< центровка текста
    int stretch_;
    int shrink_;
    string infos_[VRNODE_INFO_COUNT];
public:
    VRNode( VRGraph *graph, const char *title, int x, int y);
    ~VRNode();
    VRNode *GetNextNode() { return static_cast<VRNode*>(GrGetNextNode()); }
    VREdge *GetFirstSucc() { return (VREdge *)(GrGetFirstSucc()); }
    int FindInfoNumBySubstring( const char *substring);
private:
    VRNode( const VRNode &a);
};

enum VRDir_t
{
    VRDIR_FORWARD,
    VRDIR_BACKWARD,
    VRDIR_LAST
};
/**
 * Дуга
 */
class VREdge : public GrEdge
{
public:
    //string label_;
    int dots_;
    vector<int> x_;
    vector<int> y_;
    Linestyle_t linestyle_;
    int thickness_;
    Color_t color_;
    int arrowsize_[VRDIR_LAST];
    Arrowstyle_t arrowstyle_[VRDIR_LAST];
    Color_t arrowcolor_[VRDIR_LAST];
    bool is_endpoints_corrected_;
    double saved_endpoint_x_[VRDIR_LAST];
    double saved_endpoint_y_[VRDIR_LAST];
public:
    VREdge( VRGraph *graph);
    VREdge( VRGraph *graph, GEDGE e);
    ~VREdge();
    VREdge *GetNextSucc() { return static_cast<VREdge*>(GrGetNextSucc()); }
    void SetArrowAttrsFromVcgEdge( VRDir_t dir, GEDGE e);
    void AddDot( int x, int y);
    void AddDotsFromVcgEdge( GEDGE e);
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
private:
    struct VRInfoBox
    {
        int x_, y_;
        int width_, height_;
        VRNode *node_;
        int info_num_;
    };
    std::list<VRInfoBox> ibox_list_;

    struct EdgeLabel
    {
        int x_, y_;
        int width_, height_;
        string label_;
        Color_t color_;
        Color_t textcolor_; //!< цвет текста
    };
    std::list<EdgeLabel> elabel_list_;
public:
    VRGraph();
    /* не предназначен для иcпользования в качестве базового класса */
    ~VRGraph();

    VRNode *GetFirstNode() { return static_cast<VRNode*>(GrGetFirstNode()); }
    VREdge *GetDummyNodeFirstSucc() { return static_cast<VREdge*>(GrGetDummyNode()->GrGetFirstSucc());}

    void AddNode( DrawBuffer *draw_buffer, int x, int y, const char *title, const char *label);
    VRNode *AddSizedNode( int x, int y, int width, int height, const char *title, const char *label);
    void DrawGraph( DrawBuffer *draw_buffer);

    void HandleInfoBoxPress( DrawBuffer *draw_buffer, int x, int y, int info_num);

    /* загрузка графа из vcg */
    void LoadVcgGraph();
    /* настройка DrawBuffer по установке цвета фона, доп. цветов, начальный скейлинг и положение видимой области и т.п. */
    void SetupDrawBufferSetting( DrawBuffer *draw_buffer);
private:
    void LoadVcgEdge( GEDGE e, bool ignore_back_arrow = false);
    int GetVcgNodeAnchorsFirstY( GNODE v);
    int GetVcgNodeAnchorX( GNODE n, int y, GEDGE conn_e);
    void LoadVcgEdgesForVcgAnchorNode( GNODE v, Marker_t marker, Tempattr_t node_ta);
    void LoadVcgEdgesForVcgNodeList( GNODE list);

    void LinkConnectEdgesAndRevertRevertedForVcgNodeList( GNODE list, std::list<GEDGE> &rev_e);
    void UnlinkConnectEdgesForVcgNodeList( GNODE list);
    void RevertBackVcgEdges(std::list<GEDGE> &rev_e);
    void AddVcgEdgesChainToEdge( GEDGE e, VREdge *edge, Marker_t marker, Tempattr_t node_ta);
    void LoadVcgEdges( Marker_t marker, Tempattr_t node_ta);

    void DrawNodeText( DrawBuffer *draw_buffer, VRNode *node);
    void DrawNode( DrawBuffer *draw_buffer, VRNode *node);
    void CorrectEdgeEndpointDot( VREdge *edge, VRDir_t dir);
    void DrawEdgeArrow( DrawBuffer *draw_buffer, VREdge *edge, VRDir_t dir);
    void DrawEdge( DrawBuffer *draw_buffer, VREdge *edge);
    void DrawInfoBox( DrawBuffer *draw_buffer, VRInfoBox *ibox);
    void DrawEdgeLabel( DrawBuffer *draw_buffer, EdgeLabel *elabel);

};




#endif /* _VR_GRAPH_H_ */
