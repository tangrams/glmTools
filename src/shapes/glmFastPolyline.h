//
//  glmFastPolyline.h
//
//  Created by Patricio Gonzalez Vivo on 9/8/14.
//
//

#pragma once

#include "glmTools.h"
#include "glmPolyline.h"
#include "glmPolarPoint.h"

class glmFastPolyline : public glmPolyline {
public:
    
    glmFastPolyline();
    glmFastPolyline(const glmPolyline &_poly);
    glmFastPolyline(const std::vector<glm::vec3> &_points);
    virtual ~glmFastPolyline();
    
    virtual void add(const glm::vec3 &_point);
    virtual void clear();
    
    float       getLength(const int &_index = -1) const;
    glm::vec3   getPositionAt(const float &_dist) const;
    float       getAngleAt(const float &_dist) const;
    float       getFractAt(const float &_dist, const float &_offset=1.) const;
    
    const std::vector<float> & getDistances() const;
    const std::vector<glmPolarPoint> & getPolars() const;
    
    void    drawNormals();
    
protected:

    void    updateCache();
    std::vector<glmPolarPoint>  m_polars;
    std::vector<float>          m_distances;
};