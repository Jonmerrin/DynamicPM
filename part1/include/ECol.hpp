#ifndef EDGE_COLLAPSE
#define EDGE_COLLAPSE

#include "glm/glm.hpp"
#include <vector>

struct ECol
{
    int v1;
    int v2;
    glm::vec3 newPos;
    std::vector<int> connections;
    std::vector<int> facesToBeRemoved;
    
    ECol(int _v1, int _v2, glm::vec3 _newPos, std::vector<int> _connections, std::vector<int> _faces): v1(_v1), v2(_v2), newPos(_newPos),
        connections(_connections), facesToBeRemoved(_faces){ }

};

#endif