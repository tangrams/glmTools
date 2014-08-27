//
//  glmMesh.h
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#pragma once

#include "glmTools.h"

#include "tesselator.h"

using namespace std;
using namespace glm;

class glmMesh {
public:
    
    glmMesh();
    
    void    setDrawMode(GLenum _drawMode);
    
    void    addColor(const vec4 &_color);
    
    void    addVertex(const vec3 &_point);
    void    addVertices(const vector<vec3>& _verts);
    void    addVertices(const vec3* verts, int amt);
    
    void    addNormal(const vec3 &_normal);
    
    void    addTexCoord(const vec2 &_uv);
    
    void    addIndex(uint16_t _i);
    void    addIndices(const vector<uint16_t>& inds);
    void    addIndices(const uint16_t* inds, int amt);
    
    void    addTriangle(uint16_t index1, uint16_t index2, uint16_t index3);
    
    vector<vec3> &getVertices();
    vector<vec3> &getNormals();
    
    void    clear();
    void    draw();
    
private:
    vector<vec4>  colors;
    vector<vec3>  vertices;
    vector<vec3>  normals;
    vector<vec2>  texCoords;
    vector<uint16_t> indices;
    
    GLenum  drawMode;
};