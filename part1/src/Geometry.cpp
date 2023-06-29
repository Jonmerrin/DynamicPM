#include "Geometry.hpp"
#include <assert.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"

// Constructor
Geometry::Geometry(){

}

void Geometry::_deleteVertices(Vertex* v) {
	std::cout << " delete v" << std::endl;
	if(v == nullptr) return;
	if(v->vsplit != nullptr) {
		_deleteVertices(v->vsplit->v);
		_deleteVertices(v->vsplit->v2);
		delete(v->vsplit->e);
		delete(v->vsplit->consumedLeftEdge);
		delete(v->vsplit->consumedRightEdge);
		delete(v->vsplit);
	}
	_deleteVertices(v->next);
	delete v;
}

void Geometry::_deleteEdges(Edge* e) {
	std::cout << " delete e" << std::endl;
	if(e == nullptr){
		return;
	}
	_deleteEdges(e->next);
	delete e;
}

void Geometry::_deleteFaces(Face* f) {
	std::cout << " delete f" << std::endl;
	if(f == nullptr) {
		return;
	}
	_deleteFaces(f->next);
	delete f;
}

// Destructor
Geometry::~Geometry(){
	_deleteVertices(vertices_start);
	_deleteEdges(edges_start);
	_deleteFaces(faces_start);
}

// Sets the tolerance for this model
void Geometry::SetTolerance(float newT) {
    tolerance = newT;
}

// Get the width of the model (display purposes only)
float Geometry::GetWidth() {
	return width;
}

// Allows for adding one index at a time manually if 
// you know which vertices are needed to make a triangle.
void Geometry::AddIndex(unsigned int i){
    // Simple bounds check to make sure a valid index is added.
    // if(i >= 0 && i <= m_vertexPositions.size()/3){
    //     m_indices.push_back(i);
    // }else{
    //     std::cout << "(Geometry.cpp) ERROR, invalid index\n";
    // }
}

// Retrieves a pointer to our data.
float* Geometry::GetBufferDataPtr(){
	return m_bufferData.data();
}

// Retrieves the size of our data 
unsigned int Geometry::GetBufferDataSize(){
	return m_bufferData.size();
}

// Retrieves the number of bytes of our data
unsigned int Geometry::GetBufferSizeInBytes(){
	return m_bufferData.size()*sizeof(float);
}

// Create all data
// The idea here is that we are pshing all of our data from
// each individual vertex into a single vector.
// This makes it relatively easy to then fill in a buffer
// with the corresponding vertices
void Geometry::Gen(){
	// assert((m_vertexPositions.size()/3) == (m_textureCoords.size()/2));
	// std::cout << "Entering Gen" << std::endl;
	float leftmostX;
	float rightmostX;
	// int coordsPos =0;
	Vertex* v = vertices_start;
	int counter = 0;
	while(v != nullptr){
		//Set the index of this vertex
		v->ind = counter;
		// vertices
		m_bufferData.push_back(v->pos.x);
		m_bufferData.push_back(v->pos.y);
		m_bufferData.push_back(v->pos.z);
		if (v->pos.x < leftmostX) {
			leftmostX = v->pos.x;
		}
		if (v->pos.x > rightmostX) {
			rightmostX = v->pos.x;
		}
		// m_normals
		// m_bufferData.push_back(m_normals[i*3+0]);
		// m_bufferData.push_back(m_normals[i*3+1]);
		// m_bufferData.push_back(m_normals[i*3+2]);
    	// texture information
		m_bufferData.push_back(v->tex.s); 
		m_bufferData.push_back(v->tex.t); 
		v = v->next;
		counter++;
		// ++coordsPos; // Note separate counter for coords Pos.
					 // Because we only have two dimensions and want
					 // to make sure the corresponde to proper three
					 // dimensional vertex attributes.
		// tangents
		// m_bufferData.push_back(m_tangents[i*3+0]);
		// m_bufferData.push_back(m_tangents[i*3+1]);
		// m_bufferData.push_back(m_tangents[i*3+2]);
		// // bi-tangents
		// m_bufferData.push_back(m_biTangents[i*3+0]);
		// m_bufferData.push_back(m_biTangents[i*3+1]);
		// m_bufferData.push_back(m_biTangents[i*3+2]);
	}
	
	// get the width of the model (for display purposes only)
	width = rightmostX - leftmostX;
	
}


// Adds a vertex and associated texture coordinate.
// Will also add a and a normal
void Geometry::AddVertex(float x, float y, float z, float s, float t){
	// std::cout<< "Entering AddVertex" << std::endl;
	Vertex* v = new Vertex(glm::vec3(x,y,z), glm::vec2(s,t));
	InsertNodeToList(v);
	m_vertices.push_back(v);
	// printLinkedList(vertices_start);
	CycleCheck();
	// std::cout << m_vertices.size() << " total verts: [";
	// for (int i = m_vertices.size()-1; i >= 0 ; --i)
	// {
	// 	std::cout<< m_vertices[i] << ", ";
	// }
	// std::cout << "]" << std::endl;
	
	// std::cout<< "Exiting AddVertex" << std::endl;
}

