/** @file Geometry.hpp
 *  @brief Organizes vertex and triangle information.
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <stack>
#include <queue>
#include "Edge.hpp"
#include "Face.hpp"
#include "Vertex.hpp"
#include "VertexSplit.hpp"
#include "glm/glm.hpp"
#include "Camera.hpp"
#include "Transform.hpp"

// Purpose of this class is to store vertice and triangle information
class Geometry{
public:
	// Constructor
	Geometry();
    // Destructor
    ~Geometry();
	

	void SetTolerance(float newT);

	void Update(glm::mat4 proj_mat, Camera* camera, Camera* unhookedCamera, Transform worldTransform);

    // Functions for working with individual vertices
	unsigned int GetBufferSizeInBytes();
    // Retrieve the Buffer Data Size
	unsigned int GetBufferDataSize();
	// Retrieve the Buffer Data Pointer
	float* GetBufferDataPtr();
	// Initialize Geometry Given Faces and Vertex Data
	// Add a new vertex 
	void AddVertex(float x, float y, float z, float s, float t);
	// Allows for adding one index at a time manually if 
	// you know which vertices are needed to make a triangle.
	void AddIndex(unsigned int i);
    // Gen pushes all attributes into a single vector
	void Gen();
	// Functions for working with Indices
	// Creates a triangle from 3 indicies
	// When a triangle is made, the tangents and bi-tangents are also
	// computed
	void MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2);  
    // Retrieve how many indicies there are
	unsigned int GetIndicesSize();
    // Retrieve the pointer to the indices
	unsigned int* GetIndicesDataPtr();
	// Retrieve a particular vertex's position
	glm::vec3 GetVertexPosition(unsigned int index);
	// Retrieve a vertex's Q matrix
	glm::mat4 GetVertexQMatrix(unsigned int index);
	// Removes an edge
    void RemoveEdge(Edge* e);
    // Splits a vertex
    void SplitVertex(Vertex* v);
    // Readds an edge
    Edge* AddEdge(Vertex* v1, Vertex* v2);
	// Resets the Geometry to empty
	void Reset();
	// Gets the width of the model (along the x axis)
	float GetWidth();

	void SetDistanceFromView(float distance);
	void SetTriangleCount(int count);
private:

	// cleanup functions
    void _deleteVertices(Vertex *v);
    void _deleteEdges(Edge *e);
    void _deleteFaces(Face *f);

	// m_bufferData stores all of the vertexPositons, coordinates, normals, etc.
	// This is all of the information that should be sent to the vertex Buffer Object
	std::vector<float> m_bufferData;

	// The q_matrices for each vertex in our object
	// std::vector<glm::mat4> m_qMatrices;
	// A vector of flags to check if that vertex was consumed yet. Used for validating old edges
	// std::vector<bool> m_wasConsumed;
	
	// std::vector<VertexSplit> vSplits; //TODO: make vertex data structure that has vsplit associated
									  //	  also add prev vSplits to VertexSplit data structure. Also don't forget to delete allocated vsplits.
	// A stack of the removed edges
// //	std::stack<Edge> removedEdges;
// 	std::vector<Edge> m_edges;
// 	std::vector<Face> m_faces;
	glm::vec4 computePlaneFromPoints(int v1, int v2, int v3);
	void _removeEdge(Edge* edge);
    void printLinkedList(Vertex *vert);
    void RemoveNodeFromList(Vertex *v);
    void RemoveNodeFromList(Edge *e);
    void RemoveNodeFromList(Face *f);
    void InsertNodeToList(Vertex *v);
    void InsertNodeToList(Edge *e);
    void InsertNodeToList(Face *f);
    void printLinkedList(Edge *vert);
    void printLinkedList(Face *face);
    bool CycleCheck();
    Vertex *getActiveVertex(Vertex *vert);
    void regen();
    void updateEdges();
    void reloadIndices();
    bool isInViewFrustum(Vertex* v, glm::mat4 proj_mat, Camera* camera, Transform worldTransform);
    bool shouldSplit(Vertex* v, glm::mat4 proj_mat, Camera* camera, Transform worldTransform);
    bool canSplit(Vertex* v);
	bool isBoundaryEdge(Edge* e);
    // The tolerance for contracting unconnected vertex pairs
	float tolerance = 0;
	// The width of the model (along the x axis)
	float width;

    void SetDecimationRatio(float ratio);
    void SetRefinementRatio(float ratio);

	float closeBoundary = 10.0f;
	float farBoundary = 50;
	float maximumDecimationAmount = 0.95f;
	int triangleCount;

    // Individual components of 
	std::vector<Vertex *> m_vertices;
	// std::vector<float> m_vertexPositions;
	// std::vector<float> m_textureCoords;
	// std::vector<float> m_normals;
	// std::vector<float> m_tangents;
	// std::vector<float> m_biTangents;
	// std::vector<VertexSplit> m_vsplits;
	// std::vector<int> index_map;
	// std::vector<bool> should_collapse;

	Vertex* vertices_start = nullptr;
	Edge* edges_start = nullptr;
	Face* faces_start = nullptr;

	// The indices for a indexed-triangle mesh
	std::vector<unsigned int> m_indices;
};





#endif
