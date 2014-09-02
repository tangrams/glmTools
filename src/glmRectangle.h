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
    
    void    growToInclude(const glm::vec3& p);
    
    bool    inside(float px, float py) const;
    bool    inside(const glm::vec3& p) const;
    bool    inside(const glmRectangle& rect) const;
    bool    inside(const glm::vec3& p0, const glm::vec3& p1) const;
    
    bool    intersects(const glmRectangle& rect) const;
    bool    intersects(const glm::vec3& p0, const glm::vec3& p1) const;
    
    glm::vec3    getMin() const;
    glm::vec3    getMax() const;
    
    float   getMinX() const;
    float   getMaxX() const;
    float   getMinY() const;
    float   getMaxY() const;
    
    float   getLeft()   const;
    float   getRight()  const;
    float   getTop()    const;
    float   getBottom() const;
    
    glm::vec3   getCenter() const;
    
    glm::vec3    getTopLeft() const;
    glm::vec3    getTopRight() const;
    glm::vec3    getBottomLeft() const;
    glm::vec3    getBottomRight() const;
    
    void drawBorders();
    void drawCorners(const float &_width = 3.);
    
    float   x,y,width,height;
};