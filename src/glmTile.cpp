//
//  glmTile.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/27/14.
//
//

#include "glmTile.h"

#include <fstream>
#include <utility>

glmTile::glmTile() {
    m_geometryOffset = glm::vec3(0,0,0);
    font = NULL;
    
    lineWidth = 5.5;
}

glmTile::glmTile(std::string fileName) {
    glmTile();
    load(fileName);
}

void glmTile::setFont(FTFont *_font){
    font = _font;
}

bool glmTile::load(int _tileX, int _tileY, int _zoom){
    tileX = _tileX;
    tileY = _tileY;
    zoom = _zoom;
    
    float n = powf(2.0f, zoom);
    m_geometryOffset.x = lon2x((tileX + 0.5) / n * 360.0f - 180.0f);
    m_geometryOffset.y = lat2y(atanf(sinhf(PI*(1-2*(tileY+0.5)/n))) * 180.0f / PI);
    
    //  TODO: get JSON file from the web
    //
}

bool glmTile::load(std::string _filename){
    std::ifstream inputStream(_filename.c_str(),std::ifstream::in);
    if(inputStream.bad()){
		return false;
	}
    
    Json::Reader jsonReader;
    bool rta = jsonReader.parse(inputStream, m_jsonRoot);
    
    build("buildings");
    build("earth");
    build("landuse",1.0);
    build("water");
    build("roads",3.0);
    build("places",2.0);
    build("pois",4.0);
    
    return rta;
}

void glmTile::unload() {
    m_jsonRoot.clear();
}

void glmTile::build(std::string _layerName, float _layerHeight ){
    buildLayerGeometry( _layerName, layers[_layerName], _layerHeight );
}

void glmTile::setGeometryOffset(glm::vec3 _offset){
    m_geometryOffset = _offset;
}

void glmTile::buildLayerGeometry(std::string layerName, std::vector<glmTileFeatureRef> &_features, float _minHeight) {
    
    Json::Value featureListJson = m_jsonRoot[layerName.c_str()]["features"];
    
    std::vector<glm::vec3> verts;
    std::vector<uint16_t> inds;
    
    for (int i = 0; i < featureListJson.size(); i++) {
        
        Json::Value geometryJson = featureListJson[i]["geometry"];
        Json::Value propsJson = featureListJson[i]["properties"];
        
        std::string geometryType = geometryJson["type"].asString();
        
        float height = _minHeight;
        if (propsJson.isMember("height")) {
            height += propsJson["height"].asFloat();
        }
        
        float minHeight = _minHeight;
        if (propsJson.isMember("min_height")) {
            minHeight += propsJson["min_height"].asFloat();
        }
        
        if (geometryType.compare("Point") == 0) {
            
            // TODO
            std::cout << "TODO: Point" << std::endl;
            
        } else if (geometryType.compare("MultiPoint") == 0) {
            
            // TODO
            std::cout << "TODO: MultiPoint" << std::endl;
            
        } else if (geometryType.compare("LineString") == 0) {
            
            if (propsJson.isMember("name")) {
                
                glmLabeledFeatureRef labelRef(new glmLabeledFeature);
                
                lineJson2Polyline(geometryJson["coordinates"],labelRef->polyline,minHeight);

                labelRef->centroid = labelRef->polyline.getBoundingBox().getCenter();
                labelRef->text.set(font, propsJson["name"].asString());
                labelRef->type = LABEL_LINE;
                
                labelFeatures.push_back(labelRef);
                
                labelRef->polyline.addAsLineToMesh(labelRef->geometry,lineWidth);
                _features.push_back(labelRef);
                
            } else {
                glmTileFeatureRef feature(new glmLabeledFeature);
                lineJson2Mesh(geometryJson["coordinates"], feature->geometry, minHeight);
                _features.push_back(feature);
            }
            
        } else if (geometryType.compare("MultiLineString") == 0) {
            
            for (int j = 0; j < geometryJson["coordinates"].size(); j++) {
                glmTileFeatureRef feature(new glmTileFeature);
                lineJson2Mesh(geometryJson["coordinates"][j], feature->geometry, minHeight);
                _features.push_back(feature);
            }
            
        } else if (geometryType.compare("Polygon") == 0) {
            
            if (propsJson.isMember("name")) {
                glmLabeledFeatureRef labelRef(new glmLabeledFeature);
                
                lineJson2Polyline(geometryJson["coordinates"][0],labelRef->polyline, height);
            
                labelRef->centroid = labelRef->polyline.getBoundingBox().getCenter() + glm::vec3(0,0,height);
                labelRef->text.set(font, propsJson["name"].asString());
                labelRef->type = LABEL_AREA;
                
                labelFeatures.push_back(labelRef);
                
                polygonJson2Mesh(geometryJson["coordinates"], labelRef->geometry, minHeight, height);
                _features.push_back(labelRef);
            } else {
                glmTileFeatureRef feature(new glmTileFeature);
                polygonJson2Mesh(geometryJson["coordinates"], feature->geometry, minHeight, height);
                _features.push_back(feature);
            }
            
        } else if (geometryType.compare("MultiPolygon") == 0) {
            
            for (int j = 0; j < geometryJson["coordinates"].size(); j++) {
                glmTileFeatureRef feature(new glmTileFeature);
                polygonJson2Mesh(geometryJson["coordinates"][j], feature->geometry, minHeight, height);
                _features.push_back(feature);
            }
            
        } else if (geometryType.compare("GeometryCollection") == 0) {
            
            // TODO
            std::cout << "TODO: GeometryCollection" << std::endl;
        }
        
    }
}