// The big trick here, is that when we make a triangle
// We also need to update our m_normals, tangents, and bi-tangents.
void Geometry::MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2){
	// std::cout << "Entering MakeTriangle()" << std::endl;
	m_indices.push_back(vert0);	
	m_indices.push_back(vert1);	
	m_indices.push_back(vert2);	
	

	// Look up the actual vertex positions
	glm::vec3 pos0(m_vertices[vert0]->pos);
	glm::vec3 pos1(m_vertices[vert1]->pos);
	glm::vec3 pos2(m_vertices[vert2]->pos);

	// Look up the texture coordinates
	glm::vec2 tex0(m_vertices[vert0]->tex); 
	glm::vec2 tex1(m_vertices[vert1]->tex); 
	glm::vec2 tex2(m_vertices[vert2]->tex); 

	// Now create an edge
	// With two edges
	// This section is inspired by: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	glm::vec3 edge0 = pos1 - pos0;
	glm::vec3 edge1 = pos2 - pos0;
	// Question to ask yourself is what is going on here?
    // The difference of y's and x's? Hmm.
	glm::vec2 deltaUV0 = tex1-tex0;
	glm::vec2 deltaUV1 = tex2-tex0;

	float f = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

	// glm::vec3 tangent;
	// glm::vec3 bitangent;

	// tangent.x = f * (deltaUV1.y * edge0.x - deltaUV0.y* edge1.x);
	// tangent.y = f * (deltaUV1.y * edge0.y - deltaUV0.y* edge1.y);
	// tangent.z = f * (deltaUV1.y * edge0.z - deltaUV0.y* edge1.z);
	// tangent = glm::normalize(tangent);

	// bitangent.x = f * (-deltaUV1.x * edge0.x + deltaUV0.x* edge1.x);
	// bitangent.y = f * (-deltaUV1.x * edge0.y + deltaUV0.x* edge1.y);
	// bitangent.z = f * (-deltaUV1.x * edge0.z + deltaUV0.x* edge1.z);
	// bitangent = glm::normalize(bitangent);
	
	// // Compute a normal
	// // For now we sort of 'cheat' since this is a quad the 'z' axis points straight out
    // glm::vec3 normal1{m_normals[vert0*3+0] ,m_normals[vert0*3+1], m_normals[vert0*3+2]};
    // glm::vec3 normal2{m_normals[vert1*3+0] ,m_normals[vert1*3+1], m_normals[vert1*3+2]};
    // glm::vec3 normal3{m_normals[vert2*3+0] ,m_normals[vert2*3+1], m_normals[vert2*3+2]};


	// m_normals[vert0*3+0] = 0.0f;	m_normals[vert0*3+1] = 0.0f;	m_normals[vert0*3+2] = 1.0f;	
	// m_normals[vert1*3+0] = 0.0f;	m_normals[vert1*3+1] = 0.0f;	m_normals[vert1*3+2] = 1.0f;	
	// m_normals[vert2*3+0] = 0.0f;	m_normals[vert2*3+1] = 0.0f;	m_normals[vert2*3+2] = 1.0f;	
		
	// // Compute a tangent
	// m_tangents[vert0*3+0] = tangent.x; m_tangents[vert0*3+1] = tangent.y; m_tangents[vert0*3+2] = tangent.z;	
	// m_tangents[vert1*3+0] = tangent.x; m_tangents[vert1*3+1] = tangent.y; m_tangents[vert1*3+2] = tangent.z;	
	// m_tangents[vert2*3+0] = tangent.x; m_tangents[vert2*3+1] = tangent.y; m_tangents[vert2*3+2] = tangent.z;	

	// // Compute a bi-tangent
	// m_biTangents[vert0*3+0] = bitangent.x; m_biTangents[vert0*3+1] = bitangent.y; m_biTangents[vert0*3+2] = bitangent.z;	
	// m_biTangents[vert1*3+0] = bitangent.x; m_biTangents[vert1*3+1] = bitangent.y; m_biTangents[vert1*3+2] = bitangent.z;	
	// m_biTangents[vert2*3+0] = bitangent.x; m_biTangents[vert2*3+1] = bitangent.y; m_biTangents[vert2*3+2] = bitangent.z;	

	// Calculate the normal vector to the triangle	
	glm::vec3 planeNormal = glm::normalize(glm::vec3(   edge0.y * edge1.z - edge1.y * edge0.z,
														edge0.z * edge1.x - edge1.z * edge0.x,
														edge0.x * edge1.y - edge1.x * edge0.y));
	float a = planeNormal.x;
	float b = planeNormal.y;
	float c = planeNormal.z;
	float d = -1*(a * m_vertices[vert0]->pos.x 
				+ b * m_vertices[vert0]->pos.y
				+ c * m_vertices[vert0]->pos.z);
	// Create the fundamental error quadric for this triangle
	glm::mat4 k_pp( glm::vec4(a*a, a*b, a*c, a*d),
					glm::vec4(a*b, b*b, b*c, b*d),
					glm::vec4(a*c, b*c, c*c, c*d),
					glm::vec4(a*d, b*d, c*d, d*d));
	// Add the fundamental error quadric to each of the triangle's vertices' Q matrix
	m_vertices[vert0]->Q += k_pp;
	m_vertices[vert1]->Q += k_pp;
	m_vertices[vert2]->Q += k_pp;
	Edge* e1 = AddEdge(m_vertices[vert0], m_vertices[vert1]);
	Edge* e2 = AddEdge(m_vertices[vert1], m_vertices[vert2]);
	Edge* e3 = AddEdge(m_vertices[vert2], m_vertices[vert0]);
	Face * face = new Face(m_vertices[vert0], m_vertices[vert1], m_vertices[vert2], e1,e2,e3);
	InsertNodeToList(face);
	e1->AddFace(face);
	e2->AddFace(face);
	e3->AddFace(face);
	// Assert that our Q matrices give the expected answer of 0 when multiplied by their square vertices (within an epsilon)
	float almostEqualsEpsilon = 0.00001f;
	// assert(fabs(glm::dot(glm::vec4(pos0, 1.0) * m_vertices[vert0]->Q, glm::vec4(pos0, 1.0))) <= almostEqualsEpsilon);
	// assert(fabs(glm::dot(glm::vec4(pos1, 1.0) * m_vertices[vert1]->Q, glm::vec4(pos1, 1.0))) <= almostEqualsEpsilon);
	// assert(fabs(glm::dot(glm::vec4(pos2, 1.0) * m_vertices[vert2]->Q, glm::vec4(pos2, 1.0))) <= almostEqualsEpsilon);
	// std::cout << "Exiting MakeTriangle" << std::endl;
}


