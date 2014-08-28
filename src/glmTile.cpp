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
	m_tess = tessNewTess( NULL );
    m_geometryOffset = glm::vec3(0,0,0);
}

glmTile::glmTile(std::string fileName) {
    glmTile();
    load(fileName);
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
    
    buildings.clear();
    buildLayerGeometry("buildings", buildings);
    
    earth.clear();
    buildLayerGeometry("earth", earth);
    
    water.clear();
    buildLayerGeometry("water", water);
    
    roads.clear();
    buildLayerGeometry("roads", roads, 1.0);
    
    places.clear();
    buildLayerGeometry("places", places, 2.0);
    
    pois.clear();
    buildLayerGeometry("pois", pois, 3.0);
    
    return rta;
}

void glmTile::unload() {
    m_jsonRoot.clear();
}

void glmTile::setGeometryOffset(glm::vec3 _offset){
    m_geometryOffset = _offset;
}

void glmTile::buildLayerGeometry(std::string layerName, std::vector<glmTileFeature> &_features, float _minHeight) {
    
    Json::Value featureListJson = m_jsonRoot[layerName.c_str()]["features"];
    
    std::vector<glm::vec3> verts;
    std::vector<uint16_t> inds;
    
    for (int i = 0; i < featureListJson.size(); i++) {
        
        Json::Value geometryJson = featureListJson[i]["geometry"];
        Json::Value propsJson = featureListJson[i]["properties"];
        
        std::string geometryType = geometryJson["type"].asString();
        
        float height  = 0.0f;
        if (propsJson.isMember("height")) {
            height = propsJson["height"].asFloat();
        }
        
        float minHeight = 0.0f;
        if (propsJson.isMember("min_height")) {
            minHeight = propsJson["min_height"].asFloat();
        }
        
        if (geometryType.compare("Point") == 0) {
            
            // TODO
            std::cout << "TODO: Point" << std::endl;
            
        } else if (geometryType.compare("MultiPoint") == 0) {
            
            // TODO
            std::cout << "TODO: MultiPoint" << std::endl;
            
        } else if (geometryType.compare("LineString") == 0) {
            
            glmTileFeature feature;
            
            if (propsJson.isMember("name")) {
                for (int i = 0; i < geometryJson["coordinates"].size(); i++) {
                    feature.polyline.add(glm::vec3(lon2x(geometryJson["coordinates"][i][0].asFloat()),
                                                   lat2y(geometryJson["coordinates"][i][1].asFloat()),
                                                   _minHeight) - m_geometryOffset);
                }
                feature.polyline.addToMesh(feature.geometry);
                feature.name = propsJson["name"].asString();
            } else {
                lineJson2Mesh(geometryJson["coordinates"], feature.geometry, _minHeight);
            }
            
            _features.push_back(feature);
            
        } else if (geometryType.compare("MultiLineString") == 0) {
            
            for (int j = 0; j < geometryJson["coordinates"].size(); j++) {
                glmTileFeature feature;
                lineJson2Mesh(geometryJson["coordinates"][j], feature.geometry, _minHeight);
                _features.push_back(feature);
            }
            
        } else if (geometryType.compare("Polygon") == 0) {
            
            glmTileFeature feature;
            polygonJson2Mesh(geometryJson["coordinates"], feature.geometry, _minHeight + minHeight, height);
            _features.push_back(feature);
            
        } else if (geometryType.compare("MultiPolygon") == 0) {
            
            for (int j = 0; j < geometryJson["coordinates"].size(); j++) {
                glmTileFeature feature;
                polygonJson2Mesh(geometryJson["coordinates"][j], feature.geometry, _minHeight + minHeight, height);
                _features.push_back(feature);
            }
            
        } else if (geometryType.compare("GeometryCollection") == 0) {
            
            // TODO
            std::cout << "TODO: GeometryCollection" << std::endl;
        }
        
    }
}

void glmTile::polygonJson2Mesh(Json::Value &polygonJson, glmMesh &_mesh, float minHeight, float height) {
    m_tess = tessNewTess(NULL);
    
    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();
    glmRectangle bBox;
    
    _mesh.setDrawMode(GL_TRIANGLES);
    
    std::vector<glmPolyline> polyLines;
    for (int i = 0; i < polygonJson.size(); i++) {
        
        glmPolyline ringCoords;
        
        int nRingVerts = polygonJson[0].size();
        for (int i = 0; i < nRingVerts; i++) {
            ringCoords.add(glm::vec3(lon2x(polygonJson[0][i][0].asFloat()),
                                     lat2y(polygonJson[0][i][1].asFloat()),
                                     minHeight) - m_geometryOffset);
        }
        
        // Extrude polygon based on height
        if (height != minHeight) {
            
            glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
            glm::vec3 tan;
            glm::vec3 nor;
            
            for (int i = 0; i < ringCoords.size() - 1; i++) {
                
                //For each vertex in the polygon, make two triangles to form a quad
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
                _mesh.addVertex(ip0+glm::vec3(0.,0.,height - m_geometryOffset.z));
                _mesh.addNormal(nor);
                _mesh.addTexCoord(glm::vec2(0.,1.));
                _mesh.addVertex(ip1+glm::vec3(0.,0.,height - m_geometryOffset.z));
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
        
        ringCoords.growToInclude(bBox);
        polyLines.push_back(ringCoords);
    }
    
    for (int i = 0; i < polyLines.size(); i++) {
        // Add contour to tesselator
        tessAddContour(m_tess, 3, &polyLines[i][0].x, sizeof(glm::vec3), polyLines[i].size());
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
    
    const int numVertices = tessGetVertexCount(m_tess);
    const float* vertices = tessGetVertices(m_tess);
    for (int i = 0; i < numVertices; i++) {
        
        _mesh.addTexCoord(glm::vec2(mapValue(vertices[3*i],bBox.getMinX(),bBox.getMaxX(),0.,1.),
                                    mapValue(vertices[3*i+1],bBox.getMinY(),bBox.getMaxY(),0.,1.)));
        _mesh.addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
        _mesh.addVertex(glm::vec3(vertices[3*i], vertices[3*i + 1], vertices[3*i + 2] + height));
    }
    
    tessDeleteTess(m_tess);
}

void glmTile::lineJson2Mesh(Json::Value &lineJson, glmMesh &_mesh, float _minHeight){
    glmPolyline polyline;
    for (int i = 0; i < lineJson.size(); i++) {
        polyline.add(glm::vec3(lon2x(lineJson[i][0].asFloat()),
                               lat2y(lineJson[i][1].asFloat()),
                               _minHeight) - m_geometryOffset);
    }
    polyline.addToMesh(_mesh);
}

void glmTile::draw(){
    for (int i = 0; i < earth.size(); i++) {
        glColor3f(0.0,0.9,0.0);
        earth[i].geometry.draw();
    }
    
    for (int i = 0; i < water.size(); i++) {
        glColor3f(0.,0.,0.9);
        water[i].geometry.draw();
    }

    for (int i = 0; i < roads.size(); i++) {
        glColor3f(1.,1.,1.);
        roads[i].geometry.draw();
    }

    for (int i = 0; i < buildings.size(); i++) {
        glColor3f(0.9,0.2,0.0);
        buildings[i].geometry.draw();
    }
}