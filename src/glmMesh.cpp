//
//  glmMesh.cpp
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#include "glmMesh.h"

glmMesh::glmMesh(){
}

void glmMesh::addVertex(const vec3 &_point){
    vertices.push_back(_point);
}

void glmMesh::addVertices(const vector<vec3>& _verts){
    vertices.insert(vertices.end(),_verts.begin(),_verts.end());
}

void glmMesh::addVertices(const vec3* verts, int amt){
    vertices.insert(vertices.end(),verts,verts+amt);
}

void glmMesh::addNormal(const vec3 &_normal){
    normals.push_back(_normal);
}

void glmMesh::addColor(const vec4 &_color){
    colors.push_back(_color);
}

void glmMesh::addIndex(uint16_t _i){
    indices.push_back(_i);
}

void glmMesh::addIndices(const vector<uint16_t>& inds){
	indices.insert(indices.end(),inds.begin(),inds.end());
}

//--------------------------------------------------------------
void glmMesh::addIndices(const uint16_t* inds, int amt){
	indices.insert(indices.end(),inds,inds+amt);
}

vector<vec3> & glmMesh::getVertices(){
	return vertices;
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

void glmMesh::draw(GLenum _drawMode){
    if(vertices.size()){
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(vec3), &vertices[0].x);
	}
	if(normals.size()){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(vec3), &normals[0].x);
	}
	if(colors.size()){
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT, sizeof(vec4), &colors[0].x);
	}

    if(indices.size()){
        glDrawElements(_drawMode, indices.size(),GL_UNSIGNED_SHORT,&indices[0]);
    }else{
        glDrawArrays(_drawMode, 0, vertices.size());
    }
    
	glDrawArrays(_drawMode, 0, vertices.size());
    
	if(colors.size()){
		glDisableClientState(GL_COLOR_ARRAY);
	}
	if(normals.size()){
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}