void glmTile::polygonJson2Mesh(Json::Value &polygonJson, glmMesh &_mesh, float _minHeight, float _height) {
    TESStesselator  *m_tess = tessNewTess(NULL);                    // Tesselator instance
    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();    // track indices
    _mesh.setDrawMode(GL_TRIANGLES);
    
    glmRectangle bBox;  // will calculate the total bounding box to compute a UV for top the top view
    
    //  Go through the Json polygons making walls and adding it to the tessalator
    //
    for (int i = 0; i < polygonJson.size(); i++) {
    
        glmPolyline ringCoords;
        lineJson2Polyline(polygonJson[i],ringCoords,_height);
        
        // Extrude polygon based on height
        if (_height != _minHeight){
            
            glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
            glm::vec3 tan, nor;
            
            for (int i = 0; i < ringCoords.size() - 1; i++) {
                
                //For each vertex in the polygon, make two triangles to form a quad
                //
                glm::vec3 ip0 = ringCoords[i];
                glm::vec3 ip1 = ringCoords[i+1];
                
                tan = ringCoords[i+1] - ringCoords[i];
                nor = glm::cross(up, tan);
                
                _mesh.addTexCoord(glm::vec2(1.,0.));
                _mesh.addVertex(ip0);
                _mesh.addNormal(nor);
                _mesh.addTexCoord(glm::vec2(0.,0.));
                _mesh.addVertex(ip1);
                _mesh.addNormal(nor);
                
                _mesh.addTexCoord(glm::vec2(1.,1.));
                _mesh.addVertex(ip0+glm::vec3(0.,0.,_minHeight - _height - m_geometryOffset.z));
                _mesh.addNormal(nor);
                _mesh.addTexCoord(glm::vec2(0.,1.));
                _mesh.addVertex(ip1+glm::vec3(0.,0.,_minHeight - _height - m_geometryOffset.z));
                _mesh.addNormal(nor);
                
                _mesh.addIndex(indexOffset);
                _mesh.addIndex(indexOffset + 2);
                _mesh.addIndex(indexOffset + 1);
                
                _mesh.addIndex(indexOffset + 1);
                _mesh.addIndex(indexOffset + 2);
                _mesh.addIndex(indexOffset + 3);
                indexOffset += 4;
            }
        }
        
        //  Grow  bounding box
        ringCoords.addToBoundingBox(bBox);
        
        //  Add to tesselator
        tessAddContour(m_tess, 3, &ringCoords[0].x, sizeof(glm::vec3), ringCoords.size());
    }
    
    // Tessellate polygon into triangles
    tessTesselate(m_tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 3, NULL);
    
    // Extract triangle elements from tessellator
    const int numIndices = tessGetElementCount(m_tess);
    const TESSindex* indices = tessGetElements(m_tess);
    for (int i = 0; i < numIndices; i++) {
        const TESSindex* poly = &indices[i*3];
        for (int j = 0; j < 3; j++) {
            _mesh.addIndex(poly[j] + indexOffset);
        }
    }
    
    //  Add vertexes from tessellator
    //
    const int numVertices = tessGetVertexCount(m_tess);
    const float* vertices = tessGetVertices(m_tess);
    for (int i = 0; i < numVertices; i++) {
        _mesh.addTexCoord(glm::vec2(mapValue(vertices[3*i],bBox.getMinX(),bBox.getMaxX(),0.,1.),
                                    mapValue(vertices[3*i+1],bBox.getMinY(),bBox.getMaxY(),0.,1.)));
        _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
        _mesh.addVertex(glm::vec3(vertices[3*i], vertices[3*i + 1], vertices[3*i + 2]));
    }
    
    tessDeleteTess(m_tess);
}

void glmTile::lineJson2Mesh(Json::Value &lineJson, glmMesh &_mesh, float _minHeight){
    glmPolyline polyline;
    lineJson2Polyline(lineJson,polyline,_minHeight);
    polyline.addAsLineToMesh(_mesh, lineWidth);
}

void glmTile::lineJson2Polyline(Json::Value &lineJson, glmPolyline &_poly, float _minHeight){
    for (int i = 0; i < lineJson.size(); i++) {
        _poly.add(glm::vec3(lon2x(lineJson[i][0].asFloat()),
                            lat2y(lineJson[i][1].asFloat()),
                            _minHeight) - m_geometryOffset);
    }
}