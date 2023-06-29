#ifndef VERTEX_SPLIT
#define VERTEX_SPLIT

#include "glm/glm.hpp"
#include <vector>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Face.hpp"


struct VertexSplit
{
    Vertex* v;
    Vertex* v2;
    Edge* e;
    Edge* activeLeftEdge;
    Edge* activeRightEdge;
    Edge* consumedLeftEdge;
    Edge* consumedRightEdge;
    int leftCounter;
    int rightCounter;
    glm::vec3 v1Pos;
    
    VertexSplit(Vertex* _v1, Vertex* _v2, glm::vec3 _pos1, Edge* _e, Edge* _leftEdge, 
                Edge* _rightEdge, Edge* _consumedLeftEdge, Edge* _consumedRightEdge,
                int _leftCounter, int _rightCounter):
                v(_v1), v2(_v2), v1Pos(_pos1), e(_e), activeLeftEdge(_leftEdge),
                activeRightEdge(_rightEdge), consumedLeftEdge(_consumedLeftEdge),
                consumedRightEdge(_consumedRightEdge), leftCounter(_leftCounter),
                rightCounter(_rightCounter) {};

    VertexSplit(){}

};


#endif