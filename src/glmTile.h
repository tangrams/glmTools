//
//  glmTile.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/27/14.
//
//

#pragma once

#include "json/json.h"

#include "glmTools.h"
#include "glmTileFeature.h"
#include "glmPolyline.h"

#include "tesselator.h"
#include "ftgl.h"

class glmTile {
public:
    
    glmTile();
    glmTile(std::string fileName);
    
    void setFont(FTFont *_font);
    void setGeometryOffset(glm::vec3 _offset);
    
    bool load(std::string _fileName);
    bool load(int _tileX, int _tileY, int _zoom);
    void unload();
    
    void build(std::string layerName, float _layerHeight = 0.);
    
    std::map< std::string, std::vector<glmTileFeatureRef> > layers;
    
    std::vector<glmLabeledFeatureRef> labelFeatures;
    
private:

    void buildLayerGeometry(std::string layerName, std::vector<glmTileFeatureRef> &_mesh, float minHeight = 0.);
    
    void lineJson2Mesh(Json::Value &lineJson, glmMesh &_mesh, float minHeight);
    void lineJson2Polyline(Json::Value &lineJson, glmPolyline &_poly, float minHeight);
    void polygonJson2Mesh(Json::Value &polygonJson, glmMesh &_mesh, float minHeight, float height);
    
    Json::Value m_jsonRoot;
    glm::vec3   m_geometryOffset;
    
    FTFont          *font;
    
    float   lineWidth;
    int tileX, tileY, zoom;
};