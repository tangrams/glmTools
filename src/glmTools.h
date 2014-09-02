//
//  Header.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/25/14.
//
//

#pragma once

#include <vector>
#include <iostream>

#include <OpenGL/gl.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#ifndef PI
#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
#define TWO_PI   6.28318530717958647693
#endif

#ifndef M_TWO_PI
#define M_TWO_PI   6.28318530717958647693
#endif

#ifndef FOUR_PI
#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
#define HALF_PI  1.57079632679489661923
#endif

#ifndef R_EARTH
#define R_EARTH 6378137
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))
#endif

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

double y2lat(double y) { return glm::degrees(2. * atan(exp(glm::radians(y))) - PI / 2); }
double x2lon(double x) { return glm::degrees(x / R_EARTH); }
double lat2y(double lat) { return R_EARTH * log(tan(PI / 4 + glm::radians(lat) / 2)); }
double lon2x(double lon) { return glm::radians(lon) * R_EARTH; }

glm::vec3 worldToScreen(const glm::vec3 &_worldPos){
    glm::ivec4 viewport;
    glm::mat4x4 mvmatrix, projmatrix;
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    glGetFloatv(GL_MODELVIEW_MATRIX, &mvmatrix[0][0]);
    glGetFloatv(GL_PROJECTION_MATRIX, &projmatrix[0][0]);
    
    return glm::project(_worldPos, mvmatrix, projmatrix, viewport);
}

glm::vec3 screenToWorld(const glm::vec3 &_screenPos){
    glm::ivec4 viewport;
    glm::mat4x4 mvmatrix, projmatrix;
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    glGetFloatv(GL_MODELVIEW_MATRIX, &mvmatrix[0][0]);
    glGetFloatv(GL_PROJECTION_MATRIX, &projmatrix[0][0]);
    
    return glm::unProject(_screenPos, mvmatrix, projmatrix, viewport);
}


float mapValue(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false) {
	if (fabs(inputMin - inputMax) < FLT_EPSILON){
		return outputMin;
	} else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
        
		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
	}
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z;
	return os;
}

inline std::istream& operator>>(std::istream& is, glm::vec3& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	is.ignore(2);
	is >> vec.z;
	return is;
}

glm::vec3 getCentroid(std::vector<glm::vec3> &_pts){
    glm::vec3 centroid;
    for (int i = 0; i < _pts.size(); i++) {
        centroid += _pts[i] / (float)_pts.size();
    }
    return centroid;
}

void drawCross(const glm::vec3 &_pos, const float &_width = 3.5){
    glm::vec3 linePoints[4] = {_pos, _pos, _pos, _pos};
    linePoints[0].x -= _width;
    linePoints[1].x += _width;
    linePoints[2].y -= _width;
    linePoints[3].y += _width;
    
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(glm::vec3), &linePoints[0].x);
	glDrawArrays(GL_LINES, 0, 4);
}

void drawLine(const glm::vec3 &_a, const glm::vec3 &_b){
    glm::vec3 linePoints[2];
    linePoints[0] = _a;
    linePoints[1] = _b;
    
    glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(glm::vec3), &linePoints[0].x);
	glDrawArrays(GL_LINES, 0, 2);
};

bool lineSegmentIntersection(const glm::vec3 &_line1Start, const glm::vec3 &_line1End,
                             const glm::vec3 &_line2Start, const glm::vec3 &_line2End,
                             glm::vec3 &_intersection ){
	glm::vec3 diffLA, diffLB;
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