//
//  glmTileFeature.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 9/3/14.
//
//

#include "glmTileFeature.h"

void glmLabeledFeature::updateProjection(){
    if (type == LABEL_AREA ){
        projectedPolyline = polyline.getProjected();
        
        glm::ivec4 viewport;
        glm::mat4x4 mvmatrix, projmatrix;
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        glGetFloatv(GL_MODELVIEW_MATRIX, &mvmatrix[0][0]);
        glGetFloatv(GL_PROJECTION_MATRIX, &projmatrix[0][0]);
        
        projectedCentroid = glm::project(centroid, mvmatrix, projmatrix, viewport);
    } else if (type == LABEL_LINE){
        projectedPolyline = polyline.getProjected();
    } else if ( type == LABEL_POINT){
        glm::ivec4 viewport;
        glm::mat4x4 mvmatrix, projmatrix;
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        glGetFloatv(GL_MODELVIEW_MATRIX, &mvmatrix[0][0]);
        glGetFloatv(GL_PROJECTION_MATRIX, &projmatrix[0][0]);
        
        projectedCentroid = glm::project(centroid, mvmatrix, projmatrix, viewport);
    }
}

void glmLabeledFeature::draw(){
    if (type == LABEL_AREA ){
        //  TODO
        //
    } else if (type == LABEL_LINE){
        text.drawOnLine(projectedPolyline,0.5,0.5);
    } else if ( type == LABEL_POINT){
        //  TODO
        //
    }
}