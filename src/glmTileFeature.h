//
//  glmTileFeature.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/27/14.
//
//

#pragma once

//  TODO: use default one
//
//#include <memory>
#include <tr1/memory>

#include "glmPolyline.h"
#include "glmMesh.h"
#include "glmText.h"

class glmTileFeature {
public:

    glmTileFeature(){};
    virtual ~glmTileFeature(){};
    
    glmMesh         geometry;
    glmPolyline     polyline;
    glmText         text;
    
    std::string     idString;
    
private:
    
};

typedef std::tr1::shared_ptr<glmTileFeature> glmTileFeatureRef;