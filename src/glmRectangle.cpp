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

glmRectangle::glmRectangle(float _x, float _y, float _width, float _height){
    set(_x, _y, _width, _height);
}

void glmRectangle::set(float _x, float _y, float _width, float _height){
    x = _x;
    y = _y;
    width = _width;
    height = _height;
}


//----------------------------------------------------------
vec3 glmRectangle::getMin() const {
    return vec3(getMinX(),getMinY(),0.);
}

//----------------------------------------------------------
vec3 glmRectangle::getMax() const {
    return vec3(getMaxX(),getMaxY(),0.);
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

bool glmRectangle::inside(float px, float py) const {
	return inside(vec3(px,py,0.));
}

float glmRectangle::getLeft() const {
    return getMinX();
}

//----------------------------------------------------------
float glmRectangle::getRight() const {
    return getMaxX();
}

//----------------------------------------------------------
float glmRectangle::getTop() const {
    return getMinY();
}

//----------------------------------------------------------
float glmRectangle::getBottom() const {
    return getMaxY();
}

//----------------------------------------------------------
vec3 glmRectangle::getTopLeft() const {
    return getMin();
}

//----------------------------------------------------------
vec3 glmRectangle::getTopRight() const {
    return vec3(getRight(),getTop(),0.);
}

//----------------------------------------------------------
vec3 glmRectangle::getBottomLeft() const {
    return vec3(getLeft(),getBottom(),0.);
}

//----------------------------------------------------------
vec3 glmRectangle::getBottomRight() const {
    return getMax();
}

//----------------------------------------------------------
bool glmRectangle::inside(const vec3& p) const {
    return p.x > getMinX() && p.y > getMinY() &&
    p.x < getMaxX() && p.y < getMaxY();
}

//----------------------------------------------------------
bool glmRectangle::inside(const glmRectangle& rect) const {
    return inside(rect.getMinX(),rect.getMinY()) &&
    inside(rect.getMaxX(),rect.getMaxY());
}

//----------------------------------------------------------
bool glmRectangle::inside(const vec3& p0, const vec3& p1) const {
    // check to see if a line segment is inside the rectangle
    return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool glmRectangle::intersects(const glmRectangle& rect) const {
    return (getMinX() < rect.getMaxX() && getMaxX() > rect.getMinX() &&
            getMinY() < rect.getMaxY() && getMaxY() > rect.getMinY());
}

//----------------------------------------------------------
bool glmRectangle::intersects(const vec3& p0, const vec3& p1) const {
    // check for a line intersection
    vec3 p;
    
    vec3 topLeft     = getTopLeft();
    vec3 topRight    = getTopRight();
    vec3 bottomRight = getBottomRight();
    vec3 bottomLeft  = getBottomLeft();
    
    return inside(p0) || // check end inside
    inside(p1) || // check end inside
    lineSegmentIntersection(p0, p1, topLeft,     topRight,    p) || // cross top
    lineSegmentIntersection(p0, p1, topRight,    bottomRight, p) || // cross right
    lineSegmentIntersection(p0, p1, bottomRight, bottomLeft,  p) || // cross bottom
    lineSegmentIntersection(p0, p1, bottomLeft,  topLeft,     p);   // cross left
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