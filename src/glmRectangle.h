//
//  glmRectangle.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/27/14.
//
//

#pragma once

#include "glmTools.h"

class glmRectangle {
public:
    
    glmRectangle();
    void    set(float _x, float _y, float _width, float _height);
    
    void    growToInclude(const vec3& p);
    
    float   getMinX() const;
    float   getMaxX() const;
    float   getMinY() const;
    float   getMaxY() const;
    
    float   x,y,width,height;
};