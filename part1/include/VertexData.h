#ifndef VERTEX_DATA
#define VERTEX_DATA

#include "glm/glm.hpp"

struct VertexData{
	float x,y,z;
	float s,t;
	VertexData(float _x, float _y, float _z, float _s, float _t): x(_x),y(_y),z(_z),s(_s),t(_t) { }
	
	// Tests if two VertexData are equal
	bool operator== (const VertexData &rhs){
		if( (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (s == rhs.s) && (t == rhs.t) ){
			return true;
		}
		return false;
	}
};

#endif