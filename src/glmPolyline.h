//
//  glmPolyline.h
//
//  Created by Patricio Gonzalez Vivo on 8/20/14.
//
//

#pragma once

#include "glmTools.h"
#include "glmMesh.h"
#include "glmRectangle.h"

struct glmPolarPoint {
    glmPolarPoint(const vec3 &_org, const vec3 &_dst){
        
        //  TODO: pass this to 3D
        //
        
        vec3 diff = _dst - _org;
        a = atan2(diff.y,diff.x);
        r = length(vec2(diff.x,diff.y));

    };
    double a,r;
};

class glmPolyline {
public:
    
    void    add(const vec3 &_point);
    void    add(const vector<vec3> & _points);
    void    addVertices( const vector<vec3>& verts );
	void    addVertices(const vec3* verts, int numverts);
    
    vec3&   operator [](const int &_index);
    double  getLength(const int &_index = -1) const;
    
    vec3    getPositionAt(const double &_dist) const;
    double  getAngleAt(const double &_dist) const;
    
    vector<vec3> & getVertices();
    double  getFractAt(double _dist,double _offset=1.)const;
    glmMesh getMesh(double _width = 3.0);
	glmRectangle getBoundingBox() const;
    void    growToInclude(glmRectangle &_bbox) const;
    
    void    addToMesh(glmMesh &_mesh, double _width = 3.0);
    
    int     size() const;

    void    clear();
    void    simplify(double tolerance=0.3f);
    
    void    draw();
    void    drawPoints();
    void    drawNormals();

protected:
    vector<vec3>    cartesians;
    
    //  Cached data
    //
    void    updateCache();
    vector<glmPolarPoint>  polars;
    vector<double>         distances;
};