//
//  glmPolyline.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/20/14.
//
//

#include "glmPolyline.h"

#include <OpenGL/gl.h>
#include "tesselator.h"

int glmPolyline::size() const {
    return cartesians.size();
}

void glmPolyline::clear(){
    cartesians.clear();
    polars.clear();
    
    distances.clear();
}

void glmPolyline::add( const glm::vec3 & _point ){
    if(size()>0){
        polars.push_back( glmPolarPoint(cartesians[cartesians.size()-1],_point) );
        distances.push_back( distances[distances.size()-1] + polars[polars.size()-1].r );
    } else {
        distances.push_back(0.0);
    }
    cartesians.push_back(_point);
}

void glmPolyline::add(const std::vector<glm::vec3> & _points){
    for (int i = 0; i < _points.size(); i++) {
        add(_points[i]);
    }
}

void glmPolyline::addVertices(const std::vector<glm::vec3>& verts) {
    for (int i = 0 ; i < verts.size() ; i++) {
        add(verts[i]);
    }
}

//----------------------------------------------------------
void glmPolyline::addVertices(const glm::vec3* verts, int numverts) {
    for (int i = 0 ; i < numverts ; i++) {
        add(verts[i]);
    }
}

glm::vec3& glmPolyline::operator [](const int &_index){
    return cartesians[_index];
}

glm::vec3 glmPolyline::operator [](const int &_index) const {
    return cartesians[_index];
}

float glmPolyline::getAngleAt(const float &_dist) const{
    
    if(polars.size() == 0 || distances.size() == 0){
        return -1;
    } else if(_dist <= 0){
        return polars[0].a;
    }
    
    for (int i = 1; i < distances.size(); i++) {
        if(_dist<=distances[i]){
            return polars[i-1].a;
        }
    }
    
    return 0;
}

glm::vec3 glmPolyline::getPositionAt(const float &_dist) const{
    
    //  Todo fix this
    //
    
    if (size()==2) {
        return glm::vec3(_dist*cos(polars[0].a),
                         _dist*sin(polars[0].a),
                         0.0f);
    }
    
    for (int i = 1; i < distances.size(); i++) {
        if(_dist<=distances[i]){
            float diff = _dist-distances[i];
            return glm::vec3(cartesians[i].x + diff*cos(polars[i-1].a),
                             cartesians[i].y + diff*sin(polars[i-1].a),
                             0.0f);
        }
    }
    
    return cartesians[size()-1];
}

float glmPolyline::getFractAt(float _dist,float _offset)const{
    float a = getAngleAt(_dist-_offset);
    float b = getAngleAt(_dist+_offset);
    
    float diff= (a - b);
    if (diff < -PI) diff += PI*2.;
    if (diff > PI) diff -= PI*2.;
    
    return abs(diff)/PI;
}

//This is for polygon/contour simplification - we use it to reduce the number of points needed in
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
    if(cartesians.size() < 2) return;
    
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
    vt[0] = cartesians[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(cartesians[i], cartesians[pv]) < tol2) continue;
        
        vt[k++] = cartesians[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = cartesians[n-1];      // finish at the end
    
    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, &vt[0], 0, k-1, &mk[0] );
    
    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }
    
	//get rid of the unused points
	if( m < (int)sV.size() ){
		cartesians.assign( sV.begin(),sV.begin()+m );
	}else{
		cartesians = sV;
	}
    
    updateCache();
}

void glmPolyline::updateCache(){
    polars.clear();
    distances.clear();
    
    float total = 0;
    for (int i = 1; i < size(); i++) {
        glmPolarPoint p = glmPolarPoint(cartesians[i-1],cartesians[i]);
        polars.push_back(p);
        distances.push_back( total );
        total += p.r;
    }
    distances.push_back( total );
}

float glmPolyline::getLength(const int &_index) const {
    if(_index == -1){
        return distances[size()-1];
    } else if (_index >= size() || _index < 0){
        return 0;
    } else {
        return distances[_index];
    }
}

std::vector<glm::vec3> & glmPolyline::getVertices(){
	return cartesians;
}

void glmPolyline::draw(){
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < size(); i++) {
        glVertex2d(cartesians[i].x,cartesians[i].y);
    }
    glEnd();
}

void glmPolyline::drawPoints(){
    glPointSize(3);
    glBegin(GL_POINTS);
    for (int i = 0; i < size(); i++) {
        glVertex2d(cartesians[i].x,cartesians[i].y);
    }
    glEnd();
    glPointSize(1);
}

void glmPolyline::drawNormals(){
    for (int i = 0; i < size()-1; i++) {
        float angle = polars[i].a-HALF_PI;
        
        glm::vec3 head;
        head.x = polars[i].r * cos(angle);
        head.y = polars[i].r * sin(angle);
        head.z = 0.0f;
        drawLine(cartesians[i],cartesians[i]+head);
    }
}

