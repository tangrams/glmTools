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

class glmPolyline {
public:
    
    glmPolyline(){}
    virtual ~glmPolyline(){}
    
    virtual void    add(const glm::vec3 &_point);
    void    add(const std::vector<glm::vec3> & _points);
    void    addVertices( const std::vector<glm::vec3>& verts );
	void    addVertices(const glm::vec3* verts, int numverts);
    
    glm::vec3&  operator [](const int &_index);
    glm::vec3   operator [](const int &_index) const;
    
    std::vector<glm::vec3> & getVertices();

	glmRectangle getBoundingBox() const;
    
    void    addToBoundingBox(glmRectangle &_bbox) const;
    void    addAsLineToMesh(glmMesh &_mesh, float _width = 3.0, bool _TRIANGLE_STRIP = true);
    void    addAsShapeToMesh(glmMesh &_mesh);
    
    int     size() const;

    virtual void    clear();
    void    simplify(float tolerance=0.3f);
    
    void    draw();
    void    drawStipple(GLushort _pattern = 0x1111);
    void    drawPoints();

protected:
    virtual void updateCache(){};
    
    std::vector<glm::vec3>  m_points;
};