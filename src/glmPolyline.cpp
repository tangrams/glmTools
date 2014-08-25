//
//  glmPolyline.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/20/14.
//
//

#include "glmPolyline.h"

#include <OpenGL/gl.h>
#include "glmHelpers.h"

int glmPolyline::size() const {
    return cartesians.size();
}

void glmPolyline::clear(){
    cartesians.clear();
    polars.clear();
    
    distances.clear();
}

void glmPolyline::add( const glm::vec2 & _point ){
    if(size()>0){
        polars.push_back(glmPolarPoint(cartesians[cartesians.size()-1],_point));
        distances.push_back( distances[distances.size()-1] + polars[polars.size()-1].r );
    } else {
        distances.push_back(0.0);
    }
    cartesians.push_back(_point);
}

void glmPolyline::add(const std::vector<glm::vec2> & _points){
    for (int i = 0; i < _points.size(); i++) {
        add(_points[i]);
    }
}

glm::vec2& glmPolyline::operator [](const int &_index){
    return cartesians[_index];
}

double glmPolyline::getAngleAt(const double &_dist) const{
    
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

glm::vec2 glmPolyline::getPositionAt(const double &_dist) const{
    
    if (size()==2) {
        return glm::vec2(_dist*cos(polars[0].a),
                         _dist*sin(polars[0].a));
    }
    
    for (int i = 1; i < distances.size(); i++) {
        if(_dist<=distances[i]){
            double diff = _dist-distances[i];
            return glm::vec2(cartesians[i].x + diff*cos(polars[i-1].a),
                             cartesians[i].y + diff*sin(polars[i-1].a));
        }
    }
    
    return cartesians[size()-1];
}

double glmPolyline::getFractAt(double _dist,double _offset)const{
    double a = getAngleAt(_dist-_offset);
    double b = getAngleAt(_dist+_offset);
    
    double diff= (a - b);
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
    vec2 P0, P1;
} Segment;

#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

//--------------------------------------------------
static void simplifyDP(float tol, glm::vec2* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;
    
    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi	= j;          // index of vertex farthest from S
    float   maxd2	= 0;         // distance squared of farthest vertex
    float   tol2	= tol * tol;  // tolerance squared
    Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
    vec2 u;
	u				= S.P1 - S.P0;   // segment direction vector
    double  cu		= dot(u,u);     // segment length squared
    
    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    glm::vec2  w;
    glm::vec2   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared
    
    for (int i=j+1; i<k; i++){
        // compute distance squared
        w = v[i] - S.P0;
        cw = dot(w,u);
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

void glmPolyline::simplify(double tol){
    if(cartesians.size() < 2) return;
    
	int n = size();
	
	if(n == 0) {
		return;
	}
    
    vector<vec2> sV;
	sV.resize(n);
    
    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    vector<vec2> vt;
    vector<int> mk;
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
    
    double total = 0;
    for (int i = 1; i < size(); i++) {
        glmPolarPoint p = glmPolarPoint(cartesians[i-1],cartesians[i]);
        polars.push_back(p);
        distances.push_back( total );
        total += p.r;
    }
    distances.push_back( total );
}

double glmPolyline::getLength(const int &_index) const {
    if(_index == -1){
        return distances[size()-1];
    } else if (_index >= size() || _index < 0){
        return 0;
    } else {
        return distances[_index];
    }
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
        double angle = polars[i].a-HALF_PI;
        
        glm::vec2 head;
        head.x = polars[i].r * cos(angle);
        head.y = polars[i].r * sin(angle);
        drawLine(cartesians[i],cartesians[i]+head);
    }
}

//  http://artgrammer.blogspot.co.uk/2011/07/drawing-polylines-by-tessellation.html
//  https://www.mapbox.com/blog/drawing-antialiased-lines/
//
glmMesh glmPolyline::getMesh(double _width){
    glmMesh mesh;
    
    //  From Matt code
    //
    float xim1, yim1, zim1; // Previous point coordinates
    float xi, yi, zi;       // Current point coordinates
    float xip1, yip1, zip1; // Next point coordinates
    
    vec3 normi;             // Right normal to segment between previous and current points
    vec3 normip1;           // Right normal to segment between current and next points
    vec3 rightNorm;         // Right "normal" at current point, scaled for miter joint
    
    xi = cartesians[0].x;
    yi = cartesians[0].y;
    zi = 0.;
    
    xip1 = cartesians[1].x;
    yip1 = cartesians[1].y;
    zip1 = 0.;
    
    normip1.x = yip1 - yi;
    normip1.y = xi - xip1;
    normip1.z = 0.;
    normip1 = normalize(normip1);
    
    rightNorm = vec3(normip1.x*_width,normip1.y*_width,normip1.z*_width);
    
    vec3 startRight = vec3(xi + rightNorm.x, yi + rightNorm.y, 0.);
    vec3 startLeft = vec3(xi - rightNorm.x, yi - rightNorm.y, 0.);
    
    mesh.addVertex(startRight);
    mesh.addVertex(startLeft);
    
    // Loop over intermediate points in the polyline
    //
    for (int i = 1; i < size() - 1; i++) {
        
        xim1 = xi;
        yim1 = yi;
        zim1 = zi;
        
        xi = xip1;
        yi = yip1;
        zi = zip1;
        
        xip1 = cartesians[i+1].x;
        yip1 = cartesians[i+1].y;
        zip1 = 0.;
        
        normi = normip1;
        
        normip1.x = yip1 - yi;
        normip1.y = xi - xip1;
        normip1.z = 0.0f;
        normip1 = normalize(normip1);

        rightNorm = normi + normip1;
        float scale = sqrtf(2. / (1. + dot(normi,normip1) )) * _width / 2.;
        rightNorm *= scale;
        
        vec3 vertRight = vec3(xi + rightNorm.x, yi + rightNorm.y, 0.0f);
        vec3 vertLeft = vec3(xi - rightNorm.x, yi - rightNorm.y, 0.0f);
        
        mesh.addVertex(vertRight);
        mesh.addVertex(vertLeft);
        
    }
    
    // Last segment creates a square "end" to the polyline
    vec3 endRight = vec3(xip1 + normip1.x*_width, yip1 + normip1.y*_width , 0.0f);
    vec3 endLeft = vec3(xip1 - normip1.x*_width, yip1 - normip1.y*_width, 0.0f);
    mesh.addVertex(endRight);
    mesh.addVertex(endLeft);
    
    return mesh;
}