// Adds an edge to the edge queue
Edge* Geometry::AddEdge(Vertex* v1, Vertex* v2) {

	Edge* search = edges_start;

	while (search != nullptr)
	{
		if((search->v1 == v1 && search->v2 == v2)
		|| (search->v2 == v1 && search->v1 == v2)) {
			return search;
		}
		search = search->next;
	}
	
	Edge* e = new Edge(v1, v2);

	InsertNodeToList(e);

	return e;
}

// Removes an edge
void Geometry::RemoveEdge(Edge* e) {
	// std::cout << "REMOVING EDGE " << e << std::endl;
	if(isBoundaryEdge(e)) {
		return;
	}
	_removeEdge(e);
}

bool Geometry::isBoundaryEdge(Edge* e) {
	return (e->f1 == nullptr || e->f2 == nullptr);
}

// Helper function for removing an edge
void Geometry::_removeEdge(Edge* e) {
 	Vertex* v1 = getActiveVertex(e->v1);
	Vertex* v2 = getActiveVertex(e->v2);
	glm::vec3 pos1 = glm::vec3(v1->pos);
	glm::vec3 pos2 = glm::vec3(v2->pos);
	glm::mat4 newQ = v1->Q + v2->Q;

	

	glm::mat4 inverseQ = glm::inverse(glm::mat4(newQ[0], newQ[1], newQ[2], glm::vec4(0,0,0,1)));

	glm::vec3 contractedPos = glm::vec3(inverseQ[0][3], inverseQ[1][3], inverseQ[2][3]);

	float det = glm::determinant(glm::mat4(newQ[0], newQ[1], newQ[2], glm::vec4(0,0,0,1)));
	if(det < 1 || glm::isinf(det) || isinf(det) || isnan(det)){// < 1 || glm::isinf(glm::determinant(glm::mat4(newQ[0], newQ[1], newQ[2], glm::vec4(0,0,0,1))))) {
		// std::cout << "\tFalling back on position" << std::endl;
		contractedPos = (pos1 + pos2) * 0.5f;
	}

	//update edges
	Face* fn0 = nullptr;
	Face* fn1 = nullptr;
	Face* fn2 = nullptr;
	Face* fn3 = nullptr;
	Edge* vlvt = nullptr;
	Edge* vlvu = nullptr;
	Edge* vrvt = nullptr;
	Edge* vrvu = nullptr;

	// Find the four edges adjacent to e. Call f1 = left and f2 = right WLOG.
	for (size_t i = 0; i < 3; i++)
	{
		// Edges next to fl
		if(e->f1->edges[i] != e) {
			if(vlvu == nullptr) {
				vlvu = e->f1->edges[i];
			} else {
				vlvt = e->f1->edges[i];
			}
		}

		// Edges next to fr
		if(e->f2->edges[i] != e) {
			if(vrvu == nullptr) {
				vrvu = e->f2->edges[i];
			} else {
				vrvt = e->f2->edges[i];
			}
		}
	}
	
	//TODO: Remove this when there is a better boundary check
	if(vlvu == nullptr || vlvt == nullptr || vrvu == nullptr || vrvt == nullptr) {
		return;
	}

	// std::cout << "\tCollapsing Edge " << e << ", f1 " << e->f1 << ", f2 " << e->f2 << ", Active v1 " << getActiveVertex(e->v1) << ", Active v2 " << getActiveVertex(e->v2) << ", v1 " << (e->v1) << ", v2 " << (e->v2) << std::endl;


	// std::cout << "\tfl edges: ";
	// for (size_t i = 0; i < 3; i++)
	// {
	// 	std::cout << e->f1->edges[i] << ", ";
	// }
	// std::cout << std::endl;
	// std::cout << "\tfr edges: ";

	// for (size_t i = 0; i < 3; i++)
	// {
	// 	std::cout << e->f2->edges[i] << ", ";
	// }
	// std::cout << std::endl;

	//GOOD
	// std::cout << "\tVLVT: Edge " << vlvt << " f1: " << vlvt->f1 << " f2: " << vlvt->f2 << std::endl;
	// std::cout << "\tVLVU: Edge " << vlvu << " f1: " << vlvu->f1 << " f2: " << vlvu->f2 << std::endl;
	// std::cout << "\tVRVT: Edge " << vrvt << " f1: " << vrvt->f1 << " f2: " << vrvt->f2 << std::endl;
	// std::cout << "\tVRVU: Edge " << vrvu << " f1: " << vrvu->f1 << " f2: " << vrvu->f2 << std::endl;

	// Get the 4 faces adjacent to fl and fr. Find them by taking the face next to the four adjacent edges that is not
	// fl or fr.
	if(vlvt->f1 != e->f1) {
		fn0 = vlvt->f1;
	} else {
		fn0 = vlvt->f2;
	}
	
	if(vlvu->f1 != e->f1) {
		fn1 = vlvu->f1;
	} else {
		fn1 = vlvu->f2;
	}
	
	if(vrvt->f1 != e->f2) {
		fn2 = vrvt->f1;
	} else {
		fn2 = vrvt->f2;
	}
	
	if(vrvu->f1 != e->f2) {
		fn3 = vrvu->f1;
	} else {
		fn3 = vrvu->f2;
	}

	//TODO: Remove this when there is a better boundary check
	if(fn0 == nullptr || fn1 == nullptr || fn2 == nullptr || fn3 == nullptr) {
		return;
	}

	if(vrvt == vrvu || vrvt == vlvu || vrvt == vlvt 
	|| vrvu == vlvu || vrvu == vlvt
	|| vlvu == vlvt) {
		return;
	}

	// Safeguards are done, we can now start editing data.

	RemoveNodeFromList(e);
	RemoveNodeFromList(e->f1);
	RemoveNodeFromList(e->f2);

	Edge* activeEdgeLeft;
	Edge* activeEdgeRight;
	Edge* consumedEdgeLeft;
	Edge* consumedEdgeRight;

	if(vlvu->counter > vlvt->counter) {
		// std::cout << "ACTIVE EDGE RIGHT = VLVU " << vlvu << " F1: " << vlvu->f1 << " F2: " << vlvu->f2 << std::endl;
		// std::cout << "CONSUMED EDGE RIGHT = VLVT " << vlvt << " F1: " << vlvt->f1 << " F2: " << vlvt->f2 << std::endl;
		activeEdgeLeft = vlvu;
		RemoveNodeFromList(vlvt);
		consumedEdgeLeft = vlvt;
	} else {
		// std::cout << "ACTIVE EDGE RIGHT = VLVT " << vlvt << " F1: " << vlvt->f1 << " F2: " << vlvt->f2 << std::endl;
		// std::cout << "CONSUMED EDGE RIGHT = VLVU " << vlvu << " F1: " << vlvu->f1 << " F2: " << vlvu->f2 << std::endl;
		activeEdgeLeft = vlvt;
		RemoveNodeFromList(vlvu);
		consumedEdgeLeft = vlvu;
	}

	if(vrvu->counter > vrvt->counter) {
		// std::cout << "ACTIVE EDGE RIGHT = VRVU " << vrvu << " F1: " << vrvu->f1 << " F2: " << vrvu->f2 << std::endl;
		// std::cout << "CONSUMED EDGE RIGHT = VRVT " << vrvt << " F1: " << vrvt->f1 << " F2: " << vrvt->f2 << std::endl;
		activeEdgeRight = vrvu;
		RemoveNodeFromList(vrvt);
		consumedEdgeRight = vrvt;
	} else {
		// std::cout << "ACTIVE EDGE RIGHT = VRVT " << vrvt << " F1: " << vrvt->f1 << " F2: " << vrvt->f2 << std::endl;
		// std::cout << "CONSUMED EDGE RIGHT = VRVU " << vrvu << " F1: " << vrvu->f1 << " F2: " << vrvu->f2 << std::endl;
		activeEdgeRight = vrvt;
		RemoveNodeFromList(vrvu);
		consumedEdgeRight = vrvu;
	}

	// std::cout << "LINKING UP EDGES TO FACES" << std::endl;

	if(activeEdgeLeft->f1 == e->f1) {
		activeEdgeLeft->f1 = activeEdgeLeft->f2 == fn0 ? fn1 : fn0;
	} else {
		activeEdgeLeft->f2 = activeEdgeLeft->f1 == fn0 ? fn1 : fn0;
	}
	
	if(activeEdgeRight->f1 == e->f2) {
		activeEdgeRight->f1 = activeEdgeRight->f2 == fn2 ? fn3 : fn2;
	} else {
		activeEdgeRight->f2 = activeEdgeRight->f1 == fn2 ? fn3 : fn2;
	}
	
	vlvu->counter++;
	vlvt->counter++;
	vrvu->counter++;
	vrvt->counter++;
	e->counter++;
	
	//update faces
	//GOOD
	// std::cout << "\tUPDATING FACES: fn0: " << fn0 << " fn1: " << fn1 << " fn2: " << fn2 << " fn3: " << fn3 << std::endl;

	// std::cout << "\t\tOriginal fn0 edges: " << fn0->edges[0] << " " << fn0->edges[1] << " " << fn0->edges[2] << std::endl;
	// std::cout << "\t\tOriginal fn1 edges: " << fn1->edges[0] << " " << fn1->edges[1] << " " << fn1->edges[2] << std::endl;
	// std::cout << "\t\tOriginal fn2 edges: " << fn2->edges[0] << " " << fn2->edges[1] << " " << fn2->edges[2] << std::endl;
	// std::cout << "\t\tOriginal fn3 edges: " << fn3->edges[0] << " " << fn3->edges[1] << " " << fn3->edges[2] << std::endl;
	// std::cout << "\t\t---------------------------------------" << std::endl;
	for (size_t i = 0; i < 3; i++)
	{
		if(fn0->edges[i] == vlvt) {
			fn0->edges[i] = activeEdgeLeft;
		}
		
		if(fn1->edges[i] == vlvu) {
			fn1->edges[i] = activeEdgeLeft;
		}
		
		if(fn2->edges[i] == vrvt) {
			fn2->edges[i] = activeEdgeRight;
		}
		
		if(fn3->edges[i] == vrvu) {
			fn3->edges[i] = activeEdgeRight;
		}
	}
	//GOOD
	// std::cout << "\t\tUpdated fn0 edges: " << fn0->edges[0] << " " << fn0->edges[1] << " " << fn0->edges[2] << std::endl;
	// std::cout << "\t\tUpdated fn1 edges: " << fn1->edges[0] << " " << fn1->edges[1] << " " << fn1->edges[2] << std::endl;
	// std::cout << "\t\tUpdated fn2 edges: " << fn2->edges[0] << " " << fn2->edges[1] << " " << fn2->edges[2] << std::endl;
	// std::cout << "\t\tUpdated fn3 edges: " << fn3->edges[0] << " " << fn3->edges[1] << " " << fn3->edges[2] << std::endl;
	
	Vertex* newV = new Vertex(contractedPos, v1->tex);

	v1->ActiveVertex = newV;
	RemoveNodeFromList(v1);
	v2->ActiveVertex = newV;
	RemoveNodeFromList(v2);
	InsertNodeToList(newV);
	// v2->ActiveVertex = v1;
	// std::cout << "\tACTIVE EDGE COUNTERS: " << activeEdgeLeft->counter << " " << activeEdgeRight->counter << std::endl;
	VertexSplit * vsplit = new VertexSplit(v1, v2, glm::vec3(pos1), e, activeEdgeLeft, activeEdgeRight, consumedEdgeLeft, consumedEdgeRight, activeEdgeLeft->counter, activeEdgeRight->counter);
	newV->vsplit = vsplit;
	// std::cout << "\tOLD POSITION FOR NEW VSPLIT: " << "[" << pos1.x << ", " << pos1.y << ", " << pos1.z << "], " << std::endl;
	// std::cout << "\tNEW VSPLIT FOR V" << v2 << ": " << newV->vsplit->e << ", " 
	// 											  << newV->vsplit->activeLeftEdge << ", " 
	// 											  << newV->vsplit->activeRightEdge << ", " 
	// 											  << "[" << newV->vsplit->v1Pos.x << ", " << newV->vsplit->v1Pos.y << ", " <<newV->vsplit->v1Pos.z << "], " 
	// 											  << newV->vsplit->v2 << ", " 
	// 											  << newV->vsplit->v << ", "
	// 											  << newV->vsplit->leftCounter << ", "
	// 											  << newV->vsplit->rightCounter << std::endl;
	
	// std::cout << "New head of list: " << newV << " newV->next: " << newV->next << std::endl;
	// printLinkedList(vertices_start);
	// assert(!CycleCheck());
	// std::cout << "Is there a cycle? " << (CycleCheck() ? "Yes!" : "No!") << std::endl;
	newV->Q = v1->Q + v2->Q;
	//regen();
	// std::cout << "EXITING REMOVE EDGE" << std::endl;
}

