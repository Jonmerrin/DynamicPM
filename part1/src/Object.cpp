#include "Object.hpp"
#include "Camera.hpp"
#include "Error.hpp"
#include "ObjectLoader.hpp"
#include "VertexData.h"


Object::Object(){
}

Object::~Object(){
    
}

// TODO: In the future it may be good to 
// think about loading a 'default' texture
// if the user forgets to do this action!
void Object::LoadTexture(std::string fileName){
        // Load our actual textures
        m_textureDiffuse.LoadTexture(fileName);
}

// Makes an empty Object (used for parenting)
void Object::MakeEmpty() { }

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Object::Bind(){
        // Make sure we are updating the correct 'buffers'
        m_vertexBufferLayout.Bind();
        // Diffuse map is 0 by default, but it is good to set it explicitly
        m_textureDiffuse.Bind(0);
}

// Make an object from an obj file
void Object::MakeObjectFromFile(std::string fileName, float tolerance){
        if (tolerance >= 0)
                m_geometry.SetTolerance(tolerance);
        m_geometry.Reset();
        ObjectLoader loader;
        objPath = fileName;
        std::vector<VertexData> verts;
        std::vector<unsigned int> faces;
        std::string mat;
        // Load obj information
        loader.BufferObject(fileName, verts, faces, mat);
        
        // Add the vertex data
        for (int i = 0; i < verts.size(); i++) {
                m_geometry.AddVertex(verts[i].x, verts[i].y, verts[i].z, verts[i].s, verts[i].t);
        }
        // Add the triangle data
        for (int i = 0; i < faces.size(); i= i + 3) {
                m_geometry.MakeTriangle(faces[i], faces[i+1], faces[i+2]);
        }
        m_geometry.SetTriangleCount(faces.size()/3);
        m_geometry.Gen();

        // Create a buffer and set the stride of information
        // NOTE: How we are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        m_vertexBufferLayout.CreateTextureBufferLayout(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());
}

// Render our geometry
void Object::Render(){
    // Call our helper function to just bind everything
    Bind();
	//Render data
    glDrawElements(GL_TRIANGLES,
                   m_geometry.GetIndicesSize(), // The number of indicies, not triangles.
                   GL_UNSIGNED_INT,             // Make sure the data type matches
                        nullptr);               // Offset pointer to the data. 
                                                // nullptr because we are currently bound
}

// Decimates the mesh by a given ratio
// void Object::DecimateByRatio(float ratio){

//         int targetSize = m_geometry.GetIndicesSize() * ratio;
//         while (m_geometry.GetIndicesSize() > targetSize)
//         {
//                 m_geometry.RemoveEdge();
//         }
        
//         Bind();

//         m_vertexBufferLayout.BufferNewData(m_geometry.GetBufferDataSize(),
//                                         m_geometry.GetIndicesSize(),
//                                         m_geometry.GetBufferDataPtr(),
//                                         m_geometry.GetIndicesDataPtr());
// }

// void Object::SetRefinementRatio() {

// }

// void Object::SetDecimationRatio() {

// }

void Object::SetDistanceFromView(float distance) {
        m_geometry.SetDistanceFromView(distance);
        
        Bind();

        m_vertexBufferLayout.BufferNewData(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());
}

// // Contracts a single pair of vertices
// void Object::ContractVertexPair(){
//         m_geometry.RemoveEdge();
        
//         Bind();
//         // Note: Could be made more efficient using BufferMapping
//         m_vertexBufferLayout.BufferNewData(m_geometry.GetBufferDataSize(),
//                                         m_geometry.GetIndicesSize(),
//                                         m_geometry.GetBufferDataPtr(),
//                                         m_geometry.GetIndicesDataPtr());
// }

// void Object::SplitVertex(){
//         m_geometry.SplitVertex();

//         Bind();
//         // Note: Could be made more efficient using BufferMapping
//         m_vertexBufferLayout.BufferNewData(m_geometry.GetBufferDataSize(),
//                                         m_geometry.GetIndicesSize(),
//                                         m_geometry.GetBufferDataPtr(),
//                                         m_geometry.GetIndicesDataPtr());        
// }

void Object::Update(glm::mat4 proj_mat, Camera* camera, Camera* unhookedCamera, Transform worldTransform){
        m_geometry.Update(proj_mat, camera, unhookedCamera, worldTransform);
        
        Bind();
        
        m_vertexBufferLayout.BufferNewData(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());
}

// Resets the mesh to its un-decimated
void Object::Reset(){
        MakeObjectFromFile(objPath, -1);
}

// Get the width of the model (display purposes only)
float Object::GetWidth(){
        m_geometry.GetWidth();
}