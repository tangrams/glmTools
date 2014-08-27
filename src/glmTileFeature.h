//
//  glmTileFeature.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/27/14.
//
//

#pragma once

#include <memory>
#include "glmPolyline.h"
#include "glmMesh.h"

class glmTileFeature {
public:
    
    glmTileFeature(){};
    virtual ~glmTileFeature(){};
    
    glmMesh     geometry;
    glmPolyline polyline;
    string      idString;
    string      name;
    
    void draw(){
        geometry.draw();
    }
    
private:
    
};

typedef auto_ptr<glmTileFeature> glmTileFeatureRef;