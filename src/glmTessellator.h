//
//  glmTessellator.h
//  Labeling
//
//  Created by Patricio Gonzalez Vivo on 8/26/14.
//
//

#pragma once

#include "tesselator.h"

#include "glmPolyline.h"
#include "glmMesh.h"

enum PolyWindingMode{
	POLY_WINDING_ODD 	        ,
	POLY_WINDING_NONZERO         ,
	POLY_WINDING_POSITIVE        ,
	POLY_WINDING_NEGATIVE        ,
	POLY_WINDING_ABS_GEQ_TWO
};

class glmTessellator{
public:
	
	glmTessellator();
	virtual ~glmTessellator();
	glmTessellator(const glmTessellator & mom);
	glmTessellator & operator=(const glmTessellator & mom);
    
	void tessellateToMesh( const vector<glmPolyline>& src, PolyWindingMode polyWindingMode, glmMesh & dstmesh, bool bIs2D=false );
	void tessellateToMesh( const glmPolyline& src,  PolyWindingMode polyWindingMode, glmMesh& dstmesh, bool bIs2D=false );
    
	void tessellateToPolylines( const vector<glmPolyline>& src, PolyWindingMode polyWindingMode, vector<glmPolyline>& dstpoly, bool bIs2D=false );
	void tessellateToPolylines( const glmPolyline & src, PolyWindingMode polyWindingMode, vector<glmPolyline>& dstpoly, bool bIs2D=false );
    
private:
	
	void performTessellation( PolyWindingMode polyWindingMode, glmMesh& dstmesh, bool bIs2D );
	void performTessellation( PolyWindingMode polyWindingMode, vector<glmPolyline>& dstpoly, bool bIs2D );
	void init();
    
	TESStesselator * cacheTess;
	TESSalloc tessAllocator;
};