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
    
    m_polars.clear();
    m_distances.clear();
}

void glmSmartLine::add( const glm::vec3 & _point ){
    if(size()>0){
        m_polars.push_back( glmPolarPoint(m_points[m_points.size()-1],_point) );
        m_distances.push_back( m_distances[m_distances.size()-1] + m_polars[m_polars.size()-1].r );
    } else {
        m_distances.push_back(0.0);
    }
    glmPolyline::add(_point);
}

const std::vector<glmPolarPoint> & glmSmartLine::getPolars() const{
    return m_polars;
}

const std::vector<float> & glmSmartLine::getDistances() const{
    return m_distances;
}

float glmSmartLine::getAngleAt(const float &_dist) const{
    
    if(m_polars.size() == 0 || m_distances.size() == 0){
        return -1;
    } else if(_dist <= 0){
        return m_polars[0].a;
    }
    
    for (int i = 1; i < m_distances.size(); i++) {
        if(_dist<=m_distances[i]){
            return m_polars[i-1].a;
        }
    }
    
    return 0;
}

glm::vec3 glmSmartLine::getPositionAt(const float &_dist) const{

    if (size()==2) {
        return m_points[0] + glmPolarPoint(m_polars[0].a,_dist).getXY();
    }
    
    for (int i = 1; i < m_distances.size(); i++) {
        if(_dist<=m_distances[i]){
            return m_points[i] + glmPolarPoint(m_polars[i-1].a,_dist-m_distances[i]).getXY();
        }
    }
    
    return m_points[size()-1];
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
    m_polars.clear();
    m_distances.clear();
    
    float total = 0;
    for (int i = 1; i < size(); i++) {
        glmPolarPoint p = glmPolarPoint(m_points[i-1],m_points[i]);
        m_polars.push_back(p);
        m_distances.push_back( total );
        total += p.r;
    }
    m_distances.push_back( total );
}

float glmSmartLine::getLength(const int &_index) const {
    
    if(_index >= size() || size() == 0){
        return -1;
    } else if(_index == -1){
        return m_distances[size()-1];
    } else if ( _index < 0){
        return 0;
    } else {
        return m_distances[_index];
    }
}

void glmSmartLine::drawNormals(){
    for (int i = 0; i < size()-1; i++) {
        drawLine(m_points[i], m_points[i] + glmPolarPoint(m_polars[i].a-HALF_PI,m_polars[i].r).getXY());
    }
}