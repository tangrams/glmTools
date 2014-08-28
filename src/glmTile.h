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
    
    void draw();

    std::vector<glmTileFeatureRef> buildings;
    std::vector<glmTileFeatureRef> roads;
    std::vector<glmTileFeatureRef> earth;
    std::vector<glmTileFeatureRef> water;
    std::vector<glmTileFeatureRef> pois;
    std::vector<glmTileFeatureRef> places;

private:

    void buildLayerGeometry(std::string layerName, std::vector<glmTileFeatureRef> &_mesh, float minHeight = 0.);
    void lineJson2Mesh(Json::Value &lineJson, glmMesh &_mesh, float minHeight);
    void polygonJson2Mesh(Json::Value &polygonJson, glmMesh &_mesh, float minHeight, float height);
    
    Json::Value m_jsonRoot;
    glm::vec3   m_geometryOffset;
    
    TESStesselator  *m_tess;
    FTFont          *font;
    
    int tileX, tileY, zoom;
};