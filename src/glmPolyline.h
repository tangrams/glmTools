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
    glmPolarPoint(const glm::vec3 &_org, const glm::vec3 &_dst){
        
        //  TODO: pass this to 3D
        //
        
        glm::vec3 diff = _dst - _org;
        a = atan2(diff.y,diff.x);
        r = glm::length(glm::vec2(diff.x,diff.y));

    };
    float a,r;
};

class glmPolyline {
public:
    
    void    add(const glm::vec3 &_point);
    void    add(const std::vector<glm::vec3> & _points);
    void    addVertices( const std::vector<glm::vec3>& verts );
	void    addVertices(const glm::vec3* verts, int numverts);
    
    glm::vec3&   operator [](const int &_index);
    float  getLength(const int &_index = -1) const;
    
    glm::vec3    getPositionAt(const float &_dist) const;
    float  getAngleAt(const float &_dist) const;
    
    std::vector<glm::vec3> & getVertices();
    float  getFractAt(float _dist,float _offset=1.)const;

	glmRectangle getBoundingBox() const;
    void    growToInclude(glmRectangle &_bbox) const;
    
    void    addAsLineToMesh(glmMesh &_mesh, float _width = 3.0);
    void    addAsShapeToMesh(glmMesh &_mesh);
    
    int     size() const;

    void    clear();
    void    simplify(float tolerance=0.3f);
    
    void    draw();
    void    drawPoints();
    void    drawNormals();

protected:
    std::vector<glm::vec3>    cartesians;
    
    //  Cached data
    //
    void    updateCache();
    std::vector<glmPolarPoint>  polars;
    std::vector<float>         distances;
};