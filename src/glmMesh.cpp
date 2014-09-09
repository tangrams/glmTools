//
//  glmMesh.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#include "glmMesh.h"

glmMesh::glmMesh(){
    drawMode = GL_TRIANGLES;
}

void glmMesh::setDrawMode(GLenum _drawMode){
    drawMode = _drawMode;
}

void glmMesh::setColor(const glm::vec4 &_color){
    colors.clear();
    for (int i = 0; i < vertices.size(); i++) {
        colors.push_back(_color);
    }
}

void glmMesh::addColor(const glm::vec4 &_color){
    colors.push_back(_color);
}

void glmMesh::addVertex(const glm::vec3 &_point){
    vertices.push_back(_point);
}

void glmMesh::addVertices(const std::vector<glm::vec3>& _verts){
    vertices.insert(vertices.end(),_verts.begin(),_verts.end());
}

void glmMesh::addVertices(const glm::vec3* verts, int amt){
    vertices.insert(vertices.end(),verts,verts+amt);
}

void glmMesh::addNormal(const glm::vec3 &_normal){
    normals.push_back(_normal);
}

void glmMesh::addTexCoord(const glm::vec2 &_uv){
    texCoords.push_back(_uv);
}

void glmMesh::addIndex(uint16_t _i){
    indices.push_back(_i);
}

void glmMesh::addIndices(const std::vector<uint16_t>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
}

void glmMesh::addIndices(const uint16_t* inds, int amt){
	indices.insert(indices.end(),inds,inds+amt);
}

void glmMesh::addTriangle(uint16_t index1, uint16_t index2, uint16_t index3){
    addIndex(index1);
    addIndex(index2);
    addIndex(index3);
}

std::vector<glm::vec4> & glmMesh::getColors(){
    return colors;
}

std::vector<glm::vec3> & glmMesh::getVertices(){
	return vertices;
}

std::vector<glm::vec3> & glmMesh::getNormals(){
    return normals;
}

std::vector<glm::vec2> & glmMesh::getTexCoords(){
    return texCoords;
}

std::vector<glm::uint16_t> & glmMesh::getIndices(){
    return indices;
}

void glmMesh::clear(){
    if(!vertices.empty()){
		vertices.clear();
	}
	if(!colors.empty()){
		colors.clear();
	}
	if(!normals.empty()){
		normals.clear();
	}
    if(!indices.empty()){
		indices.clear();
	}

}

void glmMesh::draw(){
    if(vertices.size()){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), &vertices[0].x);
	}
	if(normals.size()){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(glm::vec3), &normals[0].x);
	}
	if(colors.size()){
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT, sizeof(glm::vec4), &colors[0].x);
	}
    if(texCoords.size()){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(glm::vec2), &texCoords[0].x);
	}

    if(indices.size()){
        glDrawElements(drawMode, indices.size(),GL_UNSIGNED_SHORT,&indices[0]);
    }else{
        glDrawArrays(drawMode, 0, vertices.size());
    }
    
    if(texCoords.size()){
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	if(colors.size()){
		glDisableClientState(GL_COLOR_ARRAY);
	}
	if(normals.size()){
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}