void Geometry::printLinkedList(Vertex* vert) {
	std::cout << "vertex LinkedList: ";
	// std::cout << "Entering print linked list" << std::endl;
	if(vert == nullptr) {
		std::cout << "end." << std::endl;
		return;
	}
	bool cycle = false;
	Vertex* v = vert;
	Vertex* hare = vert->next;

	// std::cout << "\t2nd checkpoint" << std::endl;
	while (v != nullptr && v!=hare)
	{
		std::cout << v << "->";
		// std::cout << "\t\tprinting " << v << std::endl;
		// std::cout << "\t\t\twith next:" << v->next << " and prev: "<<v->prev << std::endl;
		if(hare == v) {
			// std::cout << "\t\t\t\tcycle found" << std::endl;
			cycle = true;
			hare = vertices_start;
		}
		// std::cout << "\t\t\t\tstpd " << hare << std::endl;

		if(cycle) {}
		else if(hare != nullptr && hare->next != nullptr) {
			// std::cout << "\t\tthis is not it" << std::endl;
			hare = hare->next->next;
		} else {
			// std::cout << "\t\tthis is not it either" << std::endl;
			hare = nullptr;
		}
		// std::cout << "\tsnoop" << std::endl;
		v = v->next;
		// std::cout << "\tboop" << std::endl;
	}
	
	// std::cout << "\t3rd checkpoint" << std::endl;
	if(v != nullptr) {
		std::cout << v << "->" << v->next << "  CYCLE!!!" << std::endl;
		return;
	}
	std::cout << "end" << std::endl;
	return;

}

