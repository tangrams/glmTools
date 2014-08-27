//
//  glmRectangle.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/27/14.
//
//

#include "glmRectangle.h"

glmRectangle::glmRectangle(){
    x = 0.0;
    y = 0.0;
    width = 0.0;
    height = 0.0;
}

void glmRectangle::set(float _x, float _y, float _width, float _height){
    x = _x;
    y = _y;
    width = _width;
    height = _height;
}

//----------------------------------------------------------
float glmRectangle::getMinX() const {
    return MIN(x, x + width);  // - width
}

//----------------------------------------------------------
float glmRectangle::getMaxX() const {
    return MAX(x, x + width);  // - width
}

//----------------------------------------------------------
float glmRectangle::getMinY() const{
    return MIN(y, y + height);  // - height
}

//----------------------------------------------------------
float glmRectangle::getMaxY() const {
    return MAX(y, y + height);  // - height
}

void glmRectangle::growToInclude(const vec3& p){
    float x0 = MIN(getMinX(),p.x);
    float x1 = MAX(getMaxX(),p.x);
    float y0 = MIN(getMinY(),p.y);
    float y1 = MAX(getMaxY(),p.y);
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0,y0,w,h);
}