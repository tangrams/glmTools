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
    
    int     getNumVertex();
    
    void    addVertex(const vec3 &_point);
    void    addNormal(const vec3 &_normal);
    void    addColor(const vec4 &_color);
    void    addIndex(TESSindex _i);
    
    void    clear();
    void    draw(GLenum _drawMode = GL_POINTS);
    
private:
    vector<vec3>  vertices;
    vector<vec3>  normals;
    vector<vec4>  colors;
    vector<TESSindex> indices;
};