void Geometry::RemoveNodeFromList(Vertex* v) {
	// std::cout << "Removing vertex " << v << std::endl;
	if(v == nullptr) {
		return;
	}
	// Removing the head of the list
	if(v == vertices_start) {
		// std::cout << "\t0" << std::endl;
		vertices_start = v->next;
		if(vertices_start != nullptr){
			vertices_start->prev = nullptr;
		}
	} else if(v->next == nullptr){ // Removing the tail
		// std::cout << "\t1" << std::endl;
		v->prev->next = nullptr;
	} else {
		// std::cout << "\t2" << std::endl;
		// std::cout << "\tv->next: " << v->next << "\tv->prev: " << v->prev << std::endl;
		v->prev->next = v->next;
		v->next->prev = v->prev;
	}
	// std::cout << "\t3" << std::endl;
	v->next = nullptr;
	v->prev = nullptr;
	// std::cout << "\tvertex " << v << " removed" << std::endl;
}


void Geometry::RemoveNodeFromList(Edge* e) {
	// std::cout << "Removing edge " << e << " from list" << std::endl;
	if(e == nullptr) {
		return;
	}
	// Removing the head of the list
	if(e == edges_start) {
		// std::cout << "\t0" << std::endl;
		edges_start = e->next;
		if(edges_start != nullptr){
			edges_start->prev = nullptr;
		}
	} else if(e->next == nullptr){ // Removing the tail
		// std::cout << "\t1" << std::endl;
		// std::cout << "edges_start: " << edges_start << " e->prev: " << e->prev << std::endl;
		e->prev->next = nullptr;
	} else {
		// std::cout << "\t2" << std::endl;
		e->prev->next = e->next;
		e->next->prev = e->prev;
	}
	// std::cout << "\t3" << std::endl;
	e->next = nullptr;
	e->prev = nullptr;
	// std::cout << "\tedge " << e << " removed" << std::endl;
}

void Geometry::RemoveNodeFromList(Face* f) {
	// std::cout << "Removing face " << f << std::endl;
	if(f == nullptr) {
		return;
	}
	// Removing the head of the list
	if(f == faces_start) {
		// std::cout << "\t0" << std::endl;
		faces_start = f->next;
		if(faces_start != nullptr){
			faces_start->prev = nullptr;
		}
	} else if(f->next == nullptr){ // Removing the tail
		// std::cout << "\t1" << std::endl;
		f->prev->next = nullptr;
	} else {
		// std::cout << "\t2" << std::endl;
		f->prev->next = f->next;
		f->next->prev = f->prev;
	}
	// std::cout << "\t3" << std::endl;
	f->next = nullptr;
	f->prev = nullptr;
	// std::cout << "\tface " << f << " removed" << std::endl;
}

