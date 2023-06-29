#ifndef OBJECTLOADER
#define OBJECTLOADER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <algorithm>
#include "VertexData.h"



class ObjectLoader{
public:
    ObjectLoader();
    ~ObjectLoader();
    void BufferObject(std::string path, std::vector<VertexData>& vertices, std::vector<GLuint>& faces, std::string& material);

private:   
    VertexData getVertexDataFromToken(std::string token, std::vector<float> vs, std::vector<float> vts);
};

#endif