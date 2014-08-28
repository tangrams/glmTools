//
//  glmMesh.h
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#pragma once

#include "glmTools.h"

class glmMesh {
public:
    
    glmMesh();
    
    void    setDrawMode(GLenum _drawMode);
    
    void    addColor(const glm::vec4 &_color);
    
    void    addVertex(const glm::vec3 &_point);
    void    addVertices(const std::vector<glm::vec3>& _verts);
    void    addVertices(const glm::vec3* verts, int amt);
    
    void    addNormal(const glm::vec3 &_normal);
    
    void    addTexCoord(const glm::vec2 &_uv);
    
    void    addIndex(uint16_t _i);
    void    addIndices(const std::vector<uint16_t>& inds);
    void    addIndices(const uint16_t* inds, int amt);
    
    void    addTriangle(uint16_t index1, uint16_t index2, uint16_t index3);
    
    std::vector<glm::vec3> &getVertices();
    std::vector<glm::vec3> &getNormals();
    std::vector<uint16_t> &getIndices();
    
    void    clear();
    void    draw();
    
private:
    std::vector<glm::vec4>  colors;
    std::vector<glm::vec3>  vertices;
    std::vector<glm::vec3>  normals;
    std::vector<glm::vec2>  texCoords;
    std::vector<uint16_t> indices;
    
    GLenum  drawMode;
};