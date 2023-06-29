/** @file Object.hpp
 *  @brief Sets up an OpenGL camera..
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>
#include <queue>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"
#include "Edge.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Load a texture
    void LoadTexture(std::string fileName);
    // Make an empty Object
    void MakeEmpty();
    // Makes a 3D model from an obj file
    void MakeObjectFromFile(std::string fileName, float tolerance);
    // Simplifies an object to a target level of detail
    // void DecimateByRatio(float ratio);
    // Removes a single vertex pair from the mesh
    // void ContractVertexPair();
    // Splits the most recent vertex that has been contracted from a pair
    // void SplitVertex();
    void Update(glm::mat4 proj_mat, Camera *camera, Camera * unhookedCamera, Transform worldTransform);
    // Resets the mesh back to its original geometry
    void Reset();
    // How to draw the object
    virtual void Render();
    // Get the width of the model (for placement against the reference model)
    float GetWidth();

    void SetDistanceFromView(float distance);

protected: // Classes that inherit from Object are intended to be overriden.

	// Helper method for when we are ready to draw or update our object
	void Bind();
    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // For now we have one diffuse map and one normal map per object
    Texture m_textureDiffuse;
    // Store the objects Geometry
	Geometry m_geometry;
    // Store the name of the obj file
    std::string objPath;
};


#endif
