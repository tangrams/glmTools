//
//  glmSmartLine.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 9/8/14.
//
//

#pragma once

#include "glmTools.h"
#include "glmPolyline.h"

struct glmPolarPoint {
    glmPolarPoint(const glm::vec3 &_org, const glm::vec3 &_dst){
        glm::vec3 diff = _dst - _org;
        a = atan2(diff.y,diff.x);
        r = glm::length(glm::vec2(diff.x,diff.y));
    };
    
    float a,r;
};

class glmSmartLine : public glmPolyline {
public:
    
    glmSmartLine();
    virtual ~glmSmartLine();
    
    void    add(const glm::vec3 &_point);
    
    float   getLength(const int &_index = -1) const;
    glm::vec3 getPositionAt(const float &_dist) const;
    float   getAngleAt(const float &_dist) const;
    float   getFractAt(const float &_dist, const float &_offset=1.) const;

    void    clear();
    
    void    drawNormals();
    
    std::vector<float>   marks;
    glm::vec3   originalCentroid;
    
protected:
    
    //  Cached data
    //
    void    updateCache();
    std::vector<glmPolarPoint>  m_polars;
    std::vector<float>          m_distances;
};