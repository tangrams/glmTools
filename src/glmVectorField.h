//
//  glmVectorField.h
//
//  Created by Patricio Gonzalez Vivo on 9/25/14.
//
//

#pragma once

#include "glmTools.h"

class glmVectorField {
public:
    glmVectorField();
    virtual ~glmVectorField();
    
    void    set( int _width, int _height, int _res );
    
    glm::vec3 &	getForceAt(const glm::vec3 &_pos);
    
    void    draw();

private:
    void    deallocate();
    
    glm::vec3 **m_forces;
    int         m_width, m_height;
    int         m_cols, m_rows;
    int         m_resolution;
};