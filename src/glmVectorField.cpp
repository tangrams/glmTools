//
//  glmVectorField.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/25/14.
//
//

#include "glmVectorField.h"

glmVectorField::glmVectorField():m_forces(NULL),m_width(0), m_height(0), m_cols(0), m_rows(0), m_resolution(10){
    
}

glmVectorField::~glmVectorField(){
    deallocate();
}

void glmVectorField::deallocate(){
    
    if(m_forces != NULL){
        for(int i = 0; i < m_rows; i++){
            delete [] m_forces[i];
        }
        
        delete [] m_forces;  //delete the array of pointers
        m_forces = NULL;
    }
}

void glmVectorField::set( int _width, int _height, int _res ){
    
    if(m_width != _width || m_height != _height || m_resolution != _res){

        deallocate();
        
        m_width = _width;
        m_height = _height;
        m_resolution = _res;
        
        m_cols = m_width / m_resolution + 1;
        m_rows = m_height / m_resolution + 1;
        
        //  Allocation
        //
        m_forces = new glm::vec3*[m_cols];
        for(int i = 0; i < m_cols; i++){
            m_forces[i] = new glm::vec3[m_rows];
        }
        
        // Initialization
        //
        for(int j = 0; j < m_cols; j++){
            for(int i = 0; i < m_rows; i++){
                m_forces[j][i] = glm::vec3(0.,0.,0.);
            }
        }
    }
}

glm::vec3 &	glmVectorField::getForceAt(const glm::vec3 &_pos){
    
    if(m_width > 0 && m_height > 0 ){
        float pctX = _pos.x / m_width;
        float pctY = _pos.y / m_height;
        
        int cols = m_width / m_resolution;
        int rows = m_height / m_resolution;
        
        int xVal = CLAMP(pctX*m_cols, 0, m_cols-1);
        int yVal = CLAMP(pctY*m_rows, 0, m_rows-1);
        
        return m_forces[xVal][yVal];
    } else {
        glm::vec3 none = glm::vec3(0.,0.,0.);
        return none;
    }
}

void glmVectorField::draw(){
    for( int x = 0; x < m_cols; x++){
        for( int y = 0; y < m_rows; y++){
            drawCross( glm::vec3( x*m_resolution, y*m_resolution, 0.0), 5 );
        }
    }
}