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
    
    void    addVertex(const vec3 &_point);
    void    addVertices(const vector<vec3>& _verts);
    void    addVertices(const vec3* verts, int amt);
    
    void    addNormal(const vec3 &_normal);
    void    addColor(const vec4 &_color);
    
    void    addIndex(uint16_t _i);
    void    addIndices(const vector<uint16_t>& inds);
    void    addIndices(const uint16_t* inds, int amt);
    
    vector<vec3> &getVertices();
    
    void    clear();
    void    draw(GLenum _drawMode = GL_POINTS);
    
private:
    vector<vec3>  vertices;
    vector<vec3>  normals;
    vector<vec4>  colors;
    vector<uint16_t> indices;
};