void Geometry::InsertNodeToList(Vertex* v) {
	if(v == nullptr) return;
	if(vertices_start == nullptr) {
		vertices_start = v;
		return;
	}
	vertices_start->prev = v;
	v->next = vertices_start;
	vertices_start = v;
}


void Geometry::InsertNodeToList(Edge* e) {
	if(e == nullptr) return;
	if(edges_start == nullptr) {
		edges_start = e;
		return;
	}
	edges_start->prev = e;
	e->next = edges_start;
	edges_start = e;
}


void Geometry::InsertNodeToList(Face* f) {
	// std::cout << "Face info for (" << f << "): f->edges[0]=" <<f->edges[0] << "f->edges[1]=" << f->edges[1] << "f->edges[2]=" << f->edges[2] << std::endl;
	if(f == nullptr) return;
	if(faces_start == nullptr) {
		faces_start = f;
		f->next = nullptr;
		f->prev = nullptr;
		return;
	}
	faces_start->prev = f;
	f->next = faces_start;
	faces_start = f;
}

void Geometry::printLinkedList(Edge* edge) {
	if(edge == nullptr) {
		// std::cout << "end." << std::endl;
		return;
	}
	std::cout << "edge LinkedList: ";
	bool cycle = false;
	Edge* e = edge;
	Edge* hare = edge->next;

	while (e != nullptr && e!=hare)
	{
		std::cout << e << "->";
		if(hare == e) {
			cycle = true;
			hare = edges_start;
		}
		if(cycle) {}
		else if(hare != nullptr && hare->next != nullptr) {
			hare = hare->next->next;
		} else {
			hare = nullptr;
		}
		e = e->next;
	}
	if(e != nullptr) {
		std::cout << e << "->" << e->next << "->" << e->next << "->" << e->next << "  CYCLE!!!" << std::endl;
		return;
	}
	std::cout << "end" << std::endl;
	return;
}


void Geometry::printLinkedList(Face* face) {
	if(face == nullptr) {
		// std::cout << "end." << std::endl;
		return;
	}
	std::cout << "face LinkedList: ";
	bool cycle = false;
	Face* f = face;
	Face* hare = face->next;

	while (f != nullptr && f!=hare)
	{
		std::cout << f << "->";
		if(hare == f) {
			cycle = true;
			hare = faces_start;
		}
		if(cycle) {}
		else if(hare != nullptr && hare->next != nullptr) {
			hare = hare->next->next;
		} else {
			hare = nullptr;
		}
		f = f->next;
	}
	if(f != nullptr) {
		std::cout << f << "->" << f->next << "  CYCLE!!!" << std::endl;
		return;
	}
	std::cout << "end" << std::endl;
	return;
}

bool Geometry::CycleCheck() {
	if(vertices_start==nullptr){
		return false;
	}
	Vertex* tortoise = vertices_start;
	Vertex* hare = vertices_start->next;
	// std::cout << "vertex LinkedList: ";
	while (tortoise != hare && tortoise != nullptr)
	{
		// std::cout << tortoise << "->";
		if(hare != nullptr && hare->next != nullptr) {
			hare = hare->next->next;
		} else {
			hare = nullptr;
		}
		tortoise = tortoise->next;
	}
	if(tortoise != nullptr) {
		// std::cout << tortoise << "  CYCLE!!!" << std::endl;
		return true;
	}
	// std::cout << "end" << std::endl;
	return false;
}

Vertex* Geometry::getActiveVertex(Vertex* vert) {
	// std::cout << "GETTING ACTIVE VERTEX FOR " << vert << std::endl;
	Vertex* v = vert;
	while(v->ActiveVertex != nullptr) {v = v->ActiveVertex;}
	// std::cout << "GOTTEN ACTIVE VERTEX" << std::endl;
	return v;
}

