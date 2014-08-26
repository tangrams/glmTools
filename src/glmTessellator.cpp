//
//  glmTessellator.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/26/14.
//
//

#include "glmTessellator.h"

void * memAllocator( void *userData, unsigned int size ){
	return malloc(size);
}

void * memReallocator( void *userData, void* ptr, unsigned int size ){
	return realloc(ptr,size);
}

void memFree( void *userData, void *ptr ){
	free (ptr);
}

//----------------------------------------------------------
glmTessellator::glmTessellator(){
	init();
}

//----------------------------------------------------------
glmTessellator::~glmTessellator(){
	tessDeleteTess(cacheTess);
}

//----------------------------------------------------------
glmTessellator::glmTessellator(const glmTessellator & mom){
	if(&mom != this) init();
}

//----------------------------------------------------------
glmTessellator & glmTessellator::operator=(const glmTessellator & mom){
	if(&mom != this) init();
	return *this;
}

//----------------------------------------------------------
void glmTessellator::init(){
	tessAllocator.memalloc = memAllocator;
	tessAllocator.memrealloc = memReallocator;
	tessAllocator.memfree = memFree;
	tessAllocator.meshEdgeBucketSize=0;
	tessAllocator.meshVertexBucketSize=0;
	tessAllocator.meshFaceBucketSize=0;
	tessAllocator.dictNodeBucketSize=0;
	tessAllocator.regionBucketSize=0;
	tessAllocator.extraVertices=0;
	cacheTess = tessNewTess( &tessAllocator );
}

//----------------------------------------------------------
void glmTessellator::tessellateToMesh( const glmPolyline& src,  PolyWindingMode polyWindingMode, glmMesh& dstmesh, bool bIs2D){
    
	glmPolyline& polyline = const_cast<glmPolyline&>(src);
	tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0], sizeof(vec3), polyline.size());
    
	performTessellation( polyWindingMode, dstmesh, bIs2D );
}


//----------------------------------------------------------
void glmTessellator::tessellateToMesh( const vector<glmPolyline>& src, PolyWindingMode polyWindingMode, glmMesh & dstmesh, bool bIs2D ) {
    
    
	// pass vertex pointers to GLU tessellator
	for ( int i=0; i<(int)src.size(); ++i ) {
		glmPolyline& polyline = const_cast<glmPolyline&>(src[i]);
        
		tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0].x, sizeof(vec3), polyline.size());
	}
    
	performTessellation( polyWindingMode, dstmesh, bIs2D );
}

//----------------------------------------------------------
void glmTessellator::tessellateToPolylines( const glmPolyline& src,  PolyWindingMode polyWindingMode, vector<glmPolyline>& dstpoly, bool bIs2D){
    
	glmPolyline& polyline = const_cast<glmPolyline&>(src);
	tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0], sizeof(vec3), polyline.size());
    
	performTessellation( polyWindingMode, dstpoly, bIs2D );
}


//----------------------------------------------------------
void glmTessellator::tessellateToPolylines( const vector<glmPolyline>& src, PolyWindingMode polyWindingMode, vector<glmPolyline>& dstpoly, bool bIs2D ) {
    
	// pass vertex pointers to GLU tessellator
	for ( int i=0; i<(int)src.size(); ++i ) {
		glmPolyline& polyline = const_cast<glmPolyline&>(src[i]);
        
		tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0].x, sizeof(vec3), polyline.size());
	}
    
	performTessellation( polyWindingMode, dstpoly, bIs2D );
}


//----------------------------------------------------------
void glmTessellator::performTessellation(PolyWindingMode polyWindingMode, glmMesh& dstmesh, bool bIs2D ) {
    
	if (!tessTesselate(cacheTess, polyWindingMode, TESS_POLYGONS, 3, 3, 0)){
//		ofLogError("glmTessellator") << "performTessellation(): mesh polygon tessellation failed, winding mode " << polyWindingMode;
		return;
	}
    
	int numVertices = tessGetVertexCount( cacheTess );
	int numIndices = tessGetElementCount( cacheTess )*3;
    
	dstmesh.clear();
	dstmesh.addVertices((vec3*)tessGetVertices(cacheTess),numVertices);
	dstmesh.addIndices((uint16_t*)tessGetElements(cacheTess),numIndices);
	/*ofIndexType * tessElements = (ofIndexType *)tessGetElements(cacheTess);
     for(int i=0;i<numIndices;i++){
     if(tessElements[i]!=TESS_UNDEF)
     dstmesh.addIndex(tessElements[i]);
     }*/
//	dstmesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
}


//----------------------------------------------------------
void glmTessellator::performTessellation(PolyWindingMode polyWindingMode, vector<glmPolyline>& dstpoly, bool bIs2D ) {
	if (!tessTesselate(cacheTess, polyWindingMode, TESS_BOUNDARY_CONTOURS, 0, 3, 0)){
//		ofLogError("glmTessellator") << "performTesselation(): polyline boundary contours tessellation failed, winding mode " << polyWindingMode;
		return;
	}
    
	const vec3* verts = (vec3*)tessGetVertices(cacheTess);
	const TESSindex* elems = tessGetElements(cacheTess);
	const int nelems = tessGetElementCount(cacheTess);
	dstpoly.resize(nelems);
	for (int i = 0; i < nelems; ++i)
	{
        int b = elems[i*2];
        int n = elems[i*2+1];
        dstpoly[i].clear();
        dstpoly[i].addVertices(&verts[b],n);
//        dstpoly[i].setClosed(true);
	}
}
