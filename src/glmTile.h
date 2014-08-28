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

class glmTile {
public:
    
    glmTile();
    glmTile(std::string fileName);
    
    void setGeometryOffset(glm::vec3 _offset);
    
    bool load(std::string _fileName);
    bool load(int _tileX, int _tileY, int _zoom);
    void unload();
    
    void draw();
    
    int tileX, tileY, zoom;
    
    std::vector<glmTileFeature> buildings;
    std::vector<glmTileFeature> roads;
    std::vector<glmTileFeature> earth;
    std::vector<glmTileFeature> water;
    
    std::vector<glmTileFeature> pois;
    std::vector<glmTileFeature> places;

private:

    void buildLayerGeometry(std::string layerName, std::vector<glmTileFeature> &_mesh, float minHeight = 0.);
    void lineJson2Mesh(Json::Value &lineJson, glmMesh &_mesh, float minHeight);
    void polygonJson2Mesh(Json::Value &polygonJson, glmMesh &_mesh, float minHeight, float height);
    
    TESStesselator *m_tess;
    Json::Value m_jsonRoot;
    
    glm::vec3   m_geometryOffset;
};