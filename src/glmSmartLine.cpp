//
//  glmSmartLine.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 9/8/14.
//
//

#include "glmSmartLine.h"

glmSmartLine::glmSmartLine(){
};

glmSmartLine::~glmSmartLine(){
};

void glmSmartLine::clear(){
    glmPolyline::clear();
    
    polars.clear();
    distances.clear();
}

void glmSmartLine::add( const glm::vec3 & _point ){
    if(size()>0){
        polars.push_back( glmPolarPoint(points[points.size()-1],_point) );
        distances.push_back( distances[distances.size()-1] + polars[polars.size()-1].r );
    } else {
        distances.push_back(0.0);
    }
    glmPolyline::add(_point);
}

float glmSmartLine::getAngleAt(const float &_dist) const{
    
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

glm::vec3 glmSmartLine::getPositionAt(const float &_dist) const{

    if (size()==2) {
        return points[0] + glm::vec3(_dist*cos(polars[0].a),
                                         _dist*sin(polars[0].a),
                                         0.0f);
        
    }
    
    for (int i = 1; i < distances.size(); i++) {
        if(_dist<=distances[i]){
            float diff = _dist-distances[i];
            return glm::vec3(points[i].x + diff*cos(polars[i-1].a),
                             points[i].y + diff*sin(polars[i-1].a),
                             0.0f);
        }
    }
    
    return points[size()-1];
}

float glmSmartLine::getFractAt(const float &_dist, const float &_offset) const{
    float a = getAngleAt(_dist-_offset);
    float b = getAngleAt(_dist+_offset);
    
    float diff= (a - b);
    if (diff < -PI) diff += PI*2.;
    if (diff > PI) diff -= PI*2.;
    
    return abs(diff)/PI;
}

void glmSmartLine::updateCache(){
    polars.clear();
    distances.clear();
    
    float total = 0;
    for (int i = 1; i < size(); i++) {
        glmPolarPoint p = glmPolarPoint(points[i-1],points[i]);
        polars.push_back(p);
        distances.push_back( total );
        total += p.r;
    }
    distances.push_back( total );
}

float glmSmartLine::getLength(const int &_index) const {
    
    if(_index >= size() || size() == 0){
        return -1;
    } else if(_index == -1){
        return distances[size()-1];
    } else if ( _index < 0){
        return 0;
    } else {
        return distances[_index];
    }
}

void glmSmartLine::drawNormals(){
    for (int i = 0; i < size()-1; i++) {
        float angle = polars[i].a-HALF_PI;
        
        glm::vec3 head;
        head.x = polars[i].r * cos(angle);
        head.y = polars[i].r * sin(angle);
        head.z = 0.0f;
        drawLine(points[i],points[i]+head);
    }
}