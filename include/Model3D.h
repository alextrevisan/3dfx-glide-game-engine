#ifndef MODEL3D_H
#define MODEL3D_H
#include <math.h>

#include <glide.h>
#include <Vertex.h>
#include <ObjectPolygon.h>



class Vector{
public:
	float x, y, z;  //Coordinates and 1/z variable
public:
	Vector(){x=y=z=0.00;};
	~Vector(){x=y=z=0.00;};

	void Set(float xp, float yp, float zp){x=xp;y=yp;z=zp;};
	float Length(void){return(std::sqrt(x*x + y*y + z*z));};
	float Normalize(void);
	void MakeVector(Vector &Init, Vector &Term);
	void MakeVector(Vector &Term);
	void MakeVector(Vertex &Init, Vertex &Term);
	void MakeVector(Vertex &Term);
	void Vector_DEBUG(){printf("*****Vector Class*****\n");
					    printf("XYZ->%.2f,%.2f,%.2f\n", x, y, z);
						printf("***End Vector Class***\n");
	}
};

class Model3D : public Vector{
public:
	//Vertex information
	Vertex *LocalVertices;
	Vertex *TransVertices;
	Vector *VertexNormals;
	int number_vertices;

	//Face information
	ObjectPolygon *Faces;
	int number_faces;

	//Object information
	float radius;
	Vector angle, prev_angle;
	Vector scale, prev_scale;
	Vector rotate;
	PolyShadingType SHADING;
	PolyFaceType FACE;
	PolyMediumType MEDIUM;
	Color ObjectColor;
	int texture_index;
	bool rotate_object;

	//Clipping information
	bool fully_visible, partially_clipped;
	bool culled;
public:
	//Constructor/Destructor
	Model3D();
	~Model3D();

	//Destroy functions
	void Destroy(void);

	//Init functions***************************************
	void Init(int numb_points, int numb_faces,
		      Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
			  PolyShadingType S, PolyFaceType F, PolyMediumType M,
			  Color ObjColor, int text_index);
	//**************
	//Cube
	void Cube(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
		      PolyShadingType S, PolyFaceType F, PolyMediumType M,
			  Color ObjColor, int text_index, bool double_sided);
	//**************
	//Pyramid
	void Pyramid(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
		         PolyShadingType S, PolyFaceType F, PolyMediumType M,
			     Color ObjColor, int text_index, bool double_sided);
	//**************
	//Cylinder
	void Cylinder(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
			      int number_divs,
				  PolyShadingType S, PolyFaceType F, PolyMediumType M,
				  Color ObjColor, int text_index, bool double_sided);
	//**************
	//Sphere
	void Sphere(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
			    int number_divs,
				PolyShadingType S, PolyFaceType F, PolyMediumType M,
				Color ObjColor, int text_index, bool double_sided);
	//**************
	//Cone
	void Cone(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
			  int number_divs,
			  PolyShadingType S, PolyFaceType F, PolyMediumType M,
			  Color ObjColor, int text_index, bool double_sided);
	//**************
	//Loads from an ASC file
	void LoadFromASCFile(char *filename, Vector *Pos,
		                 Vector *Sca, Vector *Ang, Vector *Rot,
						 PolyShadingType S, PolyFaceType F,
						 PolyMediumType M, Color ObjColor,
						 int text_index, bool double_sided);
	//void LoadFromDS2File(
	void LoadFrom3DSFile(char *filename, Vector *Pos,
		                 Vector *Sca, Vector *Ang, Vector *Rot,
						 PolyShadingType S, PolyFaceType F,
						 PolyMediumType M, Color ObjColor,
						 int text_index, bool double_sided);


	//***Object functions***
		//Input
		void HandleInput(DS2_Input &Input);

	   //Transforms
	   void CenterObject(void);
	   void TransformLocalToWorld(void);
	   void TransformWorldToCamera(Camera &Cam);

	   //Clipping/Culling
	   bool ClipObject(Camera &Cam);
	   void ClipObjectPolys(Plane *FrustrumPlanes);
	   void CullBackFaces(Camera &Cam);

	   //Lighting
	   void LightObject(Light *LightArray, int &number_lights);

	   //Precalculate Functions
	   void GetObjectRadius(void);
	   void CalculateNormals(void);

	   //Rotate
	   void RotateXYZ(short x_ang, short y_ang, short z_ang);
	   void UpdateRotations(void);

	   //Render/Project
	   void ProjectAndRender(void);
	//************************************************
	void Object_DEBUG();
};

#endif // MODEL3D_H
