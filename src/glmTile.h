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
#include "glmMesh.h"
#include "glmPolyline.h"

#include "tesselator.h"

class glmTile {
public:
    
    glmTile();
    glmTile(string fileName);
    
    void setGeometryOffset(vec3 _offset);
    
    bool load(string _fileName);
    bool load(int _tileX, int _tileY, int _zoom);
    void unload();
    
    int tileX, tileY, zoom;
    
    vector<glmMesh> buildings;
    vector<glmMesh> roads;
    vector<glmMesh> earth;
    vector<glmMesh> water;
    
    vector<glmMesh> pois;
    vector<glmMesh> places;

private:
    vector<glmMesh> getLayerGeometry(string layerName);
    void buildLayerGeometry(string layerName, vector<glmMesh> &_mesh, float minHeight = 0.);
    void lineJson2Mesh(Json::Value &lineJson, glmMesh &_mesh, float minHeight);
    void polygonJson2Mesh(Json::Value &polygonJson, glmMesh &_mesh, float minHeight, float height);
    
    TESStesselator *m_tess;
    Json::Value m_jsonRoot;
    
    glm::vec3   m_geometryOffset;
};