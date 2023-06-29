#include "ObjectLoader.hpp"

ObjectLoader::ObjectLoader()
{
}

ObjectLoader::~ObjectLoader()
{
}

VertexData ObjectLoader::getVertexDataFromToken(std::string token, std::vector<float> vs, std::vector<float> vts) {
    unsigned int vertex_index, texture_index, normal_index;
    bool hasTexture = true;
    if (token.find_first_of("1234567890") != std::string::npos) {
        vertex_index = std::stoi(token) - 1;
        if (token.find("/") != std::string::npos) {
            token = token.substr(token.find("/"));
            token = token.substr(token.find_first_not_of("/"));
        }
    }
    
    if (token.find_first_of("1234567890") != std::string::npos) {
        if (token.find("/") != std::string::npos) {
            texture_index = std::stoi(token) - 1;
            token = token.substr(token.find("/"));
            token = token.substr(token.find_first_not_of("/"));
        } else {
            normal_index = std::stoi(token) - 1;
            hasTexture = false;
        }
    }

    if (hasTexture && token.find_first_of("1234567890") != std::string::npos) {
        normal_index = std::stoi(token) - 1;
    }
    // TODO: this is janky and easy to fix. Don't forget!
    VertexData vdata(0,0,0,0,0);
    if (hasTexture) {
        vdata = VertexData(vs[vertex_index*3],
                        vs[vertex_index*3+1],
                        vs[vertex_index*3+2],
                        vts[texture_index*2],
                        vts[texture_index*2+1]);
    } else {
        vdata = VertexData(vs[vertex_index*3],
                        vs[vertex_index*3+1],
                        vs[vertex_index*3+2],
                        0,
                        0);
    }
    return vdata;

}

void ObjectLoader::BufferObject(std::string path, std::vector<VertexData> &vertices, std::vector<unsigned int> &faces, std::string& material)
{
    // First load up all the vertices, vts and vns into their own vectors, then start adding them to the vertexbuffer
    // Per face:
    // Check if such an element exists in the vertex buffer
    // If it does, append the index of that element to the faces list
    // else add that elemenet to the VBO and put the index of the element in the faces list

    std::vector<GLfloat> vs, vts, vns;

    int offset = vertices.size() / 3;
    // Open the file to read in from
    std::ifstream obj;
    obj.open(path);

    // If successful
    if (obj.is_open())
    {

        std::string line;
        while (getline(obj, line))
        {
            std::stringstream stream(line);
            std::string token;
            std::string header;
            if (!(stream >> header)) {
                continue;
            }
            
            while (stream >> token)
            {
                if(header == "v") {                 // Adding vertex position data
                    vs.push_back(std::stof(token));
                } else if(header == "vt") {         // Adding vertex texture data
                    vts.push_back(std::stof(token));
                } else if(header == "vn") {         // Adding vertex normal data
                    vns.push_back(std::stof(token));
                } else if(header == "mtllib"){      // Finding path to our material file
                    material = path.substr(0, path.find_last_of("/")+1) + token;
                    continue;
                } else if(header == "f") {          // Adding face data
                    // Parsing information for faces got a little cumbersome, moved it to a helper function
                    VertexData vdata = getVertexDataFromToken(token, vs, vts);

                    // Check if the vertex already exists, point to it if it does, add it if it doesn't.
                    std::vector<VertexData>::iterator existingVertexPos = std::find(vertices.begin(), vertices.end(), vdata);
                    if (existingVertexPos != vertices.end()) {
                        faces.push_back(existingVertexPos - vertices.begin());
                    } else {
                        vertices.push_back(vdata);
                        faces.push_back(vertices.size() - 1);
                    }
                } else {
                    break;
                }
            }
        }
    }
    obj.close(); 
    std::cout << "Finished loading object" << std::endl;
}