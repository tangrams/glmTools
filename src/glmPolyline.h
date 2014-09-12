//
//  glmPolyline.h
//
//  Created by Patricio Gonzalez Vivo on 8/20/14.
//  Inspired by: https://github.com/openframeworks/openFrameworks/blob/master/libs/openFrameworks/graphics/ofPolyline.h
//

#pragma once

#include "glmTools.h"
#include "glmRectangle.h"

struct glmPolarPoint {
    glmPolarPoint():a(0.0),r(0.0),z(0.0){};
    glmPolarPoint(const float &_angle, const float &_radius):a(_angle),r(_radius),z(0.0){};
    glmPolarPoint(const glm::vec3 &_org, const glm::vec3 &_dst):a(0.0),r(0.0),z(0.0){
        glm::vec3 diff = _dst - _org;
        a = atan2(diff.y,diff.x);
        r = glm::length(glm::vec2(diff.x,diff.y));
        z = diff.z;
    };
    
    glm::vec3 getXY(){return glm::vec3(r*cos(a), r*sin(a), z);}
    
    float a,r,z;
};

class glmPolyline {
public:
    
    glmPolyline();
    virtual ~glmPolyline();
    
    virtual void add(const glm::vec3 &_point);
    void    add(const std::vector<glm::vec3> & _points);
	void    add(const glm::vec3* verts, int numverts);
    
    glm::vec3&  operator [](const int &_index);
    glm::vec3   operator [](const int &_index) const;
    
    virtual glm::vec3   getCentroid();
    std::vector<glm::vec3> & getVertices();
    virtual glm::vec3   getPositionAt(const float &_dist) const;

	glmRectangle getBoundingBox() const;
    
    int     size() const;

    std::vector<glmPolyline> splitAt(float _dist);
    std::vector<glmPolyline> splitAtIntersection(const glmPolyline &_other);
    
    virtual void    clear();
    void    simplify(float tolerance=0.3f);
    
    void    draw();

protected:
    virtual void updateCache(){};
    
    std::vector<glm::vec3>  m_points;
    glm::vec3   m_centroid;
    
    bool        m_bChange;
};