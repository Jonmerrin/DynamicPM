#ifndef QEM_VERTEX
#define QEM_VERTEX

#include "glm/glm.hpp"
#include "VertexSplit.hpp"

struct Vertex
{
    // Instead of positions use vertex indices and a pointer to the associated Geometry
    // Then add a getter to Geometry.

    // Total space: (12 + 8 + 1 + 8 + 8)b * n = 37n

    // 4n*3
    glm::vec3 pos;
    glm::vec2 tex;
    // 8n
    VertexSplit* vsplit = nullptr;
    // n
    bool shouldCollapse = false;
    // Vertex* nextActive = nullptr;
    // 8n
    Vertex* next = nullptr;
    // 8n
    Vertex* prev = nullptr;
    // 8n
    Vertex* ActiveVertex = nullptr;
    glm::mat4 Q = glm::mat4(0);
    int ind;
    Vertex(glm::vec3 _pos, glm::vec2 _tex):pos(_pos), tex(_tex){}

    // Tests if two Edges are equal
	bool operator== (const Vertex &rhs){
        // If a triangle shares 2 edges, it must share the 3rd.
		if(pos == rhs.pos){
			return true;
		}
		return false;
	}
    
};

#endif