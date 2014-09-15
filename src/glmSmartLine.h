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
#include "glmPolarPoint.h"

class glmSmartLine : public glmPolyline {
public:
    
    glmSmartLine();
    virtual ~glmSmartLine();
    
    void    add(const glm::vec3 &_point);
    
    float   getLength(const int &_index = -1) const;
    glm::vec3 getPositionAt(const float &_dist) const;
    
    const std::vector<glmPolarPoint> & getPolars() const;
    const std::vector<float> & getDistances() const;
    
    float   getAngleAt(const float &_dist) const;
    float   getFractAt(const float &_dist, const float &_offset=1.) const;

    void    clear();
    
    void    drawNormals();
    
    glm::vec3   originalCentroid;
    
protected:
    
    //  Cached data
    //
    void    updateCache();
    std::vector<glmPolarPoint>  m_polars;
    std::vector<float>          m_distances;
};