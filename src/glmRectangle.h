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
    glmRectangle(const glm::vec4 &_vec4);
    glmRectangle(const glm::ivec4 &_viewPort);
    glmRectangle(const float &_x, const float &_y, const float &_width, const float &_height);
    
    void    set(const glm::vec4 &_vec4);
    void    set(const glm::ivec4 &_viewPort);
    void    set(const float &_x, const float &_y, const float &_width, const float &_height);
    void    growToInclude(const glm::vec3& p);
    
    bool    inside(const float &_px, const float &_py) const;
    bool    inside(const glm::vec3& _p) const;
    bool    inside(const glmRectangle& _rect) const;
    bool    inside(const glm::vec3& p0, const glm::vec3& p1) const;
    
    bool    intersects(const glmRectangle& _rect) const;
    bool    intersects(const glm::vec3& _p0, const glm::vec3& _p1) const;
    
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