void Geometry::SplitVertex(Vertex* v){
	
	// std::cout << "Splitting vertex " << v << std::endl;
	// Find vertices labeled for split
	// 


	// Data structure brainstorm: if you keep track of the edge list per vertex, you can keep track of which
	// edges belonged to which vertex by having a "start" and "end" index into the list, then when collapsing
	// just concatenate the list.
	// This would take 2E to remember each of the edges (since each edge is represented exactly twice) + 2*8*n
	// to keep track of the indices.

	// Whereas keeping a list of which edges belonged to the previous vertex could count the same edge more than 3 times
	// because it gets added to a list every time there's a collapse. But it does make it dynamic rather than static.

	// The goal is not to have a discrete VSplit data type.

	// Let v be the vertex being split

	VertexSplit* vsplit = v->vsplit;
	RemoveNodeFromList(v);
	InsertNodeToList(vsplit->v);
	InsertNodeToList(vsplit->v2);
	InsertNodeToList(vsplit->e->f1);
	InsertNodeToList(vsplit->e->f2);
	InsertNodeToList(vsplit->e);
	InsertNodeToList(vsplit->consumedLeftEdge);
	InsertNodeToList(vsplit->consumedRightEdge);
	vsplit->activeLeftEdge->counter--;
	vsplit->activeRightEdge->counter--;
	vsplit->consumedLeftEdge->counter--;
	vsplit->consumedRightEdge->counter--;
	vsplit->e->counter--;
	// vsplit->e->v1->shouldCollapse = false;
	// vsplit->e->v2->shouldCollapse = false;

	vsplit->v->ActiveVertex = nullptr;
	vsplit->v2->ActiveVertex = nullptr;
	// if(vsplit->consumedLeftEdge->f1 != vsplit->e->f1 && vsplit->consumedLeftEdge->f2 != vsplit->e->f1) {
	// 	std::cout << "I think my brain done got broked, Clem \tfl:" << vsplit->e->f1 << " eLeft.f1" << vsplit->consumedLeftEdge->f1 << " eLeft.f2" << vsplit->consumedLeftEdge->f2 << std::endl;
	// }
	// if(vsplit->consumedRightEdge->f1 != vsplit->e->f2 && vsplit->consumedRightEdge->f2 != vsplit->e->f2) {
	// 	std::cout << "I think my brian dujn got borked, Clem \tfl:" << vsplit->e->f1 << " eLeft.f1" << vsplit->consumedLeftEdge->f1 << " eLeft.f2" << vsplit->consumedLeftEdge->f2 << std::endl;
	// }
	Face* fnLeft = vsplit->consumedLeftEdge->f1 == vsplit->e->f1 ? vsplit->consumedLeftEdge->f2 : vsplit->consumedLeftEdge->f1;
	Face* fnRight = vsplit->consumedRightEdge->f1 == vsplit->e->f2 ? vsplit->consumedRightEdge->f2 : vsplit->consumedRightEdge->f1;
	
	for (size_t i = 0; i < 3; i++)
	{
		if(fnLeft->edges[i] == vsplit->activeLeftEdge) {
			fnLeft->edges[i] = vsplit->consumedLeftEdge;
			// std::cout << "\tLeft SHOULD ONLY HAPPEN ONCE, DEAR GOD, PLEASE" << std::endl;
		}
		
		if(fnRight->edges[i] == vsplit->activeRightEdge) {
			fnRight->edges[i] = vsplit->consumedRightEdge;
			// std::cout << "\tRIGHT SHOULD ONLY HAPPEN ONCE, DEAR GOD, PLEASE" << std::endl;
		}
	}
	

	// Reset where the edges are pointing
	if(vsplit->activeLeftEdge->f1 == fnLeft) { vsplit->activeLeftEdge->f1 = vsplit->e->f1; }
	else { vsplit->activeLeftEdge->f2 = vsplit->e->f1; }
	
	if(vsplit->activeRightEdge->f1 == fnRight) { vsplit->activeRightEdge->f1 = vsplit->e->f2; }
	else { vsplit->activeRightEdge->f2 = vsplit->e->f2; }
	
	
	// v->active = false;
	// AddEdge(vsplit.v, vsplit.v2, vsplit.hasEdge);

	//regen();
	delete vsplit;
	delete v;
	// printLinkedList(vertices_start);
	CycleCheck();
	// std::cout << "EXITING VSPLIT" << std::endl;
}

void Geometry::Update(glm::mat4 proj_mat, Camera* camera, Camera* unhookedCamera, Transform worldTransform) {
	std::cout << "ENTERING GEOMETRY::UPDATE" << std::endl;
	Vertex* v = vertices_start;
	
	// std::cout << "V is " << v << std::endl;
	while(v != nullptr)
	{
		// assert(!CycleCheck());
		// if(v->active) {
		// 	std::cout << "vsplit for " << v << " is active ";
		// 	if(isInViewFrustum(v, proj_mat, unhookedCamera, worldTransform) && v->vsplit != nullptr) {
		// 		std::cout << "and is in view with counters L: " << v->vsplit->leftCounter << " R: " << v->vsplit->rightCounter << std::endl;
		// 	} else {
		// 		std::cout << "but is not in view." << std::endl;
		// 	}
		// } 
		
		// std::cout << "Random sanity check #1" << std::endl;
		bool willSplit = false;
		// std::cout << "\tIs " << v << " in the view frustum?" << std::endl;
		if(isInViewFrustum(v, proj_mat, unhookedCamera, worldTransform)) {
			// std::cout << "\tIt is! and its vsplit is " << v->vsplit <<std::endl; 
			// std::cout << "Fails in if" << std::endl;
			v->shouldCollapse = false;
			// std::cout << "In the view frustum" << std::endl;
			// std::cout << "\tshould split: " << shouldSplit(v, proj_mat, camera, worldTransform) << std::endl;
			// std::cout << "\tcan split: " << canSplit(v) << std::endl;
			// std::cout << "Is the vsplit for " << v << "active? " << (m_vsplits[v].active ? "yes!" : "no...") << std::endl;
			if(v->vsplit != nullptr && canSplit(v) && shouldSplit(v, proj_mat, unhookedCamera, worldTransform)) {
				// willSplit = true;
				// std::cout << "\tCondition met for v " << v << std::endl;
				Vertex* next = v->next;
				SplitVertex(v);
				v = next;
				continue;
			}
			// std::cout << "Random sanity check #2a" << std::endl;
		} else{
			// if(m_wasConsumed[v])
			// std::cout << "No it is not." << std::endl;
			v->shouldCollapse = true;
			// std::cout << "Random sanity check #2b" << std::endl;
		}
		v = v->next;
		// std::cout << "Loop end" << std::endl;
	}

	// printLinkedList(vertices_start);
	// printLinkedList(edges_start);
	// printLinkedList(faces_start);

	std::cout << "Random sanity check #3" << std::endl;
	// std::cout << "Done with verts, onto edges!" << std::endl;
	Edge* e = edges_start;
	
	// while(e != nullptr && !e->active) {
	// 	edges_start = e->next;
	// 	e = edges_start;
	// }
	while(e!=nullptr)
	{		
		if(getActiveVertex(e->v1)->shouldCollapse && getActiveVertex(e->v2)->shouldCollapse) {
			getActiveVertex(e->v1)->shouldCollapse = false;
			getActiveVertex(e->v2)->shouldCollapse = false;
			RemoveEdge(e);
		}
		e = e->next;
	}
	
	// std::cout << "Random sanity check #4" << std::endl;
	regen();
	std::cout << "EXITING GEOMETRY::UPDATE" << std::endl;
}

