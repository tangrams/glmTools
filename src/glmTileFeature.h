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
    std::string      idString;
    std::string      name;
    
private:
    
};

typedef std::auto_ptr<glmTileFeature> glmTileFeatureRef;