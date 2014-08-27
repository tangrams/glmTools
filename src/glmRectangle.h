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
    glmRectangle(float _x, float _y, float _width, float _height);
    
    void    set(float _x, float _y, float _width, float _height);
    
    void    growToInclude(const vec3& p);
    
    bool    inside(float px, float py) const;
    bool    inside(const vec3& p) const;
    bool    inside(const glmRectangle& rect) const;
    bool    inside(const vec3& p0, const vec3& p1) const;
    
    bool    intersects(const glmRectangle& rect) const;
    bool    intersects(const vec3& p0, const vec3& p1) const;
    
    vec3    getMin() const;
    vec3    getMax() const;
    
    float   getMinX() const;
    float   getMaxX() const;
    float   getMinY() const;
    float   getMaxY() const;
    
    float   getLeft()   const;
    float   getRight()  const;
    float   getTop()    const;
    float   getBottom() const;
    
    vec3    getTopLeft() const;
    vec3    getTopRight() const;
    vec3    getBottomLeft() const;
    vec3    getBottomRight() const;
    
    float   x,y,width,height;
};