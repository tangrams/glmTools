//
//  glmPolyline.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/20/14.
//
//

#include "glmPolyline.h"

#include <OpenGL/gl.h>


int glmPolyline::size() const {
    return m_points.size();
}

void glmPolyline::clear(){
    m_points.clear();
}

void glmPolyline::add( const glm::vec3 & _point ){
    m_points.push_back(_point);
    m_bChange = true;
}

void glmPolyline::add(const std::vector<glm::vec3> & _points){
    for (int i = 0; i < _points.size(); i++) {
        add(_points[i]);
    }
}

void glmPolyline::add(const glm::vec3* verts, int numverts) {
    for (int i = 0 ; i < numverts ; i++) {
        add(verts[i]);
    }
}

glm::vec3& glmPolyline::operator [](const int &_index){
    return m_points[_index];
}

glm::vec3 glmPolyline::operator [](const int &_index) const {
    return m_points[_index];
}

//This is for polygon/contour simplification - we use it to reduce the number of m_points needed in
//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

typedef struct{
    glm::vec3 P0, P1;
} Segment;

#define norm2(v)   glm::dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

//--------------------------------------------------
static void simplifyDP(float tol, glm::vec3* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;
    
    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi	= j;          // index of vertex farthest from S
    float   maxd2	= 0;         // distance squared of farthest vertex
    float   tol2	= tol * tol;  // tolerance squared
    Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
    glm::vec3 u;
	u				= S.P1 - S.P0;   // segment direction vector
    float  cu		= glm::dot(u,u);     // segment length squared
    
    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    glm::vec3  w;
    glm::vec3   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared
    
    for (int i=j+1; i<k; i++){
        // compute distance squared
        w = v[i] - S.P0;
        cw = glm::dot(w,u);
        if ( cw <= 0 ) dv2 = d2(v[i], S.P0);
        else if ( cu <= cw ) dv2 = d2(v[i], S.P1);
        else {
            b = (float)(cw / cu);
            Pb = S.P0 + u*b;
            dv2 = d2(v[i], Pb);
        }
        // test with current max distance squared
        if (dv2 <= maxd2) continue;
        
        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)        // error is worse than the tolerance
    {
        // split the polyline at the farthest vertex from S
        mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
        // recursively simplify the two subpolylines at v[maxi]
        simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertices
    return;
}

void glmPolyline::simplify(float tol){
    if(m_points.size() < 2) return;
    
	int n = size();
	
	if(n == 0) {
		return;
	}
    
    std::vector<glm::vec3> sV;
	sV.resize(n);
    
    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    std::vector<glm::vec3> vt;
    std::vector<int> mk;
    vt.resize(n);
	mk.resize(n,0);
    
    
    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = m_points[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(m_points[i], m_points[pv]) < tol2) continue;
        
        vt[k++] = m_points[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = m_points[n-1];      // finish at the end
    
    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, &vt[0], 0, k-1, &mk[0] );
    
    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }
    
	//get rid of the unused m_points
	if( m < (int)sV.size() ){
		m_points.assign( sV.begin(),sV.begin()+m );
	}else{
		m_points = sV;
	}
    
    updateCache();
}

std::vector<glm::vec3> & glmPolyline::getVertices(){
	return m_points;
}

glm::vec3 glmPolyline::getPositionAt(const float &_dist) const{
    
    float distance = 0.0;
    
    for (int i = 0; i < m_points.size()-1; i++){
        
        glmPolarPoint polar(m_points[i],m_points[i+1]);
        
        if(distance+polar.r <= _dist){
            return  m_points[i] + glmPolarPoint(polar.a,_dist-distance).getXY();
        }
	}
}

void glmPolyline::draw(){
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < size(); i++) {
        glVertex2d(m_points[i].x,m_points[i].y);
    }
    glEnd();
}

glmRectangle glmPolyline::getBoundingBox() const {
	glmRectangle box;
    box.growToInclude(m_points);
	return box;
}

glm::vec3 glmPolyline::getCentroid() {
    
    if(m_bChange){
        m_centroid = glm::vec3(0.0,0.0,0.0);
        for (int i = 0; i < m_points.size(); i++) {
            m_centroid += m_points[i] / (float)m_points.size();
        }
        m_bChange = false;
    }
    
    return m_centroid;
}