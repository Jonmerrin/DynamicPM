#ifndef QEM_FACE
#define QEM_FACE

#include "glm/glm.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"


struct Vertex;
struct Edge;

struct Face
{
    // Instead of positions use vertex indices and a pointer to the associated Geometry
    // Then add a getter to Geometry.

    // Total space: 72b * 3 * 2n = 216nb
    
    Edge* edges[3]; // 8b * 3
    Vertex* vertices[3];// 8b * 3
    Face* next; // 8b* 3
    Face* prev;
    Face(Vertex* _v0, Vertex* _v1, Vertex* _v2, Edge* _e1, Edge* _e2, Edge* _e3){ vertices[0] = _v0; vertices[1] = _v1; vertices[2] = _v2; edges[0] = _e1; edges[1] = _e2; edges[2] = _e3;}

    // Tests if two Edges are equal
	bool operator== (const Face &rhs){
        // If a triangle shares 2 edges, it must share the 3rd.
		if( edges[0] == rhs.edges[0] && edges[1] == rhs.edges[1] ){
			return true;
		}
		return false;
	}
    
};

#endif