//  http://artgrammer.blogspot.co.uk/2011/07/drawing-polylines-by-tessellation.html
//  https://www.mapbox.com/blog/drawing-antialiased-lines/
//
void glmPolyline::addAsLineToMesh(glmMesh &_mesh, float _width){

    //  From Matt code
    //
    glm::vec3 normi;             // Right normal to segment between previous and current points
    glm::vec3 normip1;           // Right normal to segment between current and next points
    glm::vec3 rightNorm;         // Right "normal" at current point, scaled for miter joint
    
    glm::vec3 im1;                   // Previous point coordinates
    glm::vec3 i0 = cartesians[0];    // Current point coordinates
    glm::vec3 ip1 = cartesians[1];   // Next point coordinates
    
    normip1.x = ip1.y - i0.y;
    normip1.y = i0.x - ip1.x;
    normip1.z = 0.;
    
    normip1 = glm::normalize(normip1);
    
    rightNorm = glm::vec3(normip1.x*_width,normip1.y*_width,normip1.z*_width);
    
    _mesh.addVertex(i0 + rightNorm);
    _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    _mesh.addTexCoord(glm::vec2(1.0,0.0));
    
    _mesh.addVertex(i0 - rightNorm);
    _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    _mesh.addTexCoord(glm::vec2(0.0,0.0));
    
    // Loop over intermediate points in the polyline
    //
    for (int i = 1; i < size() - 1; i++) {
        im1 = i0;
        i0 = ip1;
        ip1 = cartesians[i+1];
        
        normi = normip1;
        normip1.x = ip1.y - i0.y;
        normip1.y = i0.x - ip1.x;
        normip1.z = 0.0f;
        normip1 = glm::normalize(normip1);
        
        rightNorm = normi + normip1;
        float scale = sqrtf(2. / (1. + glm::dot(normi,normip1) )) * _width / 2.;
        rightNorm *= scale;
        
        _mesh.addVertex(i0+rightNorm);
        _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
        _mesh.addTexCoord(glm::vec2(1.0,(float)i/(float)size()));
        
        _mesh.addVertex(i0-rightNorm);
        _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
        _mesh.addTexCoord(glm::vec2(0.0,(float)i/(float)size()));
        
    }
    
    normip1 *= _width;
    
    _mesh.addVertex(ip1 + normip1);
    _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    _mesh.addTexCoord(glm::vec2(1.0,1.0));
    
    _mesh.addVertex(ip1 - normip1);
    _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
    _mesh.addTexCoord(glm::vec2(0.0,1.0));
    
    _mesh.setDrawMode(GL_TRIANGLE_STRIP);
}

void glmPolyline::addAsShapeToMesh(glmMesh &_mesh){
    TESStesselator *m_tess = tessNewTess(NULL);
    
    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();
    glmRectangle bBox;
    
    _mesh.setDrawMode(GL_TRIANGLES);
    
    for (int i = 0; i < cartesians.size(); i++) {
        // Add contour to tesselator
        tessAddContour(m_tess, 3, &cartesians[0].x, sizeof(glm::vec3), cartesians.size());
    }
    
    // Tessellate polygon into triangles
    tessTesselate(m_tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 3, NULL);
    
    // Extract triangle elements from tessellator
    
    const int numIndices = tessGetElementCount(m_tess);
    const TESSindex* indices = tessGetElements(m_tess);
    
    for (int i = 0; i < numIndices; i++) {
        const TESSindex* poly = &indices[i*3];
        for (int j = 0; j < 3; j++) {
            _mesh.addIndex(poly[j] + indexOffset);
        }
    }
    
    const int numVertices = tessGetVertexCount(m_tess);
    const float* vertices = tessGetVertices(m_tess);
    for (int i = 0; i < numVertices; i++) {
        
        _mesh.addTexCoord(glm::vec2(mapValue(vertices[3*i],bBox.getMinX(),bBox.getMaxX(),0.,1.),
                                    mapValue(vertices[3*i+1],bBox.getMinY(),bBox.getMaxY(),0.,1.)));
        _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
        _mesh.addVertex(glm::vec3(vertices[3*i], vertices[3*i + 1], vertices[3*i + 2]));
    }
    
    tessDeleteTess(m_tess);
}

glmRectangle glmPolyline::getBoundingBox() const {
	glmRectangle box;
    growToInclude(box);
	return box;
}

void glmPolyline::growToInclude(glmRectangle &_bbox) const {
    for(size_t i = 0; i < size(); i++) {
        if(i == 0) {
            _bbox.set(cartesians[i].x,cartesians[i].y,0.,0.);
        } else {
            _bbox.growToInclude(cartesians[i]);
        }
    }
}

glmPolyline glmPolyline::getUnProjected(){
    
    glm::ivec4 viewport;
    glm::mat4x4 mvmatrix, projmatrix;
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    glGetFloatv(GL_MODELVIEW_MATRIX, &mvmatrix[0][0]);
    glGetFloatv(GL_PROJECTION_MATRIX, &projmatrix[0][0]);
    
    glmPolyline unprojected;
    for (int i = 0; i < size(); i++) {
        unprojected.add(glm::project(cartesians[i], mvmatrix, projmatrix, viewport));
    }
	
    return unprojected;
    
}