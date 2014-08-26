//
//  glmHelpers.h
//
//  Created by Patricio Gonzalez Vivo on 8/21/14.
//
//

#pragma once

#include "glmTools.h"

float y2lat(float y) { return RAD_TO_DEG*(2. * atan(exp(DEG_TO_RAD*y)) - PI / 2); }
float x2lon(float x) { return RAD_TO_DEG*(x / R_EARTH); }
float lat2y(float lat) { return R_EARTH * log(tan(PI / 4 + (DEG_TO_RAD*lat) / 2)); }
float lon2x(float lon) { return (DEG_TO_RAD*lon) * R_EARTH; }

vec3 getCentroid(vector<vec3> &_pts){
    vec3 centroid;
    for (int i = 0; i < _pts.size(); i++) {
        centroid += _pts[i] / (float)_pts.size();
    }
    return centroid;
}

void drawLine(const vec3 &_a, const vec3 &_b){
    vec3 linePoints[2];
    linePoints[0] = _a;
    linePoints[1] = _b;
    
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(vec3), &linePoints[0].x);
	glDrawArrays(GL_LINES, 0, 2);
};

bool lineSegmentIntersection(const vec3 &_line1Start, const vec3 &_line1End,
                             const vec3 &_line2Start, const vec3 &_line2End,
                             vec3 &_intersection ){
	vec3 diffLA, diffLB;
	double compareA, compareB;
	diffLA = _line1End - _line1Start;
	diffLB = _line2End - _line2Start;
	compareA = diffLA.x*_line1Start.y - diffLA.y*_line1Start.x;
	compareB = diffLB.x*_line2Start.y - diffLB.y*_line2Start.x;
	if (
		(
         ( ( diffLA.x*_line2Start.y - diffLA.y*_line2Start.x ) < compareA ) ^
         ( ( diffLA.x*_line2End.y - diffLA.y*_line2End.x ) < compareA )
         )
		&&
		(
         ( ( diffLB.x*_line1Start.y - diffLB.y*_line1Start.x ) < compareB ) ^
         ( ( diffLB.x*_line1End.y - diffLB.y*_line1End.x) < compareB )
         )
        )
	{
		double lDetDivInv = 1 / ((diffLA.x*diffLB.y) - (diffLA.y*diffLB.x));
		_intersection.x =  -((diffLA.x*compareB) - (compareA*diffLB.x)) * lDetDivInv ;
		_intersection.y =  -((diffLA.y*compareB) - (compareA*diffLB.y)) * lDetDivInv ;
        
		return true;
	}
    
	return false;
};