#ifndef OBJECTPOLYGON_H
#define OBJECTPOLYGON_H
#include <Vertex.h>

class ObjectPolygon{
public:
	//Vertex information
	Vertex *Vertices;    //Pointer to object vertice array
	Vertex *ClippedVertices;  //Array of clipped vertices
	int *VertexIndex;	//Array of vertex indices for the vertice array
	float *UV_Coords;	//Array of texture coordinates
	int number_vertices;	//Total number of original vertices
	int number_clipped_vertices; //Total number of clipped vertices

	//Polygon type information
	PolyShadingType SHADING;
	PolyFaceType FACE;
	PolyMediumType MEDIUM;
	LastStateType LAST;
	Color FaceColor, OriginalFaceColor;
	Vector FaceNormal;
	int texture_index;

	//Clipping information
	unsigned int ClippedBoundingPlanes;	//Up, Down, Left, Right Planes
	unsigned int ClippedZPlanes;         //Far Near Planes
	int number_planes_tested;	//# of planes the poly has been tested

	//Boolean values
	bool fully_clipped, partially_clipped, fully_visible;
	bool culled;
	bool double_sided;

public:
	//Constructor/Destructor
	ObjectPolygon();
	~ObjectPolygon();

	//Init functions
	void InitPolygon(int num_vertices, Vertex *V_List, int *V_Index, float *UV_List,
					 PolyShadingType S, PolyFaceType F, PolyMediumType M,
					 Color col, int text_index);
	void CalculateNormal(Vertex *LocalVertices);

	//Set functions
	void SetDoubleSided(bool ds){double_sided = ds;};
	void SetFaceColor(Color col){OriginalFaceColor = col;FaceColor = col;};

	//Clip functions
	void ClipTest(void);
//	int ClipLineToPlane(ClippingLine &in, ClippingLine &out, Plane &ClipPlane);
//	int ClipLineToFarPlane(ClippingLine &in, ClippingLine &out, Plane &ClipPlane);
//	bool ClipToPlane(Plane &ClipPlane, int plane_index);
//	bool ClipToFarPlane(Plane &ClipPlane);
//	bool ClipToFrustrum(Plane *FrustrumPlanes);

	//Update/Vis/Light functions
	void Cull(Camera &Cam);
	void LightPolygon(Light *LightArray, int &number_lights);

	void ObjectPolygon_DEBUG();
};

#endif // OBJECTPOLYGON_H