bool Geometry::isInViewFrustum(Vertex* v, glm::mat4 proj_mat, Camera* camera, Transform worldTransform) {
	// std::cout << "Entering isInViewFrustum" << std::endl;
	glm::vec4 p(v->pos,1);
	p = proj_mat * camera->GetWorldToViewmatrix() * worldTransform.GetInternalMatrix() * p;
	if(p.w < 0
	  || p.x < -p.w || p.x > p.w
	  || p.y < -p.w || p.y > p.w
	  || p.z < -p.w || p.z > p.w) {
		// std::cout << "[" << p.x << ", " << p.y << ", " << p.z << ", " << p.w << "]" << std::endl;
		return false;
	  }
	// std::cout << "Actually, it is in the view Frustum!" << std::endl;
	return true;
}

bool Geometry::shouldSplit(Vertex* v, glm::mat4 proj_mat, Camera* camera, Transform worldTransform) {
	return isInViewFrustum(v, proj_mat, camera, worldTransform);
}

bool Geometry::canSplit(Vertex* v) {
	VertexSplit* vsplit = v->vsplit;
	if(vsplit == nullptr) {return false;}
	
	// if(vsplit.active) {
		// std::cout << "Checking canSplit(" << v << ") on edges e = " << vsplit->e << ", left = " << vsplit->activeLeftEdge << ", right = " << vsplit->activeRightEdge << std::endl;
		// std::cout << "\tLEFT COUNTER : " << vsplit->activeLeftEdge->counter  << " VS " << vsplit->leftCounter << std::endl;
		// std::cout << "\tRIGHT COUNTER : " << vsplit->activeRightEdge->counter  << " VS " << vsplit->rightCounter << std::endl;
	// }
	return vsplit->activeLeftEdge->counter  == vsplit->leftCounter
	    && vsplit->activeRightEdge->counter == vsplit->rightCounter;
}

void Geometry::SetDecimationRatio(float ratio){
	/*int target = std::round(ratio * triangleCount);
	while(m_indices.size()/3 > target) {
		RemoveEdge();
	}*/
}

void Geometry::SetRefinementRatio(float ratio){
	/* TODO: Figure this out
	int target = std::round(ratio * triangleCount);
	while(m_indices.size()/3 < target) {
		SplitVertex();
	}*/
}

void Geometry::SetDistanceFromView(float distance) {
	float effective_distance = std::clamp(distance, closeBoundary, farBoundary) - closeBoundary;
	float ratio = 1-std::clamp(effective_distance / (farBoundary - closeBoundary), 0.0f, maximumDecimationAmount);
	SetDecimationRatio(ratio);
	SetRefinementRatio(ratio);
	// std::cout << m_indices.size() / 3 << std::endl;
}

void Geometry::SetTriangleCount(int count){
	triangleCount = count;
}

// Regenerates the buffer data for the mesh
void Geometry::regen(){
	std::cout << "ENTERING REGEN" << std::endl;
	
	// updateEdges();
	// printLinkedList(edges_start);
	// std::cout << "Total triangle count: " << m_indices.size() / 3 << std::endl;

	m_bufferData.clear();
	Gen();
	m_indices.clear();
	reloadIndices();
	// std::cout << "Total faces: " << m_indices.size() << std::endl;
	std::cout << "EXITING REGEN" << std::endl;
}

void Geometry::updateEdges() {

	// while(edges_start != nullptr && !edges_start->active) {
	// 	edges_start = edges_start->next;
	// }

	// Edge* e = edges_start;
	
	// while(e != nullptr && !e->next->active) {
	// 	e->next = e->next->next;
	// }
}

void Geometry::reloadIndices() {
	// std::cout << "Entering reload indices" << std::endl;
	Face* f = faces_start;
	// std::cout << "\tf: " << f << std::endl;//" f->next: " << f->next << std::endl;
	int count = 0;
	while(f != nullptr && f->next != nullptr)
	{
		// std::cout << "\t f(" << f << "): v0 " << f->vertices[0] << " v1 " << f->vertices[1] << " v2 " << f->vertices[2] << std::endl;
		// std::cout << "face count: " << count << std::endl;
		m_indices.push_back(getActiveVertex(f->vertices[0])->ind);
		m_indices.push_back(getActiveVertex(f->vertices[1])->ind);
		m_indices.push_back(getActiveVertex(f->vertices[2])->ind);
		f = f->next;
		count++;
	}
	// std::cout << "Exiting reload indices" << std::endl;
}

// Retrieves the number of indices that we have.
unsigned int Geometry::GetIndicesSize(){
	return m_indices.size();
}

// Retrieves a pointer to the indices that we have
unsigned int* Geometry::GetIndicesDataPtr(){
	return m_indices.data();
}

// Clears all stored data
void Geometry::Reset(){
	// std::cout<< "Reset!" << std::endl;
	m_bufferData.clear();
	m_indices.clear();
	m_vertices.clear();
	_deleteVertices(vertices_start);
	_deleteEdges(edges_start);
	_deleteFaces(faces_start);
	vertices_start = nullptr;
	edges_start = nullptr;
	faces_start = nullptr;
	// std::cout << "exiting reset" << std::endl;
	// m_normals.clear();
	// m_tangents.clear();
	// m_biTangents.clear();
	// m_textureCoords.clear();
	// m_vertexPositions.clear();
	// index_map.clear();
	// m_faces.clear();
	// m_edges.clear();
	// m_wasConsumed.clear();
	// m_qMatrices.clear();
	// m_edges.clear();
	// m_vsplits.clear(); //TODO: Don't actually need this vector.
	// for (size_t i = 0; i < m_faces.size(); i++)
	// {
	// 	m_faces[i].active = true;
	// }
	
	
}

/*
IDEA:
Not that this is the avenue we're going to pursue, but thanks to the QEM, we have an error metric that allows
us to track the total amount of geometric error we've introduced to a shape. Maybe that's useful?
Probably less useful than screen space error, since it's basically a proxy for screen-space error.
*/