//
//  glmPolyline.h
//
//  Created by Patricio Gonzalez Vivo on 8/20/14.
//  Inspired by: https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/graphics/ofPolyline.h
//

#pragma once

#include "glmTools.h"
#include "glmMesh.h"
#include "glmRectangle.h"

struct glmPolarPoint {
    
    glmPolarPoint():a(0.0),r(0.0){};
    
    glmPolarPoint(const float &_angle, const float &_radius){
        a = _angle;
        r = _radius;
    };
    
    glmPolarPoint(const glm::vec2 &_org, const glm::vec2 &_dst){
        glm::vec2 diff = _dst - _org;
        a = atan2(diff.y,diff.x);
        r = glm::length(diff);
    };
    
    glmPolarPoint(const glm::vec3 &_org, const glm::vec3 &_dst){
        glm::vec3 diff = _dst - _org;
        a = atan2(diff.y,diff.x);
        r = glm::length(glm::vec2(diff.x,diff.y));
    };
    
    glm::vec3 getCartesian(){
        return glm::vec3(r*cos(a), r*sin(a), 0.0);
    }
    
    float a,r;
};

struct glmLinePair {
    std::vector<glm::vec3> A,B;
};

class glmPolyline {
public:
    
    glmPolyline():m_centroid(0.0,0.0,0.0),m_bChange(true){}
    virtual ~glmPolyline(){}
    
    virtual void add(const glm::vec3 &_point);
    void    add(const std::vector<glm::vec3> & _points);
	void    add(const glm::vec3* verts, int numverts);
    
    glm::vec3&  operator [](const int &_index);
    glm::vec3   operator [](const int &_index) const;
    
    glm::vec3   getCentroid();
    std::vector<glm::vec3> & getVertices();
    glm::vec3   getPositionAt(const float &_dist) const;

	glmRectangle getBoundingBox() const;
    
    void    addToBoundingBox(glmRectangle &_bbox) const;
    void    addAsLineToMesh(glmMesh &_mesh, float _width = 3.0, bool _TRIANGLE_STRIP = false);
    void    addAsShapeToMesh(glmMesh &_mesh);
    
    int     size() const;

    glmLinePair splitAt();
    virtual void    clear();
    void    simplify(float tolerance=0.3f);
    
    void    draw();
    void    drawStipple(GLushort _pattern = 0x1111);
    void    drawPoints();

protected:
    virtual void updateCache(){};
    
    std::vector<glm::vec3>  m_points;
    glm::vec3   m_centroid;
    
    bool        m_bChange;
};