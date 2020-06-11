#include "ObjectPolygon.h"

ObjectPolygon::ObjectPolygon(){
	//Reset all data
	Vertices = NULL;
	VertexIndex = NULL;
	ClippedVertices = NULL;
	UV_Coords = NULL;
	number_vertices = 0;
	number_clipped_vertices = 0;
	SHADING = PolyShadingFlat;
	FACE = PolyFacePlain;
	MEDIUM = PolyMediumSolid;
	LAST = Last_FLAT;
	FaceNormal.Set(0,0,0);
	texture_index = NO_TEXTURE_INDEX;
	ClippedBoundingPlanes = 0;
	ClippedZPlanes = 0;
	number_planes_tested = 0;
	fully_clipped = partially_clipped = fully_visible = false;
	culled = false;
	double_sided = false;
}
/***********************************************************/
ObjectPolygon::~ObjectPolygon(){
	//NOTE: Since this is an object polygon,
	//object is responsible for deleting Vertices array

	//Delete all arrays
	if (ClippedVertices)delete [] ClippedVertices;
	if (VertexIndex)delete [] VertexIndex;
	if (UV_Coords)delete [] UV_Coords;
//	MEMORY.DestroyMemory((void **)&ClippedVertices);
//	MEMORY.DestroyMemory((void **)&VertexIndex);
//	MEMORY.DestroyMemory((void **)&UV_Coords);
	Vertices = NULL;
}
/***********************************************************/
//Init functions
/***********************************************************/
void ObjectPolygon::InitPolygon(int num_vertices, Vertex *V_List,
				 int *V_Index, float *UV_List,
				 PolyShadingType S, PolyFaceType F, PolyMediumType M,
				 Color col, int text_index){
	int j=0;

	//Init proper data
	number_vertices = num_vertices;
	SHADING = S;
	FACE = F;
	MEDIUM = M;
	FaceColor = col;
	OriginalFaceColor = col;
	texture_index = text_index;

	//Get last state type
	if (FACE == PolyFaceTextured){
		if (MEDIUM == PolyMediumAlpha)
			LAST = Last_TEXTURE_ALPHA;
		else
			LAST = Last_TEXTURE;
	}else{
		if (MEDIUM == PolyMediumAlpha)
			LAST = Last_ALPHA;
		else
			LAST = Last_FLAT;
	};

	//Get rid of any old allocated arrays
//	if (ClippedVertices)delete [] ClippedVertices;
//	MEMORY.DestroyMemory((void **)&ClippedVertices);
//	if (VertexIndex)delete [] VertexIndex;
///	MEMORY.DestroyMemory((void **)&VertexIndex);
//	if (UV_Coords)delete [] UV_Coords;
//	MEMORY.DestroyMemory((void **)&UV_Coords);

	//Allocate array data
	Vertices = V_List;
	ClippedVertices = new Vertex[number_vertices+NUMBER_FRUSTRUM_PLANES];
//	ClippedVertices = (Vertex *)MEMORY.AllocateMemory(sizeof(Vertex), number_vertices+NUMBER_FRUSTRUM_PLANES);
	VertexIndex = new int[number_vertices];
//	VertexIndex = (int *)MEMORY.AllocateMemory(sizeof(int), number_vertices);
	UV_Coords = new float[number_vertices<<1];
//	UV_Coords = (float *)MEMORY.AllocateMemory(sizeof(float), number_vertices<<1);

	//Copy array data
	for (j = 0; j < number_vertices; j++){
		VertexIndex[j] = V_Index[j];
		Vertices[VertexIndex[j]].part_of_visible_polygon = false;
		UV_Coords[j<<1] = UV_List[j<<1];
		UV_Coords[(j<<1)+1] = UV_List[(j<<1)+1];
	};
}
/***********************************************************/
void ObjectPolygon::CalculateNormal(Vertex *LocalVertices){
	Vector u, v;			//Triangle edge vectors for calculating the normal
	Vertex *v0, *v1, *v2;
	int v_0 = VertexIndex[0];
	int v_1 = VertexIndex[1];
	int v_2 = VertexIndex[2];

	//Get vector pointers
	v0 = &LocalVertices[VertexIndex[0]];
	v1 = &LocalVertices[VertexIndex[1]];
	v2 = &LocalVertices[VertexIndex[2]];

	//Get edge vectors
//	u.MakeVector(LocalVertices[VertexIndex[0]], LocalVertices[VertexIndex[1]]);
//	v.MakeVector(LocalVertices[VertexIndex[0]], LocalVertices[VertexIndex[2]]);
	u.x = v1->x - v0->x;
	u.y = v1->y - v0->y;
	u.z = v1->z - v0->z;
	v.x = v2->x - v0->x;
	v.y = v2->y - v0->y;
	v.z = v2->z - v0->z;

	//Calculate normal
	CrossProduct(u, v, FaceNormal);

	//Normalize vector
	FaceNormal.Normalize();
}
/***********************************************************/
//Clip functions
/***********************************************************/
//Tests 90 fov only
void ObjectPolygon::ClipTest(void){
	int j;
	Vertex *curVertex=NULL;
	int ZNear=0, ZFar=0;
	int XLeft=0, XRight=0;
	int YBottom=0, YTop=0;

	//Reset data
	fully_clipped = false;
	partially_clipped = false;
	fully_visible = false;

	//Reset clip planes
	ClippedBoundingPlanes = 0;
	ClippedZPlanes = 0;
	number_planes_tested = 0;
	number_clipped_vertices = 0;
	if ((CLIP_TYPE == DS2_CLIP_Z_ONLY)||(CLIP_TYPE == DS2_CLIP_2D_SCREEN))number_clipped_vertices = number_vertices;

	DEBUG("Clipping Poly Test");

	for (j = 0; j < number_vertices; j++){
		DEBUGf("Vertice#%d", j);

		//Get next vertex
		curVertex = &Vertices[VertexIndex[j]];
		if ((CLIP_TYPE == DS2_CLIP_Z_ONLY)||(CLIP_TYPE == DS2_CLIP_2D_SCREEN))ClippedVertices[j] = *curVertex;

		//Test NEAR/FAR z values
		if (curVertex->z < NEAR_Z){
			DEBUG("Near Clip");
			ZNear++;
		}else if (curVertex->z > FAR_Z){
			DEBUG("Far Clip");
			ZFar++;
		};

		//Test LEFT/RIGHT x values
		if (curVertex->x < -(curVertex->z)){
			DEBUG("Left Clip");
			XLeft++;
		}else if (curVertex->x > curVertex->z){
			DEBUG("Right Clip");
			XRight++;
		};

		//Test BOTTOM/TOP y values
		if (curVertex->y < -(curVertex->z)){
			DEBUG("Bottom Clip");
			YBottom++;
		}else if (curVertex->y > curVertex->z){
			DEBUG("Top Clip");
			YTop++;
		};


	};

	//Insert clip amounts
	//Z Planes
	ClippedZPlanes = (ZFar << FAR_SHIFT) | (ZNear << NEAR_SHIFT);
	//Left/Right/Top/Bottom Planes
	ClippedBoundingPlanes = (YBottom << BOTTOM_SHIFT) | (YTop << TOP_SHIFT) |
		                    (XRight << RIGHT_SHIFT) | (XLeft << LEFT_SHIFT);

	//Do trivial rejection tests
	if ( (ZNear == number_vertices) ||
		 (ZFar == number_vertices) ||
		 (XLeft == number_vertices) ||
		 (XRight == number_vertices) ||
		 (YTop == number_vertices) ||
		 (YBottom == number_vertices)){
		//Set to fully clipped
		fully_clipped = true;
	}else if ((ClippedZPlanes > 0) || (ClippedBoundingPlanes > 0)){
		//Set to partially clipped
		partially_clipped = true;
	}else{
		//Set to fully visible
		fully_visible = true;
	};
}
/***********************************************************/
/*int ObjectPolygon::ClipLineToPlane(ClippingLine &in, ClippingLine &out, Plane &ClipPlane){
	float dp1, dp2;
	float scale;
    Vector *n;
	int rt=0, r0, g0, b0, a0, r1, g1, b1, a1;
	Vertex *v0, *v1, *ov0, *ov1;
	Color V0, V1;
	n = &ClipPlane.normal;
	v0 = &in.v0;
	v1 = &in.v1;
	ov0 = &out.v0;
	ov1 = &out.v1;
	DEBUG("Clipping Line to Plane:");
//	DEBUGf("V0(%.2f,%.2f,%.2f)", in.v0.x, in.v0.y, in.v0.z);
//	DEBUGf("V1(%.2f,%.2f,%.2f)", in.v1.x, in.v1.y, in.v1.z);
//	DEBUGf("Plane Normal(%.2f,%.2f,%.2f)", ClipPlane.normal.x, ClipPlane.normal.y, ClipPlane.normal.z);
//	DEBUGf("Plane Distance=%.2f", ClipPlane.distance);
//	DEBUGf("Starting Clip...");
	dp1 = v0->x * n->x + v0->y * n->y + v0->z * n->z;
	dp2 = v1->x * n->x + v1->y * n->y + v1->z * n->z;
//	if (switch_dp){
//		t = dp1;
//		dp1 = dp2;
//		dp2 = t;
//	};
//	DEBUGf("DotProduct1=%.2f", dp1);
//	DEBUGf("DotProduct2=%.2f", dp2);

	if ((dp1 >= ClipPlane.distance)&&(dp2 >= ClipPlane.distance)){
		rt = CLIPPED_NONE;
		out.CopyToV0(in.v0);
		out.CopyToV1(in.v1);
//		DEBUG("Clipped Neither Point");
	}else if ((dp1 < ClipPlane.distance)&&(dp2 < ClipPlane.distance)){
		rt = CLIPPED_ALL;
//		DEBUG("Clipped Both Points");
	}else{
        //Check to see if first point is outside plane
		if (dp2 >= ClipPlane.distance){
			scale = (ClipPlane.distance-dp2) / (dp1-dp2);
//			 DEBUGf("Clipping V0 with scale=%.2f", scale);
//			 v0 = &InLine->v0;
//			 v1 = &InLine->v1;
//			 ov0 = &OutLine->v0;
//			 ov1 = &OutLine->v1;
			 //Get new data
			 //XYZ*****************************
//			 *ov1 = *v1;
			 ov0->x = v1->x + ((v0->x - v1->x) * scale);
			 ov0->y = v1->y + ((v0->y - v1->y) * scale);
			 ov0->z = v1->z + ((v0->z - v1->z) * scale);
			 //********************************
			 //RGB
             ov0->color = 0;
			 if (SHADING == PolyShadingGouraud){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				r0 = V0.GetR();  r1 = V1.GetR();
				g0 = V0.GetG();  g1 = V1.GetG();
				b0 = V0.GetB();  b1 = V1.GetB();
//				r0 = v0->color & RED_COLOR;   r1 = v1->color & RED_COLOR;
//				g0 = v0->color & GREEN_COLOR; g1 = v1->color & GREEN_COLOR;
//				b0 = v0->color & BLUE_COLOR;  b1 = v1->color & BLUE_COLOR;
		 	    ov0->color |= (int)(r1 + ((r0 - r1) * scale)) << RED_SHIFT;
			    ov0->color |= (int)(g1 + ((g0 - g1) * scale)) << GREEN_SHIFT;
			    ov0->color |= (int)(b1 + ((b0 - b1) * scale)) << BLUE_SHIFT;
			 };

			 //********************************
			 //Alpha
			 if (MEDIUM == PolyMediumAlpha){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				a0 = V0.GetA(); a1 = V1.GetA();
				ov0->color |= (unsigned int)(a1 + ((a0 - a1) * scale)) << ALPHA_SHIFT;
			 };
			 //********************************
			 //UV Coords
			 if (FACE == PolyFaceTextured){
				 ov0->u0 = v1->u0 + ((v0->u0 - v1->u0) * scale);
				 ov0->v0 = v1->v0 + ((v0->v0 - v1->v0) * scale);
//				 ov0->u1 = v1->u1 + ((v0->u1 - v1->u1) * scale);
//				 ov0->v1 = v1->v1 + ((v0->v1 - v1->v1) * scale);
			 };
			 //Copy in vertex 1 to out vertex 1
			 out.CopyToV1(in.v1);
			 rt = CLIPPED_V0;
		}else{
			//The other point is outside
			scale = (ClipPlane.distance-dp1) / (dp2-dp1);
//			 DEBUGf("Clipping V1 with scale=%.2f", scale);
//			 v0 = &InLine->v0;
//			 v1 = &InLine->v1;
//			 ov0 = &OutLine->v0;
//			 ov1 = &OutLine->v1;
			 //Get new data
			 //XYZ***************************
//			 *ov0 = *v0;
			 ov1->x = v0->x + ((v1->x - v0->x) * scale);
			 ov1->y = v0->y + ((v1->y - v0->y) * scale);
			 ov1->z = v0->z + ((v1->z - v0->z) * scale);
			 //******************************
			 //RGB
             ov1->color = 0;
			 if (SHADING == PolyShadingGouraud){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				r0 = V0.GetR();  r1 = V1.GetR();
				g0 = V0.GetG();  g1 = V1.GetG();
				b0 = V0.GetB();  b1 = V1.GetB();
//				r0 = v0->color & RED_COLOR;   r1 = v1->color & RED_COLOR;
//				g0 = v0->color & GREEN_COLOR; g1 = v1->color & GREEN_COLOR;
//				b0 = v0->color & BLUE_COLOR;  b1 = v1->color & BLUE_COLOR;
		 	    ov1->color |= (int)(r0 + ((r1 - r0) * scale)) << RED_SHIFT;
			    ov1->color |= (int)(g0 + ((g1 - g0) * scale)) << GREEN_SHIFT;
			    ov1->color |= (int)(b0 + ((b1 - b0) * scale)) << BLUE_SHIFT;
			 };

			 //********************************
			 //Alpha
			 if (MEDIUM == PolyMediumAlpha){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				a0 = V0.GetA(); a1 = V1.GetA();
				ov1->color |= (unsigned int)(a0 + ((a1 - a0) * scale)) << ALPHA_SHIFT;
			 };
			 //********************************
			 //UV Coords
			 if (FACE == PolyFaceTextured){
				 ov1->u0 = v0->u0 + ((v1->u0 - v0->u0) * scale);
				 ov1->v0 = v0->v0 + ((v1->v0 - v0->v0) * scale);
//				 ov1->u1 = v0->u1 + ((v1->u1 - v0->u1) * scale);
//				 ov1->v1 = v0->v1 + ((v1->v1 - v0->v1) * scale);
			 };
			 //Copy in vertex 1 to out vertex 1
			 out.CopyToV0(in.v0);
			 rt = CLIPPED_V1;
		};
	};
//	DEBUGf("Clip Done!\n");
	return(rt);
}
/***********************************************************/
/*int ObjectPolygon::ClipLineToFarPlane(ClippingLine &in, ClippingLine &out, Plane &ClipPlane){
	float dp1, dp2;
	float scale;
    Vector *n;
	int rt=0, r0, g0, b0, a0, r1, g1, b1, a1;
	Vertex *v0, *v1, *ov0, *ov1;
	Color V0, V1;
	n = &ClipPlane.normal;
	v0 = &in.v0;
	v1 = &in.v1;
	ov0 = &out.v0;
	ov1 = &out.v1;
	DEBUG("Clipping Line to **Far** Plane:");
//    DEBUGf("V0(%.2f,%.2f,%.2f)", in.v0.x, in.v0.y, in.v0.z);
//	DEBUGf("V1(%.2f,%.2f,%.2f)", in.v1.x, in.v1.y, in.v1.z);
//	DEBUGf("Plane Normal(%.2f,%.2f,%.2f)", ClipPlane.normal.x, ClipPlane.normal.y, ClipPlane.normal.z);
//	DEBUGf("Plane Distance=%.2f", ClipPlane.distance);
//	DEBUGf("Starting Clip...");
	dp1 = v0->x * n->x + v0->y * n->y + v0->z * n->z;
	dp2 = v1->x * n->x + v1->y * n->y + v1->z * n->z;
//	DEBUGf("DotProduct1=%.2f", dp1);
//	DEBUGf("DotProduct2=%.2f", dp2);

	if ((dp1 >= ClipPlane.distance)&&(dp2 >= ClipPlane.distance)){
		rt = CLIPPED_ALL;
//		DEBUG("Clipped Both Points");
	}else if ((dp1 < ClipPlane.distance)&&(dp2 < ClipPlane.distance)){
		rt = CLIPPED_NONE;
		out.CopyToV0(in.v0);
		out.CopyToV1(in.v1);
//		DEBUG("Clipped Neither Point");
	}else{
        //Check to see if first point is outside plane
		if (dp1 >= ClipPlane.distance){
			scale = (ClipPlane.distance-dp2) / (dp1-dp2);
//			 DEBUGf("Clipping V0 with scale=%.2f", scale);
//			 v0 = &InLine->v0;
//			 v1 = &InLine->v1;
//			 ov0 = &OutLine->v0;
//			 ov1 = &OutLine->v1;
			 //Get new data
			 //XYZ*****************************
//			 *ov1 = *v1;
			 ov0->x = v1->x + ((v0->x - v1->x) * scale);
			 ov0->y = v1->y + ((v0->y - v1->y) * scale);
			 ov0->z = v1->z + ((v0->z - v1->z) * scale);
			 //********************************
			 //RGB
             ov0->color = 0;
			 if (SHADING == PolyShadingGouraud){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				r0 = V0.GetR();  r1 = V1.GetR();
				g0 = V0.GetG();  g1 = V1.GetG();
				b0 = V0.GetB();  b1 = V1.GetB();
		 	    ov0->color |= (int)(r1 + ((r0 - r1) * scale)) << RED_SHIFT;
			    ov0->color |= (int)(g1 + ((g0 - g1) * scale)) << GREEN_SHIFT;
			    ov0->color |= (int)(b1 + ((b0 - b1) * scale)) << BLUE_SHIFT;
			 };

			 //********************************
			 //Alpha
			 if (MEDIUM == PolyMediumAlpha){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				a0 = V0.GetA(); a1 = V1.GetA();
				ov0->color |= (unsigned int)(a1 + ((a0 - a1) * scale)) << ALPHA_SHIFT;
			 };
			 //********************************
			 //UV Coords
			 if (FACE == PolyFaceTextured){
				 ov0->u0 = v1->u0 + ((v0->u0 - v1->u0) * scale);
				 ov0->v0 = v1->v0 + ((v0->v0 - v1->v0) * scale);
//				 ov0->u1 = v1->u1 + ((v0->u1 - v1->u1) * scale);
//				 ov0->v1 = v1->v1 + ((v0->v1 - v1->v1) * scale);
			 };
			 //Copy in vertex 1 to out vertex 1
			 out.CopyToV1(in.v1);
			 rt = CLIPPED_V0;
		}else{
			//The other point is outside
			scale = (ClipPlane.distance-dp1) / (dp2-dp1);
//			 DEBUGf("Clipping V1 with scale=%.2f", scale);
//			 v0 = &InLine->v0;
//			 v1 = &InLine->v1;
//			 ov0 = &OutLine->v0;
//			 ov1 = &OutLine->v1;
			 //Get new data
			 //XYZ***************************
//			 *ov0 = *v0;
			 ov1->x = v0->x + ((v1->x - v0->x) * scale);
			 ov1->y = v0->y + ((v1->y - v0->y) * scale);
			 ov1->z = v0->z + ((v1->z - v0->z) * scale);
			 //******************************
			 //RGB
             ov1->color = 0;
			 if (SHADING == PolyShadingGouraud){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				r0 = V0.GetR();  r1 = V1.GetR();
				g0 = V0.GetG();  g1 = V1.GetG();
				b0 = V0.GetB();  b1 = V1.GetB();
		 	    ov1->color |= (int)(r0 + ((r1 - r0) * scale)) << RED_SHIFT;
			    ov1->color |= (int)(g0 + ((g1 - g0) * scale)) << GREEN_SHIFT;
			    ov1->color |= (int)(b0 + ((b1 - b0) * scale)) << BLUE_SHIFT;
			 };

			 //********************************
			 //Alpha
			 if (MEDIUM == PolyMediumAlpha){
				V0.ClearToColor(v0->color);
				V1.ClearToColor(v1->color);
				a0 = V0.GetA(); a1 = V1.GetA();
				ov1->color |= (unsigned int)(a0 + ((a1 - a0) * scale)) << ALPHA_SHIFT;
			 };
			 //********************************
			 //UV Coords
			 if (FACE == PolyFaceTextured){
				 ov1->u0 = v0->u0 + ((v1->u0 - v0->u0) * scale);
				 ov1->v0 = v0->v0 + ((v1->v0 - v0->v0) * scale);
//				 ov1->u1 = v0->u1 + ((v1->u1 - v0->u1) * scale);
//				 ov1->v1 = v0->v1 + ((v1->v1 - v0->v1) * scale);
			 };
			 //Copy in vertex 1 to out vertex 1
			 out.CopyToV0(in.v0);
			 rt = CLIPPED_V1;
		};
	};
//	DEBUGf("Clip Done!\n");
	return(rt);
}
/***********************************************************/
/*bool ObjectPolygon::ClipToPlane(Plane &ClipPlane, int plane_index){
	int j=0;
	int nextV=0;
	int num_verts=0, new_verts=0;
	Vertex *inVert, *outVert, *nextVert;
	bool rt=true;
	ClippingLine inLine, outLine;
	int Result=0, LastResult=CLIPPED_NONE;
	bool finish=false, finished_error=false;
	int number_total_points=0;
	int number_c_verts=0;
	float *U_Coord, *V_Coord;
	float *NextU, *NextV;
	int number_times_both_rejected=0;

	//Create temp array
	outVert = &tempClippingVertices[0];
	DEBUG("Plane Clipping Polygon");
	//Get number vertices to be processed
	switch(plane_index){
	case NEAR_INDEX : number_c_verts = (ClippedZPlanes & NEAR_CLIPPING_PLANE) >> NEAR_SHIFT;break;
	case FAR_INDEX : number_c_verts = (ClippedZPlanes & FAR_CLIPPING_PLANE) >> FAR_SHIFT;break;
	case LEFT_INDEX : number_c_verts = (ClippedBoundingPlanes & LEFT_CLIPPING_PLANE) >> LEFT_SHIFT;break;
	case RIGHT_INDEX : number_c_verts = (ClippedBoundingPlanes & RIGHT_CLIPPING_PLANE) >> RIGHT_SHIFT;break;
	case TOP_INDEX : number_c_verts = (ClippedBoundingPlanes & TOP_CLIPPING_PLANE) >> TOP_SHIFT;break;
	case BOTTOM_INDEX : number_c_verts = (ClippedBoundingPlanes & BOTTOM_CLIPPING_PLANE) >> BOTTOM_SHIFT;break;
	};

	//The polygon has not yet been clipped
	if (!number_planes_tested){
		//Setup loop variables and pointers
		num_verts = number_vertices;
		inVert = &Vertices[VertexIndex[0]];
		U_Coord = &UV_Coords[0];
		V_Coord = &UV_Coords[1];
		j = 0;
//		DEBUG("Polygon has not yet been clipped...");
//		DEBUGf("# Vertices=%d", num_verts);
		//Get total number of points to be in the final array
		number_total_points = num_verts+2-number_c_verts;
//		DEBUGf("# C Points = %d", number_c_verts);
//		DEBUGf("# End Clip Points = %d", number_total_points);
//		DEBUG("Starting main Loop...");
		//Start main clipping loop
		while(!finish){
			//Get next coordinate vars and pointers
			nextV = (j+1) % num_verts;
			nextVert = &Vertices[VertexIndex[nextV]];
			NextU = &UV_Coords[nextV<<1];
			NextV = &UV_Coords[(nextV<<1)+1];
//			DEBUGf("Next Vertex = %d", nextV);
			//Get points for line
			inLine.CopyToV0(inVert);
			inLine.CopyToV1(nextVert);
			//Copy texture coords
			inLine.v0.u0 = *U_Coord;
			inLine.v0.v0 = *V_Coord;
			inLine.v1.u0 = *NextU;
			inLine.v1.v0 = *NextV;
			//Clip line to plane
			Result = ClipLineToPlane(inLine, outLine, ClipPlane);
			//Switch out end result
			switch(Result){
			//First vertex clipped
			case CLIPPED_V0:
//				DEBUG("Result returned was Clipped_V0");
				//If last clip was none or clipped v0
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					//Copy both vertices to the temp array
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
//					DEBUG("Last result was none or v0, so copy both vertices");
				}else{
					//Otherwise, v1 was clipped last
					//So that means v0 of this line is already in the array
					//Copy v1 into the array
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
//					DEBUG("Copying v1 only");
				};break;
			//Second vertex clipped
			case CLIPPED_V1:
//				DEBUG("Return result was Clipped_V1");
				//If last clip was none or clipped v0
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					//Copy both vertices to the temp array
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
//					DEBUG("Last result was none or v0, so copy both vertices");
				}else{
					//Otherwise, v1 was clipped last
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
//					DEBUG("Copying v1 only");
				};break;
			//If no points were clipped
			case CLIPPED_NONE:
//				DEBUG("Return result was Clipped_None");
				//If last clip was none or v0
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					//Copy both points
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
//					DEBUG("Last result was none or v0, so copy both vertices");
				}else{
					//Last clip was v1, so copy v1
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
//					DEBUG("Copying v1 only");
				};break;
			case CLIPPED_ALL:
//				DEBUG("Clipped Both points");
				number_times_both_rejected++;
//				DEBUGf("NumTimesReject->%d", number_times_both_rejected);
				if (number_times_both_rejected > number_total_points){
//					DEBUG("Finish Error!");
					finished_error = true;
					finish = true;
				};break;
			};
			//Increment loop
			j++;
			//Get next loop vars.
			LastResult = Result;
			inVert = nextVert;
			U_Coord = NextU;
			V_Coord = NextV;
			//If new verts added to temp array == total points
			//we are done
			if (new_verts >= number_total_points)finish = true;
		};
	//Polygon has been clipped at least once
	}else{
//		DEBUGf("Polygon has been clipped %d times...", number_planes_tested);
		//Get startup data
		num_verts = number_clipped_vertices;
		inVert = &ClippedVertices[0];
		j = 0;
		//Get total number of points to be in the final array
		number_total_points = num_verts+2-number_c_verts;
		//Start loop
		while(!finish){
			//Get next vertex vars and pointers
			nextV = (j+1) % num_verts;
			nextVert = &ClippedVertices[nextV];
			//Get points for line
			inLine.CopyToV0(inVert);
			inLine.CopyToV1(nextVert);
			//Clip line
			Result = ClipLineToPlane(inLine, outLine, ClipPlane);
			//Switch clip result(NOTE:All corresponding cases are
			//commented above)
			switch(Result){
			case CLIPPED_V0:
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
				}else{
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
				};break;
			case CLIPPED_V1:
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
				}else{
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
				};break;
			case CLIPPED_NONE:
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
				}else{
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
				};break;
			case CLIPPED_ALL:
				number_times_both_rejected++;
				if (number_times_both_rejected > number_total_points){
					finished_error = true;
					finish = true;
				};break;
			};
			//Increment loop
			j++;
			//Get next loop vars.
			LastResult = Result;
			inVert = nextVert;
			//If all verts are processed, we are done
			if (new_verts >= number_total_points)finish = true;
		};
	};
	DEBUG("Clip Loop finished...");
	if (!finished_error){
		//Set number new vertices to number added vertices
		number_clipped_vertices = number_total_points;
		//Increment number planes tested
		number_planes_tested++;
		//Copy vertices
		for (j = 0; j < number_clipped_vertices; j++)
			ClippedVertices[j] = tempClippingVertices[j];
		//If number vertices left do not make a triangle, then return false
		if (number_clipped_vertices < 3)rt = false;
	}else{
		//All points were clipped
		rt = false;
	};
	return(rt);
}
/***********************************************************/
/*bool ObjectPolygon::ClipToFarPlane(Plane &ClipPlane){
	int j=0;
	int nextV=0;
	int num_verts=0, new_verts=0;
	Vertex *inVert, *outVert, *nextVert;
	bool rt=true;
	ClippingLine inLine, outLine;
	int Result=0, LastResult=CLIPPED_NONE;
	bool finish=false, finished_error=false;
	int number_total_points=0;
	int number_c_verts=0;
	float *U_Coord, *V_Coord;
	float *NextU, *NextV;
	int number_times_both_rejected=0;
	//Create temp array
	outVert = &tempClippingVertices[0];
	DEBUG("Far Plane Clipping Polygon");
	//Get number vertices to be processed
	number_c_verts = (ClippedZPlanes & FAR_CLIPPING_PLANE) >> FAR_SHIFT;

	//The polygon has not yet been clipped
	if (!number_planes_tested){
		//Setup loop variables and pointers
		num_verts = number_vertices;
		inVert = &Vertices[VertexIndex[0]];
		U_Coord = &UV_Coords[0];
		V_Coord = &UV_Coords[1];
		j = 0;
//		DEBUG("Polygon has not yet been clipped...");
//		DEBUGf("# Vertices=%d", num_verts);
		//Get total number of points to be in the final array
		number_total_points = num_verts+2-number_c_verts;
//		DEBUGf("# C Points = %d", number_c_verts);
//		DEBUGf("# End Clip Points = %d", number_total_points);
//		DEBUG("Starting main Loop...");
		//Start main clipping loop
		while(!finish){
			//Get next coordinate vars and pointers
			nextV = (j+1) % num_verts;
			nextVert = &Vertices[VertexIndex[nextV]];
			NextU = &UV_Coords[nextV<<1];
			NextV = &UV_Coords[(nextV<<1)+1];
//			DEBUGf("Next Vertex = %d", nextV);
			//Get points for line
			inLine.CopyToV0(inVert);
			inLine.CopyToV1(nextVert);
			//Copy texture coords
			inLine.v0.u0 = *U_Coord;
			inLine.v0.v0 = *V_Coord;
			inLine.v1.u0 = *NextU;
			inLine.v1.v0 = *NextV;
			//Clip line to plane
			Result = ClipLineToFarPlane(inLine, outLine, ClipPlane);
			//Switch out end result
			switch(Result){
			//First vertex clipped
			case CLIPPED_V0:
//				DEBUG("Result returned was Clipped_V0");
				//If last clip was none or clipped v0
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					//Copy both vertices to the temp array
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
//					DEBUG("Last result was none or v0, so copy both vertices");
				}else{
					//Otherwise, v1 was clipped last
					//So that means v0 of this line is already in the array
					//Copy v1 into the array
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
//					DEBUG("Copying v1 only");
				};break;
			//Second vertex clipped
			case CLIPPED_V1:
//				DEBUG("Return result was Clipped_V1");
				//If last clip was none or clipped v0
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					//Copy both vertices to the temp array
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
//					DEBUG("Last result was none or v0, so copy both vertices");
				}else{
					//Otherwise, v1 was clipped last
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
//					DEBUG("Copying v1 only");
				};break;
			//If no points were clipped
			case CLIPPED_NONE:
//				DEBUG("Return result was Clipped_None");
				//If last clip was none or v0
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					//Copy both points
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
//					DEBUG("Last result was none or v0, so copy both vertices");
				}else{
					//Last clip was v1, so copy v1
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
//					DEBUG("Copying v1 only");
				};break;
			case CLIPPED_ALL:
				number_times_both_rejected++;
				if (number_times_both_rejected > number_total_points){
					finished_error = true;
					finish = true;
				};break;
			};
			//Increment loop
			j++;
			//Get next loop vars.
			LastResult = Result;
			inVert = nextVert;
			U_Coord = NextU;
			V_Coord = NextV;
			//If new verts added to temp array == total points
			//we are done
			if (new_verts >= number_total_points)finish = true;
		};
	//Polygon has been clipped at least once
	}else{
//		DEBUGf("Polygon has been clipped %d times...", number_planes_tested);
		//Get startup data
		num_verts = number_clipped_vertices;
		inVert = &ClippedVertices[0];
		j = 0;
		//Get total number of points to be in the final array
		number_total_points = num_verts+2-number_c_verts;
		//Start loop
		while(!finish){
			//Get next vertex vars and pointers
			nextV = (j+1) % num_verts;
			nextVert = &ClippedVertices[nextV];
			//Get points for line
			inLine.CopyToV0(inVert);
			inLine.CopyToV1(nextVert);
			//Clip line
			Result = ClipLineToFarPlane(inLine, outLine, ClipPlane);
			//Switch clip result(NOTE:All corresponding cases are
			//commented above)
			switch(Result){
			case CLIPPED_V0:
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
				}else{
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
				};break;
			case CLIPPED_V1:
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
				}else{
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
				};break;
			case CLIPPED_NONE:
				if ((LastResult != CLIPPED_NONE)&&(LastResult != CLIPPED_V0)){
					outLine.CopyV0To(outVert);
					outLine.CopyV1To(outVert+1);
					new_verts+=2;
					outVert+=2;
				}else{
					outLine.CopyV1To(outVert);
					new_verts++;
					outVert++;
				};break;
			case CLIPPED_ALL:
				number_times_both_rejected++;
				if (number_times_both_rejected > number_total_points){
					finished_error = true;
					finish = true;
				};break;
			};
			//Increment loop
			j++;
			//Get next loop vars.
			LastResult = Result;
			inVert = nextVert;
			//If all verts are processed, we are done
			if (new_verts >= number_total_points)finish = true;
		};
	};
//	DEBUG("Clip Loop finished...");
	if (!finished_error){
		//Set number new vertices to number added vertices
		number_clipped_vertices = number_total_points;
		//Increment number planes tested
		number_planes_tested++;
		//Copy vertices
		for (j = 0; j < number_clipped_vertices; j++)
			ClippedVertices[j] = tempClippingVertices[j];
		//If number vertices left do not make a triangle, then return false
		if (number_clipped_vertices < 3)rt = false;
	}else{
		//All points were clipped
		rt = false;
	};
	return(rt);
}
/***********************************************************/
/*bool ObjectPolygon::ClipToFrustrum(Plane *FrustrumPlanes){
	int j=0;
	bool rt=true;
	DEBUG("Clipping Polygon to Frustrum");
	//Near Plane
	if ( ((ClippedZPlanes & NEAR_CLIPPING_PLANE)>>NEAR_SHIFT) > 0)
		rt = ClipToPlane(FrustrumPlanes[NEAR_INDEX], NEAR_INDEX);
	//Far Plane
	if (( ((ClippedZPlanes & FAR_CLIPPING_PLANE)>>FAR_SHIFT) > 0) && (rt))
		rt = ClipToFarPlane(FrustrumPlanes[FAR_INDEX]);
	//Left Plane
	if (( ((ClippedBoundingPlanes & LEFT_CLIPPING_PLANE)>>LEFT_SHIFT) > 0) && (rt))
		rt = ClipToPlane(FrustrumPlanes[LEFT_INDEX], LEFT_INDEX);
	//Right Plane
	if (( ((ClippedBoundingPlanes & RIGHT_CLIPPING_PLANE)>>RIGHT_SHIFT) > 0) && (rt))
		rt = ClipToPlane(FrustrumPlanes[RIGHT_INDEX], RIGHT_INDEX);
	//Top Plane
	if (( ((ClippedBoundingPlanes & TOP_CLIPPING_PLANE)>>TOP_SHIFT) > 0) && (rt))
		rt = ClipToPlane(FrustrumPlanes[TOP_INDEX], TOP_INDEX);
	//Bottom Plane
	if (( ((ClippedBoundingPlanes & BOTTOM_CLIPPING_PLANE)>>BOTTOM_SHIFT) > 0) && (rt))
		rt = ClipToPlane(FrustrumPlanes[BOTTOM_INDEX], BOTTOM_INDEX);
	if (!rt)fully_clipped = true;

	return(rt);
}
/***********************************************************/
//Update/Render functions
/***********************************************************/
void ObjectPolygon::Cull(Camera &Cam){
	Vector viewvec;	//View vector
	int index = VertexIndex[0];
	Vertex *vertex = &Vertices[index];

//	DEBUG("Culling polygon to camera");

	//Reset visibility
	culled = true;

	if (!double_sided)
	{
		//Get view vector
		viewvec.x = vertex->x - Cam.x;
		viewvec.y = vertex->y - Cam.y;
		viewvec.z = vertex->z - Cam.z;

		//Do vis test
		//if (DotProduct(viewvec, FaceNormal) < -0.01)
		if ( (viewvec.x*FaceNormal.x + viewvec.y*FaceNormal.y + viewvec.z*FaceNormal.z) < -0.01)
		{
			culled = false;
			if (number_vertices == 3){
				Vertices[VertexIndex[0]].part_of_visible_polygon = true;
				Vertices[VertexIndex[1]].part_of_visible_polygon = true;
				Vertices[VertexIndex[2]].part_of_visible_polygon = true;
			}else if (number_vertices == 4){
				Vertices[VertexIndex[0]].part_of_visible_polygon = true;
				Vertices[VertexIndex[1]].part_of_visible_polygon = true;
				Vertices[VertexIndex[2]].part_of_visible_polygon = true;
				Vertices[VertexIndex[3]].part_of_visible_polygon = true;
			}else{
				for (int j = 0; j < number_vertices; j++)
					Vertices[VertexIndex[j]].part_of_visible_polygon = true;
			};
		};
	}else
	{
		culled = false;
		if (number_vertices == 3){
			Vertices[VertexIndex[0]].part_of_visible_polygon = true;
			Vertices[VertexIndex[1]].part_of_visible_polygon = true;
			Vertices[VertexIndex[2]].part_of_visible_polygon = true;
		}else if (number_vertices == 4){
			Vertices[VertexIndex[0]].part_of_visible_polygon = true;
			Vertices[VertexIndex[1]].part_of_visible_polygon = true;
			Vertices[VertexIndex[2]].part_of_visible_polygon = true;
			Vertices[VertexIndex[3]].part_of_visible_polygon = true;
		}else{
			for (int j = 0; j < number_vertices; j++)
				Vertices[VertexIndex[j]].part_of_visible_polygon = true;
		};
	};
}
/***********************************************************/
//NOTE:will only do flat shading for the polygon,
//All other shading is point shading and should be
//done at the object level
void ObjectPolygon::LightPolygon(Light *LightArray, int &number_lights){
	int j;
	float n_dot_l, length, reflect, dist_affect;
	int r, g, b;
	Light *L_Ptr=&LightArray[0];
	Color *C=&L_Ptr->LightColor;
	Vector Light_Vec;
	Vertex *v = &Vertices[VertexIndex[0]];
	FaceColor = OriginalFaceColor;

	if (SHADING == PolyShadingFlat){
		for (j = 0; j < number_lights; j++){
			C = &L_Ptr->LightColor;
			switch(L_Ptr->TYPE){
			case LightAmbient :
				//Just basic ambient lighting
				FaceColor.SetR(FaceColor.GetR() + C->GetR());
				FaceColor.SetG(FaceColor.GetG() + C->GetG());
				FaceColor.SetB(FaceColor.GetB() + C->GetB());
				break;
			case LightPoint :
				//Distance falloff point lighting
				Light_Vec.x = L_Ptr->x - v->x;
				Light_Vec.y = L_Ptr->y - v->y;
				Light_Vec.z = L_Ptr->z - v->z;

				length = Light_Vec.Normalize();
				n_dot_l = DotProduct(Light_Vec, FaceNormal);

				//Test lighting code
				reflect = n_dot_l / length;
				if (reflect < 0)reflect = -reflect;
				reflect = (float)((reflect + 1.0)*0.5);
				dist_affect = L_Ptr->falloff_distance * length;
				r = (int)(reflect*C->GetR() - dist_affect);
				g = (int)(reflect*C->GetG() - dist_affect);
				b = (int)(reflect*C->GetB() - dist_affect);

				FaceColor.SetR(FaceColor.GetR() + (int)(r));
				FaceColor.SetG(FaceColor.GetG() + (int)(g));
				FaceColor.SetB(FaceColor.GetB() + (int)(b));
				break;
			case LightInfinitePoint:
				//Distance falloff point lighting
				Light_Vec.x = L_Ptr->x - v->x;
				Light_Vec.y = L_Ptr->y - v->y;
				Light_Vec.z = L_Ptr->z - v->z;

				Light_Vec.Normalize();
				n_dot_l = DotProduct(Light_Vec, FaceNormal);
				FaceColor.SetR(FaceColor.GetR() + (int)(n_dot_l * C->GetR()));
				FaceColor.SetG(FaceColor.GetG() + (int)(n_dot_l * C->GetG()));
				FaceColor.SetB(FaceColor.GetB() + (int)(n_dot_l * C->GetB()));
				break;
			default : break;
			};

			//Increment light pointer
			L_Ptr++;
		};
	};
}
/***********************************************************/
void ObjectPolygon::ObjectPolygon_DEBUG(){
	int j;

	DEBUG("*****ObjectPolygon Class*****");

	//Variable information
	DEBUGf("# of Vertices = %d", number_vertices);
	DEBUGf("# of Clipped Vertices = %d", number_clipped_vertices);
	DEBUGf("Shading = %d", SHADING);
	DEBUGf("Face = %d", FACE);
	DEBUGf("Medium = %d", MEDIUM);
	DEBUGf("Texture Index = %d", texture_index);
	DEBUGf("ClippedBoundingPlanes = %x", ClippedBoundingPlanes);
	DEBUGf("Left,Right,Top,Bottom=%d,%d,%d,%d",
		   (ClippedBoundingPlanes & LEFT_CLIPPING_PLANE) >> LEFT_SHIFT,
		   (ClippedBoundingPlanes & RIGHT_CLIPPING_PLANE) >> RIGHT_SHIFT,
		   (ClippedBoundingPlanes & TOP_CLIPPING_PLANE) >> TOP_SHIFT,
		   (ClippedBoundingPlanes & BOTTOM_CLIPPING_PLANE) >> BOTTOM_SHIFT);
	DEBUGf("ClippedZPlanes = %x", ClippedZPlanes);
	DEBUGf("Near,Far=%d,%d",
		   (ClippedZPlanes & NEAR_CLIPPING_PLANE) >> NEAR_SHIFT,
		   (ClippedZPlanes & FAR_CLIPPING_PLANE) >> FAR_SHIFT);
	DEBUGf("Number Planes Tested = %d", number_planes_tested);
	DEBUGf("Fully Clipped = %d", fully_clipped);
	DEBUGf("Partially Clipped = %d", partially_clipped);
	DEBUGf("Fully Visible = %d", fully_visible);
	DEBUGf("Culled = %d", culled);
	DEBUGf("Double Sided = %d", double_sided);

	//Color information
	DEBUG("Color:");
	FaceColor.Color_DEBUG();
	DEBUG("Original Color:");
	OriginalFaceColor.Color_DEBUG();

	//Vector information
	DEBUG("Face Normal:");
	FaceNormal.Vector_DEBUG();

	//Array information
	DEBUG("Vertices:");
	for (j = 0; j < number_vertices; j++){
		DEBUGf("Vertex (#%d)->Index=%d:", j, VertexIndex[j]);
		Vertex_DEBUG(Vertices[VertexIndex[j]]);
		DEBUG("UV:");
		DEBUGf("#%d=%.2f, %.2f", UV_Coords[j<<1], UV_Coords[(j<<1)+1]);
	};
	if (partially_clipped){
		DEBUG("Clipped Vertices:");
		for (j = 0; j < number_clipped_vertices; j++){
			DEBUGf("C Vertex(#%d):", j);
			Vertex_DEBUG(ClippedVertices[j]);
		};
	};

	DEBUG("***End ObjectPolygon Class***");
}
