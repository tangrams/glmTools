//
//  glmPolyline.h
//
//  Created by Patricio Gonzalez Vivo on 8/20/14.
//
//

#pragma once

#include "glmTools.h"
#include "glmMesh.h"

using namespace std;
using namespace glm;

struct glmPolarPoint {
    glmPolarPoint(const glm::vec2 &_org, const glm::vec2 &_dst){
        vec2 diff = _dst - _org;
        a = atan2(diff.y,diff.x);
        r = length(diff);
    };
    double a,r;
};

class glmPolyline {
public:
    
    void    add(const glm::vec2 &_point);
    void    add(const std::vector<glm::vec2> & _points);
    
    vec2&   operator [](const int &_index);
    double  getLength(const int &_index = -1) const;
    
    vec2    getPositionAt(const double &_dist) const;
    double  getAngleAt(const double &_dist) const;
    
    double  getFractAt(double _dist,double _offset=1.)const;
    glmMesh getMesh(double _width);
    
    int     size() const;

    void    clear();
    void    simplify(double tolerance=0.3f);
    
    void    draw();
    void    drawPoints();
    void    drawNormals();

protected:
    vector<vec2>    cartesians;
    
    //  Cached data
    //
    void    updateCache();
    vector<glmPolarPoint>  polars;
    vector<double>         distances;
};