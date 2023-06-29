#ifndef QEM_EDGE
#define QEM_EDGE

#include "glm/glm.hpp"
#include "Vertex.hpp"
#include "Face.hpp"
#include <iostream>


struct Vertex;
struct Face;

struct Edge
{
    // Total space: 42b * 3n = 126n
    // Instead of positions use vertex indices and a pointer to the associated Geometry
    // Then add a getter to Geometry.
    Vertex* v1; // 8b
    Vertex* v2; // 8b
    Face* f1 = nullptr; // 8b
    Face* f2 = nullptr; // 8b
    int counter = 0; // 4b
    Edge* prev = nullptr;
    Edge* next = nullptr; // 8b


    Edge(Vertex* _v1, Vertex* _v2): 
    v1(_v1), v2(_v2) {}
    
    Edge(Vertex* _v1, Vertex* _v2, Face* _f1): 
    v1(_v1), v2(_v2), f1(_f1) { }
    
    Edge(Vertex* _v1, Vertex* _v2, Face* _f1, Face* _f2): 
    v1(_v1), v2(_v2), f1(_f1), f2(_f2) { }

    // Tests if two Edges are equal
	bool operator== (const Edge &rhs){
		if( (v1 == rhs.v2 && v2 == rhs.v1) || (v1 == rhs.v1 && v2 == rhs.v2) ){
			return true;
		}
		return false;
	}

    void AddFace(Face* face_index) {
        if(f1 == nullptr) {
            f1 = face_index;
        } else {
            f2 = face_index;
        }
    }
    
};

#endif