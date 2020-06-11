#include "Model3D.h"

Model3D::Model3D(){
	//Reset all data
	LocalVertices = NULL;
	TransVertices = NULL;
	Faces = NULL;
	VertexNormals = NULL;
	number_vertices = number_faces = 0;
	radius = 0.00;
	SHADING = PolyShadingFlat;
	FACE = PolyFacePlain;
	MEDIUM = PolyMediumSolid;
	texture_index = NO_TEXTURE_INDEX;
	rotate_object = false;
	fully_visible = partially_clipped = culled = false;
}
/***********************************************************/
Model3D::~Model3D(){
	Destroy();
}
/***********************************************************/
void Model3D::Destroy(void){
	//Delete all arrays
	if (Faces)delete [] Faces;
	Faces = NULL;
//	MEMORY.DestroyMemory((void **)&Faces);
	if (VertexNormals)delete[]VertexNormals;
	VertexNormals = NULL;
//	MEMORY.DestroyMemory((void **)&VertexNormals);
	if (LocalVertices)delete [] LocalVertices;
	LocalVertices = NULL;
//	MEMORY.DestroyMemory((void **)&LocalVertices);
	if (TransVertices)delete [] TransVertices;
	TransVertices = NULL;
//	MEMORY.DestroyMemory((void **)&TransVertices);
}
/***********************************************************/
//Init functions***************************************
/***********************************************************/
void Model3D::Init(int numb_points, int numb_faces,
	      Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
		  PolyShadingType S, PolyFaceType F, PolyMediumType M,
		  Color ObjColor, int text_index){
	int j;

	//Get data
	number_vertices = numb_points;
	number_faces = numb_faces;
	SHADING = S;
	FACE = F;
	MEDIUM = M;
	angle = *Ang;
	prev_angle = angle;
	scale = *Sca;
	prev_scale = scale;
	if (Rot){
		rotate = *Rot;
		rotate_object = true;
	};
	texture_index = text_index;
	ObjectColor = ObjColor;

	//Get position
	x = Pos->x;
	y = Pos->y;
	z = Pos->z;

	//Allocate data arrays
	Destroy();
	LocalVertices = new Vertex[number_vertices];
//	LocalVertices = (Vertex *)MEMORY.AllocateMemory(sizeof(Vertex), numb_points);
	TransVertices = new Vertex[number_vertices];
//	TransVertices = (Vertex *)MEMORY.AllocateMemory(sizeof(Vertex), numb_points);
	Faces = new ObjectPolygon[number_faces];
//	Faces = (ObjectPolygon *)MEMORY.AllocateMemory(sizeof(ObjectPolygon), number_faces);
	VertexNormals = new Vector[number_vertices];
//	VertexNormals = (Vector *)MEMORY.AllocateMemory(sizeof(Vector), numb_points);


	//Set colors for local/trans vertices
	for (j = 0; j < numb_points; j++){
		LocalVertices[j].color = ObjectColor.color;
	};
}

/***********************************************************/
//Cube
void Model3D::Cube(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
	      PolyShadingType S, PolyFaceType F, PolyMediumType M,
		  Color ObjColor, int text_index, bool double_sided){
	Vertex *v=NULL, *t=NULL;
	ObjectPolygon *f=NULL;
	int j;
	int VertexI[4];
	float UV_List[8];

	//Call general initialization function
	Init(8, 6,
		 Pos, Sca, Ang, Rot,
		 S, F, M,
		 ObjColor, text_index);

	//Setup local vertices
	v = &LocalVertices[0];
	t = &TransVertices[0];
	//V#0
	v->x = -1;
	v->y = 1;
	v->z = -1;
	v++;
	//V#1
	v->x = 1;
	v->y = 1;
	v->z = -1;
	v++;
	//V#2
	v->x = 1;
	v->y = -1;
	v->z = -1;
	v++;
	//V#3
	v->x = -1;
	v->y = -1;
	v->z = -1;
	v++;
	//V#4
	v->x = -1;
	v->y = 1;
	v->z = 1;
	v++;
	//V#5
	v->x = 1;
	v->y = 1;
	v->z = 1;
	v++;
	//V#6
	v->x = 1;
	v->y = -1;
	v->z = 1;
	v++;
	//V#7
	v->x = -1;
	v->y = -1;
	v->z = 1;
	v++;
	//End local vertice setup

	//Scale local vertices
	for (j = 0; j < number_vertices; j++){
		LocalVertices[j].x *= scale.x;
		LocalVertices[j].y *= scale.y;
		LocalVertices[j].z *= scale.z;
	};

	//Setup faces
	f = &Faces[0];

	//Setup UV_List
	UV_List[0] = 0.0;
	UV_List[1] = 0.0;

	UV_List[2] = 1.0;
	UV_List[3] = 0.0;

	UV_List[4] = 1.0;
	UV_List[5] = 1.0;

	UV_List[6] = 0.0;
	UV_List[7] = 1.0;

	//Face 0
	VertexI[0]=0;VertexI[1]=1;VertexI[2]=2;VertexI[3]=3;
	f->InitPolygon(4, TransVertices, VertexI, UV_List,
		           SHADING, FACE, MEDIUM, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//Face 1
	VertexI[0]=1;VertexI[1]=5;VertexI[2]=6;VertexI[3]=2;
	f->InitPolygon(4, TransVertices, VertexI, UV_List,
		           SHADING, FACE, MEDIUM, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//Face 2
	VertexI[0]=4;VertexI[1]=0;VertexI[2]=3;VertexI[3]=7;
	f->InitPolygon(4, TransVertices, VertexI, UV_List,
		           SHADING, FACE, MEDIUM, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//Face 3
	VertexI[0]=4;VertexI[1]=5;VertexI[2]=1;VertexI[3]=0;
	f->InitPolygon(4, TransVertices, VertexI, UV_List,
		           SHADING, FACE, MEDIUM, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//Face 4
	VertexI[0]=3;VertexI[1]=2;VertexI[2]=6;VertexI[3]=7;
	f->InitPolygon(4, TransVertices, VertexI, UV_List,
		           SHADING, FACE, MEDIUM, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//Face 5
	VertexI[0]=5;VertexI[1]=4;VertexI[2]=7;VertexI[3]=6;
	f->InitPolygon(4, TransVertices, VertexI, UV_List,
		           SHADING, FACE, MEDIUM, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;

	//Do precalculated stuff
	GetObjectRadius();
	CalculateNormals();

	//Rotate object
	RotateXYZ((short)angle.x, (short)angle.y, (short)angle.z);

}

/***********************************************************/
//Pyramid
void Model3D::Pyramid(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
	         PolyShadingType S, PolyFaceType F, PolyMediumType M,
			 Color ObjColor, int text_index, bool double_sided){
	Vertex *v=NULL, *t=NULL;
	ObjectPolygon *f=NULL;
	int j;
	int V_Index[4];
	float uv_index[8];

	//Call general initialization function
	Init(5, 5,
		 Pos, Sca, Ang, Rot,
		 S, F, M,
		 ObjColor, text_index);

	//Setup local vertices
	v = &LocalVertices[0];
	t = &TransVertices[0];

	//Setup XYZ
	//***Vertice #0***//
	v->x = 0.00;
	v->y = 1.00;
	v->z = 0.00;
	v++;
	//***Vertice #1***//
	v->x = 1.00;
	v->y = -1.00;
	v->z = -1.00;
	v++;
	//***Vertice #2***//
	v->x = -1.00;
	v->y = -1.00;
	v->z = -1.00;
	v++;
	//***Vertice #3***//
	v->x = -1.00;
	v->y = -1.00;
	v->z = 1.00;
	v++;
	//***Vertice #4***//
	v->x = 1.00;
	v->y = -1.00;
	v->z = 1.00;

	//Scale local vertices
	for (j = 0; j < number_vertices; j++){
		LocalVertices[j].x *= scale.x;
		LocalVertices[j].y *= scale.y;
		LocalVertices[j].z *= scale.z;
	};

	//Get face pointer
	f = &Faces[0];

	//Setup UV
	uv_index[0] = 0.50;uv_index[1] = 0.00;
	uv_index[2] = 1.00;uv_index[3] = 1.00;
	uv_index[4] = 0.00;uv_index[5] = 1.00;

	//Setup faces
	//*************Face #0***************//
	V_Index[0]=0;V_Index[1]=1;V_Index[2]=2;
	f->InitPolygon(3, TransVertices, V_Index, uv_index,
		           S, F, M, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//*************Face #1***************//
	V_Index[0]=0;V_Index[1]=4;V_Index[2]=1;
	f->InitPolygon(3, TransVertices, V_Index, uv_index,
		           S, F, M, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//*************Face #2***************//
	V_Index[0]=0;V_Index[1]=3;V_Index[2]=4;
	f->InitPolygon(3, TransVertices, V_Index, uv_index,
		           S, F, M, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//*************Face #3***************//
	V_Index[0]=0;V_Index[1]=2;V_Index[2]=3;
	f->InitPolygon(3, TransVertices, V_Index, uv_index,
		           S, F, M, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);
	f++;
	//*************Face #4***************//
	//Setup UV Coords
	uv_index[0] = 0.00;uv_index[1] = 0.00;
	uv_index[2] = 1.00;uv_index[3] = 0.00;
	uv_index[4] = 1.00;uv_index[5] = 1.00;
	uv_index[6] = 0.00;uv_index[7] = 1.00;

	V_Index[0]=2;V_Index[1]=1;V_Index[2]=4;V_Index[3]=3;
	f->InitPolygon(4, TransVertices, V_Index, uv_index,
		           S, F, M, ObjectColor, text_index);
	f->SetDoubleSided(double_sided);

	//Do precalculated stuff
	GetObjectRadius();
	CalculateNormals();

	//Rotate object
	RotateXYZ((short)angle.x, (short)angle.y, (short)angle.z);

}
/***********************************************************/
//Cylinder
void Model3D::Cylinder(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
		      int number_divs,
			  PolyShadingType S, PolyFaceType F, PolyMediumType M,
			  Color ObjColor, int text_index, bool double_sided){
	int j=0,k=0;
	int V_Index[500];
	float uv_index[1000];
	Vertex *v;
	ObjectPolygon *f;
	int number_divisions = number_divs;
	int number_points;
	float angle_division=0.00;
	float current_angle=0.00;

	//Check divisions
	if (number_divisions < 10)number_divisions = 10;
	if (number_divisions % 2 != 0)number_divisions--;
	angle_division = (float)(360.00 / (float)number_divisions);

	//Create the points
	number_points = (number_divisions*2)+2;
	number_faces = (2*(number_divisions)) + number_divisions;

	//Init the object
	Init(number_points, number_faces,
		 Pos, Sca, Ang, Rot,
		 S, F, M,
		 ObjColor, text_index);

	//Setup position data
	v = &LocalVertices[0];
	for (j = 0; j < number_divisions; j++){
		//Do two points at a time(top/bottom)
		//Top
		v->x = MathPtr->cos_table[(int)current_angle] * scale.x;
		v->y = scale.y;
		v->z = MathPtr->sin_table[(int)current_angle] * scale.z;
		v++;
		*v = *(v-1);

		//Bottom
		v->y = -scale.y;
		v++;

		//Increment current angle
		current_angle += angle_division;
		if (current_angle > 360)current_angle = 360;
	};
	v->x = 0;
	v->y = scale.y;
	v->z = 0;
	v++;

	v->x = 0;
	v->y = -scale.y;
	v->z = 0;

	//Setup polygon faces
	f = &Faces[0];

	//Do top bottom faces first
/*	//Setup vindex
	V_Index[0] = 0;
	V_Index[1] = number_points-2;
	k = 2;
	for (j = number_points-4; j >= 0; j-=2){
		V_Index[k] = j;
		k++;
	};
	//Setup uvIndex
	for (j = 0; j < number_divisions*2; j++)uv_index[j] = 0.00;

	//Top
	f->InitPolygon(number_divisions, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, text_index);
	f++;
	k = 0;
	for (j = 1; j < number_points; j+=2){
		V_Index[k] = j;
		k++;
	};
	//Bottom
	f->InitPolygon(number_divisions, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, text_index);
	f++;*/
	//Top faces
	V_Index[0] = number_divisions*2;
	uv_index[0] = 0;
	uv_index[1] = 0;
	uv_index[2] = 0;
	uv_index[3] = 0;
	uv_index[4] = 0;
	uv_index[5] = 0;
	for (j = 0; j < number_divisions-1; j++)
	{
		V_Index[1] = (j+1)<<1;
		V_Index[2] = (j)<<1;
		f->InitPolygon(3, TransVertices, V_Index, uv_index, S, PolyFacePlain, M, ObjectColor, NO_TEXTURE_INDEX);
		f++;
	};
	V_Index[1] = 0;
	V_Index[2] = (number_divisions-1)<<1;
	f->InitPolygon(3, TransVertices, V_Index, uv_index, S, PolyFacePlain, M, ObjectColor, NO_TEXTURE_INDEX);
	f++;

	//Bottom faces
	V_Index[0] = (number_divisions*2)+1;
	for (j = 0; j < number_divisions-1; j++)
	{
		V_Index[1] = ((j)<<1)+1;
		V_Index[2] = ((j+1)<<1)+1;
		f->InitPolygon(3, TransVertices, V_Index, uv_index, S, PolyFacePlain, M, ObjectColor, NO_TEXTURE_INDEX);
		f++;
	};
	V_Index[1] = ((number_divisions-1)<<1)+1;
	V_Index[2] = 1;
	f->InitPolygon(3, TransVertices, V_Index, uv_index, S, PolyFacePlain, M, ObjectColor, NO_TEXTURE_INDEX);
	f++;

	//Polys around the cylinder
	current_angle = 0.00;
	angle_division = (float)(90.00 / (float)(number_divisions+1));
	uv_index[1] = uv_index[3] = 0.00;
	uv_index[5] = uv_index[7] = 1.00;
	for (j = 0; j < number_divisions-1; j++){
		//Get VIndex
		V_Index[0] = j<<1;
		V_Index[1] = ((j+1)<<1);
		V_Index[2] = ((j+1)<<1)+1;
		V_Index[3] = (j<<1)+1;

		//Get uv_index
		uv_index[0] = MathPtr->sin_table[(int)current_angle];
		uv_index[2] = MathPtr->sin_table[(int)(current_angle + angle_division)];
		uv_index[4] = uv_index[2];
		uv_index[6] = uv_index[0];

		f->InitPolygon(4, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, text_index);
		f++;
		current_angle += angle_division;
	};

	//Setup last face
	V_Index[0] = (number_divisions-1)<<1;
	V_Index[1] = 0;
	V_Index[2] = 1;
	V_Index[3] = ((number_divisions-1)<<1)+1;

	uv_index[0] = MathPtr->sin_table[(int)(90.00 - angle_division)];
	uv_index[2] = 1.0;
	uv_index[4] = 1.0;
	uv_index[6] = uv_index[0];
	f->InitPolygon(4, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, text_index);

	//Get object radius
	GetObjectRadius();
	CalculateNormals();

	//Rotate object
	RotateXYZ((short)angle.x, (short)angle.y, (short)angle.z);
}

/***********************************************************/
//Sphere
void Model3D::Sphere(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
		    int number_divs,
			PolyShadingType S, PolyFaceType F, PolyMediumType M,
			Color ObjColor, int text_index, bool double_sided){
	int j=0,k=0;
	int V_Index[4];
	float uv_index[8];
	Vertex *v;
	ObjectPolygon *f;
	int number_divisions = number_divs;
	float angle_division=0.00, angle_division2=0.00;
	float current_angle1=0.00, current_angle2=0.00;
	int start_index=0, next_index=0;
	int start2_index=0, next2_index=0;
	int number_points=0, t_i = text_index;

	//Check divisions
	if (number_divisions < 10)number_divisions = 10;
	if (number_divisions % 2 != 0)number_divisions--;
	angle_division = (float)(360.00 / (float)number_divisions);
	angle_division2 = (float)(180.00 / (float)number_divisions);

	//Number points
	number_points = 2 + ((number_divisions)*(number_divisions-1));
	number_faces = number_divisions * number_divisions;

	//Create points
//	verticesLocal = new dsePoint[number_points];
//	verticesTrans = new dsePoint[number_points];
	Init(number_points, number_faces,
		 Pos, Sca, Ang, Rot,
		 S, F, M,
		 ObjColor, text_index);

	//Create top/bottom points
	v = &LocalVertices[0];
	v->x = 0.00;
	v->y = scale.y;
	v->z = 0.00;
	v++;

	v->x = 0.00;
	v->y = -scale.y;
	v->z = 0.00;
	v++;

	//Create rest of points
	current_angle2 = angle_division2;
	for (j = 0; j < number_divisions-1; j++){
		current_angle1 = 0.00;
		for (k = 0; k < number_divisions; k++){
			v->x = MathPtr->cos_table[(int)current_angle1] *
				   MathPtr->sin_table[(int)current_angle2] *
				   scale.x;
			v->y = MathPtr->cos_table[(int)current_angle2] * scale.y;
			v->z = MathPtr->sin_table[(int)current_angle1] *
				   MathPtr->sin_table[(int)current_angle2] *
				   scale.z;
//			v->r = r;
//			v->g = g;
//			v->b = b;
//			v->a = (float)alpha_level;
			v++;

			current_angle1 += angle_division;
			if (current_angle1 > 360)current_angle1 = 360;
		};
		current_angle2 += angle_division2;
		if (current_angle2 > 180)current_angle2 = 180;
	};

	//Setup faces
//	faces = new dsePolygon[number_faces];
	f = &Faces[0];

	//Setup top row
	V_Index[0] = 0;
	uv_index[0] = 0.5;
	uv_index[1] = 0.0;
	start_index = 2;
	next_index = 3;
	angle_division2 = (float)(90.00 / (float)(number_divisions+1));
	current_angle2 = angle_division2;
	current_angle1 = 0.00;
	for (j = 0; j < number_divisions-1; j++){
		V_Index[1] = next_index;
		V_Index[2] = next_index-1;

		uv_index[2] = MathPtr->sin_table[(int)(current_angle1 + angle_division2)];
		uv_index[3] = MathPtr->sin_table[(int)current_angle2];
		uv_index[4] = MathPtr->sin_table[(int)current_angle1];
		uv_index[5] = uv_index[3];

   	    f->InitPolygon(3, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, t_i);
		f->SetDoubleSided(double_sided);
		f++;
		current_angle1 += angle_division2;
//		start_index = next_index;
		next_index++;
	};
	uv_index[2] = MathPtr->sin_table[90];
	uv_index[4] = MathPtr->sin_table[(int)(90.00 - angle_division2)];
	V_Index[1] = start_index;
	V_Index[2] = next_index-1;
	f->InitPolygon(3, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, t_i);
	f->SetDoubleSided(double_sided);
	f++;

	//Middle sphere
	for (j = 0; j < number_divisions-2; j++){
		start2_index = start_index;
		next2_index = start_index + 1;
		start_index += number_divisions;
		next_index = start_index+1;
		current_angle2 += angle_division2;
		current_angle1 = 0.00;

		for (k = 0; k < number_divisions-1; k++){
			V_Index[0] = next2_index-1;
			V_Index[1] = next2_index;
			V_Index[2] = next_index;
			V_Index[3] = next_index-1;

			uv_index[0] = MathPtr->sin_table[(int)current_angle1];
			uv_index[1] = MathPtr->sin_table[(int)(current_angle2 - angle_division2)];
			uv_index[2] = MathPtr->sin_table[(int)(current_angle1 + angle_division2)];
			uv_index[3] = uv_index[1];
			uv_index[4] = uv_index[2];
			uv_index[5] = MathPtr->sin_table[(int)current_angle2];
			uv_index[6] = uv_index[0];
			uv_index[7] = uv_index[5];
			f->InitPolygon(4, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, t_i);
			f->SetDoubleSided(double_sided);
			f++;
			next_index++;
			next2_index++;
			current_angle1 += angle_division2;
		};

		V_Index[0] = next2_index-1;
		V_Index[1] = start2_index;
		V_Index[2] = start_index;
		V_Index[3] = next_index-1;

		uv_index[0] = uv_index[2];
		uv_index[1] = uv_index[3];
		uv_index[2] = MathPtr->sin_table[90];
		uv_index[3] = uv_index[1];
		uv_index[6] = uv_index[4];
		uv_index[4] = uv_index[2];
		f->InitPolygon(4, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, t_i);
		f->SetDoubleSided(double_sided);
		f++;
	};

	start2_index = start_index;
	next2_index = start_index + 1;
	current_angle2 = 90.00;
	current_angle1 = 0.00;
	V_Index[2] = 1;
	uv_index[4] = 0.5;
	uv_index[5] = 1.0;

	for (j = 0; j < number_divisions-1; j++){
		V_Index[0] = next2_index-1;
		V_Index[1] = next2_index;

		uv_index[0] = MathPtr->sin_table[(int)current_angle1];
		uv_index[1] = MathPtr->sin_table[(int)(current_angle2 - angle_division2)];
		uv_index[2] = MathPtr->sin_table[(int)(current_angle1 + angle_division2)];
		uv_index[3] = uv_index[1];

		f->InitPolygon(3, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, t_i);
		f->SetDoubleSided(double_sided);
		f++;
		current_angle1 += angle_division2;
		next2_index++;
	};
	uv_index[2] = MathPtr->sin_table[90];
	uv_index[0] = MathPtr->sin_table[(int)(90.00 - angle_division2)];
	V_Index[1] = start2_index;
	V_Index[0] = next2_index-1;
	f->InitPolygon(3, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, t_i);
	f->SetDoubleSided(double_sided);
	f++;

	//Do precalculated stuff
	GetObjectRadius();
	CalculateNormals();

	//Rotate object
	RotateXYZ((short)angle.x, (short)angle.y, (short)angle.z);
}

/***********************************************************/
//Cone
//void Model3D::Cone(Vector *Pos, Vector *Sca, Vector *Ang, Vector *Rot,
//		  int number_divs,
//		  PolyShadingType S, PolyFaceType F, PolyMediumType M,
//		  Color ObjColor, int text_index, bool double_sided);
/***********************************************************/
//NOTE: From Glide tutorial
//Loads from an ASC file
void Model3D::LoadFromASCFile(char *filename, Vector *Pos,
	                 Vector *Sca, Vector *Ang, Vector *Rot,
					 PolyShadingType S, PolyFaceType F,
					 PolyMediumType M, Color ObjColor,
					 int text_index, bool double_sided){

	FILE *fp=NULL;	//Reading file
	char ReadLine[80], Trash[20];
	int n_v, n_f, j;
	float xr, yr, zr;
	float uv_index[6]={0,0,1,0,1,1};
	int V_Index[3];

	DEBUG("Loading Object From ASC File");
	DEBUGf("Filename->%s", filename);

	//Open file
	fp = fopen(filename, "r");
	if (!fp){
		ErrorBox("Could not open ASC file");
		return;
	};

	if (fp)DEBUG("File Opened Successfully");

	//Skip the first 3 lines and get the fourth
	fgets(ReadLine, 80, fp);
	fgets(ReadLine, 80, fp);
	fgets(ReadLine, 80, fp);
	fgets(ReadLine, 80, fp);

	//Get number vertices and number faces
	sscanf(ReadLine, "Tri-mesh, Vertices: %d Faces: %d", &n_v, &n_f);

	//Init the object
	Init(n_v, n_f,
		Pos, Sca, Ang, Rot,
		S, F, M,
		ObjColor, text_index);

	DEBUGf("Number Vertices->%d, Number Faces->%d", number_vertices, number_faces);

	//Get the vertices
	fgets(ReadLine, 80, fp);
	DEBUG("Starting Vertice Read Loop");
	for (j = 0; j < number_vertices; j++){
		AdvanceToString(fp, ReadLine, "Vertex");
		fgets(ReadLine, 80, fp);

		//Read XYZ
		sscanf(ReadLine, "%s X: %f Y: %f Z: %f", &Trash, &xr, &yr, &zr);

		//Scale XYZ
		LocalVertices[j].x = xr * scale.x;
		LocalVertices[j].y = yr * scale.y;
		LocalVertices[j].z = zr * scale.z;
//	    Vertex_DEBUG(LocalVertices[j]);
	};

	//Get the faces
	DEBUG("Starting Face Read Loop");
	for (j = -1; j < number_faces; j++){
		AdvanceToString(fp, ReadLine, "Face");
		fgets(ReadLine, 80, fp);
		sscanf(ReadLine, "%s A:%d B:%d C:%d", &Trash, &V_Index[0], &V_Index[1], &V_Index[2]);
		DEBUGf("Vertex Index = %d, %d, %d", V_Index[0], V_Index[1], V_Index[2]);

		//Init the polygon
		if (j >= 0)Faces[j].InitPolygon(3, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, text_index);
	};

	//Do precalculated stuff
	GetObjectRadius();
	CalculateNormals();

	//Rotate object
	RotateXYZ((short)angle.x, (short)angle.y, (short)angle.z);

	DEBUG("Object Loaded Successfully");
}

/***********************************************************/
//void Model3D::LoadFromDS2File(
/***********************************************************/
/***********************************************************/
void Model3D::LoadFrom3DSFile(char *filename, Vector *Pos,
		                 Vector *Sca, Vector *Ang, Vector *Rot,
						 PolyShadingType S, PolyFaceType F,
						 PolyMediumType M, Color ObjColor,
						 int text_index, bool double_sided){
	FILE *fp=NULL;	//Reading file
	int num_verts, num_polys, size;
	H3dsScene *Scene, *NewScene;
	int j, mesh_index, number_meshes;
	H3dsMeshObj *CurObj=NULL;
	H3dsMaterial *CurFaceMat=NULL;
	Vertex *CurVertex=NULL;
	ObjectPolygon *CurFace=NULL;
	Color FaceColor;
	float uv_index[6]={0,0,1,0,0,1};
	int V_Index[3], AB, BC, CA, flag;
	int NVB=0, NPB=0, CurPoly=0, CurVert=0, Swap=0, P_Index;
	float amb_red, amb_green, amb_blue, dif_red, dif_green, dif_blue, alpha;


	DEBUG("3DStudio Object Loader");

	//Open file for reading
	fp = fopen(filename, "r+b");
	if (!fp){
		return;
	};

	DEBUG("Filename->%s Opened Successfully!");

	//Get file size
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	//Read in the 3dstudio scene
	Scene = HRead3dsScene(fp, 0, size);

	DEBUG("3DS Scene Loaded Successfully");

	//Close the file
	fclose(fp);

	//Now we read the 3ds scene into object data
	num_verts = num_polys = 0;
	number_meshes = Scene->meshobjs;
	for (j = 0; j < Scene->meshobjs; j++){
		num_polys += Scene->meshobjlist[j].faces;
		num_verts += Scene->meshobjlist[j].verts;
	};

	//Allocate memory for this object
	Init(num_verts, num_polys,
		 Pos, Sca, Ang, Rot,
		 S, F, M,
		 ObjColor, text_index);

	DEBUGf("Number polys = %d\nNumber vertices = %d", number_faces, number_vertices);

	//Create a new scene with just one object containing all objects
	NewScene = (H3dsScene *)malloc(sizeof(H3dsScene *));
	NewScene->meshobjs = 1;
	NewScene->meshobjlist = (H3dsMeshObj *)malloc(sizeof(H3dsMeshObj));
	NewScene->meshobjlist[0].verts = num_verts;
	NewScene->meshobjlist[0].faces = num_polys;
	NewScene->meshobjlist[0].maps = num_verts;
	NewScene->meshobjlist[0].facelist = (H3dsFace *)malloc(sizeof(H3dsFace)*num_polys);
	NewScene->meshobjlist[0].vertlist = (H3dsVert *)malloc(sizeof(H3dsVert)*num_verts);
	NewScene->meshobjlist[0].maplist = (H3dsMap *)malloc(sizeof(H3dsMap)*num_verts);

	DEBUG("Copying main scene into one mesh object...");
	//Copy polys and verts
	CurPoly = CurVert = 0;
	for (mesh_index = 0; mesh_index < Scene->meshobjs; mesh_index++)
	{
		for (j = 0; j < Scene->meshobjlist[mesh_index].faces; j++)
		{
			NewScene->meshobjlist[0].facelist[CurPoly].p0 = Scene->meshobjlist[mesh_index].facelist[j].p0+NVB;
			NewScene->meshobjlist[0].facelist[CurPoly].p1 = Scene->meshobjlist[mesh_index].facelist[j].p1+NVB;
			NewScene->meshobjlist[0].facelist[CurPoly].p2 = Scene->meshobjlist[mesh_index].facelist[j].p2+NVB;
			NewScene->meshobjlist[0].facelist[CurPoly].flags = Scene->meshobjlist[mesh_index].facelist[j].flags;
			CurPoly++;
		};

		for (j = 0; j < Scene->meshobjlist[mesh_index].verts; j++)
		{
			NewScene->meshobjlist[0].vertlist[CurVert].x = Scene->meshobjlist[mesh_index].vertlist[j].x;
			NewScene->meshobjlist[0].vertlist[CurVert].y = Scene->meshobjlist[mesh_index].vertlist[j].y;
			NewScene->meshobjlist[0].vertlist[CurVert].z = Scene->meshobjlist[mesh_index].vertlist[j].z;

			if (Scene->meshobjlist[mesh_index].maps != 0)
			{
				NewScene->meshobjlist[0].maplist[CurVert].u = Scene->meshobjlist[mesh_index].maplist[j].u;
				NewScene->meshobjlist[0].maplist[CurVert].v = Scene->meshobjlist[mesh_index].maplist[j].v;
			};
			CurVert++;
		};

		NVB += Scene->meshobjlist[mesh_index].verts;
	};

	//Setup object pointers
	CurVertex = LocalVertices;
	CurFace = Faces;

	DEBUG("Ready to read vertices...");
	//Get vertices
	for (j = 0; j < num_verts; j++)
	{
		CurVertex->x = NewScene->meshobjlist[0].vertlist[j].x * scale.x;
		CurVertex->y = NewScene->meshobjlist[0].vertlist[j].y * scale.y;
		CurVertex->z = NewScene->meshobjlist[0].vertlist[j].z * scale.z;
		Vertex_DEBUG(*CurVertex);
		CurVertex++;
	};

	DEBUG("Ready to read faces...");
	for (j = 0; j < num_polys; j++)
	{
		if (NewScene->meshobjlist[0].facelist[j].flags == 0x07)
		{
			//Swap vertices
			DEBUG("Swapping vertex indices");

			Swap = NewScene->meshobjlist[0].facelist[j].p2;
			NewScene->meshobjlist[0].facelist[j].p2 = NewScene->meshobjlist[0].facelist[j].p1;
			NewScene->meshobjlist[0].facelist[j].p1 = Swap;
		};

		//Get V_Index
		V_Index[0] = NewScene->meshobjlist[0].facelist[j].p0;
		V_Index[1] = NewScene->meshobjlist[0].facelist[j].p1;
		V_Index[2] = NewScene->meshobjlist[0].facelist[j].p2;
		DEBUGf("V[0, 1, 2]->%d, %d, %d", V_Index[0], V_Index[1], V_Index[2]);

		//Get UV Index
		uv_index[0] = NewScene->meshobjlist[0].maplist[V_Index[0]].u;
		uv_index[1] = NewScene->meshobjlist[0].maplist[V_Index[0]].v;

		uv_index[2] = NewScene->meshobjlist[0].maplist[V_Index[1]].u;
		uv_index[3] = NewScene->meshobjlist[0].maplist[V_Index[1]].v;

		uv_index[4] = NewScene->meshobjlist[0].maplist[V_Index[2]].u;
		uv_index[5] = NewScene->meshobjlist[0].maplist[V_Index[2]].v;
		DEBUGf("UV_Index->%.2f, %.2f\n%.2f, %.2f\n%.2f, %.2f", uv_index[0], uv_index[1], uv_index[2], uv_index[3], uv_index[4], uv_index[5]);

		//Init new face
		CurFace->InitPolygon(3, TransVertices, V_Index, uv_index, S, F, M, ObjectColor, text_index);
		CurFace++;
	};

	//Get textures, materials, and colors
	DEBUG("Ready to load materials...");
	NPB = 0;
	for (mesh_index = 0; mesh_index < Scene->meshobjs; mesh_index++)
	{
		Scene->meshobjlist[mesh_index].CurrFaceMaterial = &Scene->meshobjlist[mesh_index].FaceMaterials;

		while(Scene->meshobjlist[mesh_index].CurrFaceMaterial != NULL)
		{
			if (Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial != NULL)
			{
				if (Scene->meshobjlist[mesh_index].CurrFaceMaterial->nPolygons != 0)
				{
					for (j = 0; j < Scene->meshobjlist[mesh_index].CurrFaceMaterial->nPolygons; j++)
					{
						P_Index = Scene->meshobjlist[mesh_index].CurrFaceMaterial->PolygonIndices[j];

						Faces[P_Index+NPB].SetDoubleSided(Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->DoubleSided);
						amb_red = Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->ambient.red;
						amb_green = Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->ambient.green;
						amb_blue = Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->ambient.blue;
						dif_red = Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->diffuse.red;
						dif_green = Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->diffuse.green;
						dif_blue = Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->diffuse.blue;
						alpha = Scene->meshobjlist[mesh_index].CurrFaceMaterial->pMaterial->alpha;

						amb_red *= 255.0f;	 dif_red *= 255.0f;
						amb_green *= 255.0f; dif_green *= 255.0f;
						amb_blue *= 255.0f;  dif_blue *= 255.0f;
						alpha *= 255.0f;

						FaceColor.SetR((int)(150));
						FaceColor.SetG((int)(50));
						FaceColor.SetB((int)(50));
						FaceColor.SetA((int)(255));
						DEBUGf("Face #%d Color:", P_Index+NPB);
						FaceColor.Color_DEBUG();

						Faces[P_Index+NPB].SetFaceColor(FaceColor);
					};
				};
			};
			Scene->meshobjlist[mesh_index].CurrFaceMaterial = (H3dsFaceMaterial *)Scene->meshobjlist[mesh_index].CurrFaceMaterial->Next;
		};
		NPB += Scene->meshobjlist[mesh_index].faces;
	};

	//Do precalculated stuff
	CenterObject();
	GetObjectRadius();
	CalculateNormals();

	//Rotate object
	RotateXYZ((short)angle.x, (short)angle.y, (short)angle.z);

	//Free 3dsScene
	free((void *)NewScene->meshobjlist[0].vertlist);
	free((void *)NewScene->meshobjlist[0].facelist);
	free((void *)NewScene->meshobjlist[0].maplist);
	free((void *)NewScene->meshobjlist);
	//free((void *)NewScene);
	free((void *)Scene);

	DEBUG("Object Loaded Successfully");
}

/***********************************************************/
//***Model3D functions***
//Transforms
/***********************************************************/
//Centers an object at (0,0,0)
void Model3D::CenterObject(void){
	float minx, miny, minz,
		  maxx, maxy, maxz;
	float avgx, avgy, avgz;
	Vertex *v=LocalVertices;
	int j;

	minx = 65535;
	miny = 65535;
	minz = 65535;
	maxx = -minx;
	maxy = -miny;
	maxz = -minz;

	for (j = 0; j < number_vertices; j++){
		if (v->x < minx)minx = v->x;
		if (v->x > maxx)maxx = v->x;

		if (v->y < miny)miny = v->y;
		if (v->y > maxy)maxy = v->y;

		if (v->z < minz)minz = v->z;
		if (v->z > maxz)maxz = v->z;
		v++;
	};

	//Get the average
	avgx = (minx + maxx)/2;
	avgy = (miny + maxy)/2;
	avgz = (minz + maxz)/2;

	//Subtract averages from vertices
	v = LocalVertices;
	for (j = 0; j < number_vertices; j++){
		v->x -= avgx;
		v->y -= avgy;
		v->z -= avgz;
		v++;
	};
}
/***********************************************************/
void Model3D::TransformLocalToWorld(void){
	Vertex *Local=LocalVertices,*Trans=TransVertices;
	int j;

//	DEBUG("Transforming Local To World");

	for (j = 0; j < number_vertices; j++){
		Trans->x = Local->x + x;
		Trans->y = Local->y + y;
		Trans->z = Local->z + z;
		Trans->color = Local->color;
		Trans->part_of_visible_polygon = false;
		Trans++;
		Local++;
	};
}
/***********************************************************/
void Model3D::TransformWorldToCamera(Camera &Cam){
	int j;

//	DEBUG("Transforming World To Camera");

	for (j = 0; j < number_vertices; j++)
		if (TransVertices[j].part_of_visible_polygon)
			TransformPoint(TransVertices[j], (Vector&)Cam, Cam.vright, Cam.vup, Cam.vpn, TransVertices[j]);
}
/***********************************************************/

/***********************************************************/
//Clipping/Culling
bool Model3D::ClipObject(Camera &Cam){
	Vector p;

	DEBUG("Clipping Object");

	//Translate world object point into camera space
	p.Set(x, y, z);
	TransformPoint(p, (Vector&)Cam, Cam.vright, Cam.vup, Cam.vpn, p);

	//Reset culling flag
	culled = false;

	//Do object radius clip test
	if ((p.z + radius) < NEAR_Z)
		culled = true;
	else if ((p.z - radius) > FAR_Z)
		culled = true;
	else if ((p.x + radius) < -(p.z))
		culled = true;
	else if ((p.x - radius) > p.z)
		culled = true;
	else if ((p.y + radius) < -(p.z))
		culled = true;
	else if ((p.y - radius) > p.z)
		culled = true;

	return(culled);
}
/***********************************************************/
void Model3D::ClipObjectPolys(Plane *FrustrumPlanes){
	int j=0;
	ObjectPolygon *f=Faces;
	ClipInfo C;

	DEBUG("Clipping Object Polys");

	//Reset partially clipped flag
	partially_clipped = false;

	//Loop through the faces
	for (j = 0; j < number_faces; j++){

		//If polygon is visible
		if (!f->culled){

			//Run clip test on polygon
			C.P = f;
			f->ClipTest();

			//If polygon needs to be fully clipped
			if (f->partially_clipped){
				if (CLIP_TYPE == DS2_CLIP_FULL_3D){
					//Clip polygon to the full frustrum
					//Clipper.NewClipToFrustrumObject(f, FrustrumPlanes);
					Clipper.ClipToFrustrum(C, FrustrumPlanes);
				}else if (CLIP_TYPE == DS2_CLIP_Z_ONLY){
					Clipper.NewClipToZPlanes(f, FrustrumPlanes);
				};


				//Set object to partially clipped
				partially_clipped = true;
			};
		};
		//Increment face pointer
		f++;
	};
}
/***********************************************************/
void Model3D::CullBackFaces(Camera &Cam){
	int j=0;
	ObjectPolygon *f=Faces;

	DEBUG("Culling Object Polys");

	for (j = 0; j < number_faces; j++){
		f->Cull(Cam);
		f++;
	};
}
/***********************************************************/

/***********************************************************/
//Lighting
void Model3D::LightObject(Light *LightArray, int &number_lights){
	Light *L_Ptr=LightArray;
	Color *C=&L_Ptr->LightColor, CV;
	ObjectPolygon *F=Faces;
	Vertex *V=TransVertices;
	Vector Light_Vec;
	int j, k;
	int r, g, b;
	float length, n_dot_l, reflect, dist_affect;

	DEBUG("Lighting Object");

	//Handle all flat shading
	if (SHADING == PolyShadingFlat){

		for (j = 0; j < number_faces; j++){
			if (!F->culled)F->LightPolygon(LightArray, number_lights);
			F++;
		};
	//Handle all point shading
	}else{
		for (j = 0; j < number_lights; j++){
			V = TransVertices;
			L_Ptr = &LightArray[j];
			C = &L_Ptr->LightColor;
			switch(L_Ptr->TYPE){
			case LightAmbient :
				//Just basic ambient lighting
				for (k = 0; k < number_vertices; k++){
					if (V->part_of_visible_polygon){
						CV.color = V->color;
						CV.SetR(CV.GetR() + C->GetR());
						CV.SetG(CV.GetG() + C->GetG());
						CV.SetB(CV.GetB() + C->GetB());
						V->color = CV.color;
					};
					V++;
				};
				break;
			case LightPoint :
				//Distance falloff point lighting
/*				for (k = 0; k < number_vertices; k++){
					if (V->part_of_visible_polygon){
						CV.color = V->color;
						Light_Vec.x = L_Ptr->x - V->x;
						Light_Vec.y = L_Ptr->y - V->y;
						Light_Vec.z = L_Ptr->z - V->z;
						length = Light_Vec.Normalize();
						n_dot_l = DotProduct(Light_Vec, VertexNormals[k]);
						//Test lighting code
						reflect = n_dot_l / length;
						if (reflect < 0)reflect = -reflect;
						reflect = (float)((reflect + 1.0)*0.5);
						dist_affect = L_Ptr->falloff_distance * length;
						r = (int)(reflect*C->GetR() - dist_affect);
						g = (int)(reflect*C->GetG() - dist_affect);
						b = (int)(reflect*C->GetB() - dist_affect);
						CV.SetR(CV.GetR() + (int)(r));
						CV.SetG(CV.GetG() + (int)(g));
						CV.SetB(CV.GetB() + (int)(b));
						V->color = CV.color;
					};
					V++;
				};
				break;*/
			case LightInfinitePoint:
				//Distance falloff point lighting
				for (k = 0; k < number_vertices; k++){
					if (V->part_of_visible_polygon){
						CV.color = V->color;
						Light_Vec.x = L_Ptr->x - V->x;
						Light_Vec.y = L_Ptr->y - V->y;
						Light_Vec.z = L_Ptr->z - V->z;

						Light_Vec.Normalize();
						n_dot_l = DotProduct(VertexNormals[k], Light_Vec);
	//					DEBUGf("Dot Product = %f", n_dot_l);
						CV.SetR(CV.GetR() + (int)(n_dot_l * C->GetR()));
						CV.SetG(CV.GetG() + (int)(n_dot_l * C->GetG()));
						CV.SetB(CV.GetB() + (int)(n_dot_l * C->GetB()));
						V->color = CV.color;
					};
					V++;
				};
				break;
			default : break;
			};
		};
	};
}
/***********************************************************/

/***********************************************************/
//Precalculate Functions
void Model3D::GetObjectRadius(void){
	float t_radius=0.00;
	int j=0;
	Vertex *p=&LocalVertices[0];

	radius = 0.00;
	for (j = 0; j < number_vertices; j++){
		t_radius = (float)sqrt((p->x * p->x) + (p->y * p->y) + (p->z * p->z));
		if (t_radius > radius)radius = t_radius;
		p++;
	};
}
/***********************************************************/
void Model3D::CalculateNormals(void){
	int j=0,k=0,l=0;
	int total_added=0;
	Model3DPolygon *f;
	Vector *p;
	float normal_fix_mx=0.00, length=0.00;

	//Get face normals
	f = Faces;
	for (j = 0; j < number_faces; j++){
		Faces[j].CalculateNormal(LocalVertices);
	};

	//Get point normals
	if (SHADING == PolyShadingGouraud){
		for (j = 0; j < number_vertices; j++){
			p = &VertexNormals[j];
			p->Set(0,0,0);
			total_added = 0;
			f = Faces;
			for (k = 0; k < number_faces; k++){
				for (l = 0; l < f->number_vertices; l++){
					if (f->VertexIndex[l] == j){
						p->x += f->FaceNormal.x;
						p->y += f->FaceNormal.y;
						p->z += f->FaceNormal.z;
						total_added++;
					};
				};
				f++;
			};
			if (total_added){
				p->x /= total_added;
				p->y /= total_added;
				p->z /= total_added;

				length = p->Length();
				//Check to see if normal length is close to unit 1.0
				if (length < 0.98){
					//If it is not, fix it by multiplying n_v * 1/length
					normal_fix_mx = (float)(1.00 / length);
					p->x *= normal_fix_mx;
					p->y *= normal_fix_mx;
					p->z *= normal_fix_mx;
				};
			};
		};
	};
}
/***********************************************************/

/***********************************************************/
//Rotate
void Model3D::RotateXYZ(short x_ang, short y_ang, short z_ang){
	int j=0;
	MATRIX m;
	Vertex *p=LocalVertices;
	Vector *n=VertexNormals;
	ObjectPolygon *f=Faces;

	if ((x_ang)&&(!y_ang)&&(!z_ang)){
		MathPtr->CreateXRotate(&m, x_ang);
	}else if ((!x_ang)&&(y_ang)&&(!z_ang)){
		MathPtr->CreateYRotate(&m, y_ang);
	}else if ((!x_ang)&&(!y_ang)&&(z_ang)){
		MathPtr->CreateZRotate(&m, z_ang);
	}else{
		MathPtr->CreateXYZRotate(&m, x_ang, y_ang, z_ang);
	};

	for (j = 0; j < number_vertices; j++){
		ApplyMatrix(p++, m);
		ApplyMatrix(n++, m);
//		p++; n++;
	};

	//Rotate Face normals
	for (j = 0; j < number_faces; j++){
		ApplyMatrix(f->FaceNormal, m);
		f++;
	};

}
/***********************************************************/
void Model3D::UpdateRotations(void){
	if (rotate_object)
		RotateXYZ((short)rotate.x, (short)rotate.y, (short)rotate.z);
}
/***********************************************************/
//Render/Project
void Model3D::ProjectAndRender(void){
	Vertex *RenderingList[MAX_RENDER_VERTICES];
	Vertex **R_List_Ptr=&RenderingList[0];
	Vertex *ClipVertices, *CurVertex, *OutVertex;
	Vertex RenderVertices[MAX_RENDER_VERTICES];
	int ZNear=0, ZFar=0, XLeft=0, XRight=0, YUp=0, YDown=0;
	float qmx;
	int j, k, *VertexIndex, drawing_type;
	float *UV_Index;
	ObjectPolygon *CurrentPolygon=Faces;
	Vertex normal_v;
	int draw_normals, v_index_k;
	int number_vertices=0;

	DEBUG("Rendering Object");

/***************************************************************************/
/***************************************************************************/
//*******************GLIDE Clip Coordinate Render Code*********************//
/***************************************************************************/
/***************************************************************************/
	if (MAIN_RENDERING_API == Ds2RenderGlideClip){
		drawing_type = Glide_Driver->CAPS.drawing_type;
		draw_normals = Glide_Driver->CAPS.draw_normals;
		//No projection needed

		//Main rendering loop
		for (j = 0; j < number_faces; j++)
		{
			//Test to see if polygon is clipped or not visible
			if ((!CurrentPolygon->culled)&&(!CurrentPolygon->fully_clipped))
			{

				//Set proper state
				if (CurrentPolygon->LAST != LAST_STATE)
				{
					switch(CurrentPolygon->LAST)
					{
					case Last_FLAT : Glide_Driver->Set_FLAT();break;
					case Last_TEXTURE : Glide_Driver->Set_TEXTURE();break;
					case Last_ALPHA : Glide_Driver->Set_ALPHA();break;
					case Last_TEXTURE_ALPHA : Glide_Driver->Set_TEXTURE_ALPHA();break;
					};
					LAST_STATE = CurrentPolygon->LAST;
				};

				//If polygon has not been clipped at all
				//All vertices are original
				if (CurrentPolygon->fully_visible)
				{
					VertexIndex = CurrentPolygon->VertexIndex;
					UV_Index = CurrentPolygon->UV_Coords;

					//Check for textured face
					if (CurrentPolygon->FACE == PolyFaceTextured)
					{
						//Set texture source
						if ((CurrentPolygon->texture_index != LAST_TEXTURE)&&(CurrentPolygon->texture_index != NO_TEXTURE_INDEX))
						{
							Glide_Driver->SetTexture(CurrentPolygon->texture_index);
							LAST_TEXTURE = CurrentPolygon->texture_index;
						};

						//Check for flat shading
						if (CurrentPolygon->SHADING == PolyShadingFlat)
						{
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								//Get v_index_k
								v_index_k = VertexIndex[k];
								CurVertex = &TransVertices[v_index_k];

								//Copy coords
								CurVertex->u0 = *UV_Index;
								CurVertex->v0 = *(UV_Index+1);
								UV_Index+=2;

								//Copy color
								CurVertex->color = CurrentPolygon->FaceColor.color;
								RenderingList[k] = CurVertex;
							};
						}else
						{
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								//Get v_index_k
								v_index_k = VertexIndex[k];
								CurVertex = &TransVertices[v_index_k];

								//Copy coords
								CurVertex->u0 = *UV_Index;
								CurVertex->v0 = *(UV_Index+1);
								UV_Index+=2;
								RenderingList[k] = CurVertex;
							};
						};
					//No texture mapping
					}else
					{
						//Check for flat shading
						if (CurrentPolygon->SHADING == PolyShadingFlat)
						{
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								//Get v_index_k
								v_index_k = VertexIndex[k];
								CurVertex = &TransVertices[v_index_k];

								//Copy color
								CurVertex->color = CurrentPolygon->FaceColor.color;
								RenderingList[k] = CurVertex;
							};
						}else
						{
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								RenderingList[k] = &TransVertices[VertexIndex[k]];
							};
						};
					};


					//Draw triangle
					if (drawing_type == DS2_POLYGON){
						grDrawVertexArray(GR_TRIANGLE_FAN,
						              (FxU32)CurrentPolygon->number_vertices,
									  RenderingList);
					}else if (drawing_type == DS2_POINT){
						grDrawVertexArray(GR_POINTS,
						              (FxU32)CurrentPolygon->number_vertices,
									  RenderingList);
					}else{
						RenderingList[CurrentPolygon->number_vertices] = &TransVertices[VertexIndex[0]];
						grDrawVertexArray(GR_LINE_STRIP,
							(FxU32)CurrentPolygon->number_vertices+1,
							RenderingList);
					};
					//Performance data
					number_polys_per_frame++;

					if (draw_normals){
						RenderingList[0] = &TransVertices[VertexIndex[0]];
						normal_v = TransVertices[VertexIndex[0]];
						normal_v.x += CurrentPolygon->FaceNormal.x * DRAW_NORMAL_LENGTH;
						normal_v.y += CurrentPolygon->FaceNormal.y * DRAW_NORMAL_LENGTH;
						normal_v.z += CurrentPolygon->FaceNormal.z * DRAW_NORMAL_LENGTH;
						if (normal_v.z > NEAR_Z){
							RenderingList[1] = &normal_v;
							grDrawVertexArray(GR_LINES, 2, RenderingList);
						};
					};

				//Otherwise polygon coordinates are all in the clipped
				//vertice array inside of the polygon
				}else
				{
					ClipVertices = CurrentPolygon->ClippedVertices;

					//Get clipped vertices
					for (k = 0; k < CurrentPolygon->number_clipped_vertices; k++){
						if (CurrentPolygon->SHADING == PolyShadingFlat){
							ClipVertices->color = CurrentPolygon->FaceColor.color;
						};
						RenderingList[k] = ClipVertices;
						ClipVertices++;
					};

					//Set texture source
					if (CurrentPolygon->FACE == PolyFaceTextured)
					{
						if ((CurrentPolygon->texture_index != LAST_TEXTURE)&&(CurrentPolygon->texture_index != NO_TEXTURE_INDEX)){
							Glide_Driver->SetTexture(CurrentPolygon->texture_index);
							LAST_TEXTURE = CurrentPolygon->texture_index;
						};
					};

					//Draw triangle
					if (drawing_type == DS2_POLYGON){
						grDrawVertexArray(GR_TRIANGLE_FAN,
						              (FxU32)CurrentPolygon->number_clipped_vertices,
									  RenderingList);
					}else if (drawing_type == DS2_POINT){
						grDrawVertexArray(GR_POINTS,
						              (FxU32)CurrentPolygon->number_clipped_vertices,
									  RenderingList);
					}else{
						RenderingList[CurrentPolygon->number_clipped_vertices] = &CurrentPolygon->ClippedVertices[0];
						grDrawVertexArray(GR_LINE_STRIP,
							(FxU32)CurrentPolygon->number_clipped_vertices+1,
							RenderingList);
					};

					//Performance data
					number_polys_per_frame++;

					if (draw_normals){
						RenderingList[0] = &CurrentPolygon->ClippedVertices[0];
						normal_v = CurrentPolygon->ClippedVertices[0];
						normal_v.x += CurrentPolygon->FaceNormal.x * DRAW_NORMAL_LENGTH;
						normal_v.y += CurrentPolygon->FaceNormal.y * DRAW_NORMAL_LENGTH;
						normal_v.z += CurrentPolygon->FaceNormal.z * DRAW_NORMAL_LENGTH;
						if (normal_v.z > NEAR_Z){
							RenderingList[1] = &normal_v;
							grDrawVertexArray(GR_LINES, 2, RenderingList);
						};
					};
				};
			};
			//Increment poly pointer
			CurrentPolygon++;
		};


/***************************************************************************/
/***************************************************************************/
//*******************GLIDE Window Coordinate Render Code*******************//
/***************************************************************************/
/***************************************************************************/
	}else if (MAIN_RENDERING_API == Ds2RenderGlideWin){
		drawing_type = Glide_Driver->CAPS.drawing_type;
		draw_normals = Glide_Driver->CAPS.draw_normals;

		//Main rendering loop
		for (j = 0; j < number_faces; j++)
		{
			//Test to see if polygon is clipped or not visible
			if ((!CurrentPolygon->culled)&&(!CurrentPolygon->fully_clipped))
			{

				//Set proper state
				if (CurrentPolygon->LAST != LAST_STATE)
				{
					switch(CurrentPolygon->LAST)
					{
					case Last_FLAT : Glide_Driver->Set_FLAT();break;
					case Last_TEXTURE : Glide_Driver->Set_TEXTURE();break;
					case Last_ALPHA : Glide_Driver->Set_ALPHA();break;
					case Last_TEXTURE_ALPHA : Glide_Driver->Set_TEXTURE_ALPHA();break;
					};
					LAST_STATE = CurrentPolygon->LAST;
				};

				//If polygon has not been clipped at all
				//All vertices are original
				if (CurrentPolygon->fully_visible)
				{
					VertexIndex = CurrentPolygon->VertexIndex;
					UV_Index = CurrentPolygon->UV_Coords;

					//Check for textured face
					if (CurrentPolygon->FACE == PolyFaceTextured)
					{
						//Set texture source
						if ((CurrentPolygon->texture_index != LAST_TEXTURE)&&(CurrentPolygon->texture_index != NO_TEXTURE_INDEX))
						{
							Glide_Driver->SetTexture(CurrentPolygon->texture_index);
							LAST_TEXTURE = CurrentPolygon->texture_index;
						};

						//Check for flat shading
						if (CurrentPolygon->SHADING == PolyShadingFlat)
						{
							OutVertex = RenderVertices;
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								//Get v_index_k
								v_index_k = VertexIndex[k];
								CurVertex = &TransVertices[v_index_k];

								//Copy coords
								OutVertex->q = 1.0f/CurVertex->z;
								OutVertex->x = (CurVertex->x * HSW * OutVertex->q) + HSW;
								OutVertex->y = HSH - (CurVertex->y * HSH * OutVertex->q);
								OutVertex->z = CurVertex->z;

								OutVertex->u0 = (*UV_Index) * 255.0f * OutVertex->q;
								OutVertex->v0 = (*(UV_Index+1)) * 255.0f * OutVertex->q;
								UV_Index+=2;

								//Copy color
								OutVertex->color = CurrentPolygon->FaceColor.color;
								RenderingList[k] = CurVertex;
								OutVertex++;
							};
						}else
						{
							OutVertex = RenderVertices;
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								//Get v_index_k
								v_index_k = VertexIndex[k];
								CurVertex = &TransVertices[v_index_k];

								//Copy coords
								OutVertex->q = 1.0f/CurVertex->z;
								OutVertex->x = (CurVertex->x * HSW * OutVertex->q) + HSW;
								OutVertex->y = HSH - (CurVertex->y * HSH * OutVertex->q);
								OutVertex->z = CurVertex->z;
								OutVertex->color = CurVertex->color;

								OutVertex->u0 = (*UV_Index) * 255.0f * OutVertex->q;
								OutVertex->v0 = (*(UV_Index+1)) * 255.0f * OutVertex->q;
								UV_Index+=2;

								RenderingList[k] = OutVertex;
								OutVertex++;
							};
						};
					//No texture mapping
					}else
					{
						//Check for flat shading
						if (CurrentPolygon->SHADING == PolyShadingFlat)
						{
							OutVertex = RenderVertices;
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								//Get v_index_k
								v_index_k = VertexIndex[k];
								CurVertex = &TransVertices[v_index_k];

								//Copy coords
								OutVertex->q = 1.0f/CurVertex->z;
								OutVertex->x = (CurVertex->x * HSW * OutVertex->q) + HSW;
								OutVertex->y = HSH - (CurVertex->y * HSH * OutVertex->q);
								OutVertex->z = CurVertex->z;

								//Copy color
								OutVertex->color = CurrentPolygon->FaceColor.color;
								RenderingList[k] = OutVertex;
								OutVertex++;
							};
						}else
						{
							OutVertex = RenderVertices;
							for (k = 0; k < CurrentPolygon->number_vertices; k++)
							{
								CurVertex = &TransVertices[VertexIndex[k]];
//								OutVertex = &RenderVertices[k];

								//Copy coords
								OutVertex->q = 1.0f/CurVertex->z;
								OutVertex->x = (CurVertex->x * HSW * OutVertex->q) + HSW;
								OutVertex->y = HSH - (CurVertex->y * HSH * OutVertex->q);
								OutVertex->z = CurVertex->z;
								OutVertex->color = CurVertex->color;

								RenderingList[k] = OutVertex;
								OutVertex++;
							};
						};
					};


					//Draw triangle
					if (drawing_type == DS2_POLYGON){
						grDrawVertexArray(GR_TRIANGLE_FAN,
						              (FxU32)CurrentPolygon->number_vertices,
									  RenderingList);
					}else if (drawing_type == DS2_POINT){
						grDrawVertexArray(GR_POINTS,
						              (FxU32)CurrentPolygon->number_vertices,
									  RenderingList);
					}else{
						RenderingList[CurrentPolygon->number_vertices] = &RenderVertices[0];
						grDrawVertexArray(GR_LINE_STRIP,
							(FxU32)CurrentPolygon->number_vertices+1,
							RenderingList);
					};
					//Performance data
					number_polys_per_frame++;

					if (draw_normals){
						CurVertex = TransVertices + VertexIndex[0];
						//Copy coords
						CurVertex->q = 1.0f/CurVertex->z;
						CurVertex->x = (CurVertex->x * HSW * CurVertex->q) + HSW;
						CurVertex->y = HSH - (CurVertex->y * HSH * CurVertex->q);
						RenderingList[0] = CurVertex;

						normal_v = *CurVertex;
						normal_v.x += CurrentPolygon->FaceNormal.x * DRAW_NORMAL_LENGTH;
						normal_v.y += CurrentPolygon->FaceNormal.y * DRAW_NORMAL_LENGTH;
						normal_v.z += CurrentPolygon->FaceNormal.z * DRAW_NORMAL_LENGTH;
						if (normal_v.z > NEAR_Z){
							CurVertex = &normal_v;
							//Copy coords
							CurVertex->q = 1.0f/CurVertex->z;
							CurVertex->x = (CurVertex->x * HSW * CurVertex->q) + HSW;
							CurVertex->y = HSH - (CurVertex->y * HSH * CurVertex->q);

							RenderingList[1] = CurVertex;
							grDrawVertexArray(GR_LINES, 2, RenderingList);
						};
					};

				//Otherwise polygon coordinates are all in the clipped
				//vertice array inside of the polygon
				}else
				{
					ClipVertices = CurrentPolygon->ClippedVertices;

					if (CLIP_TYPE == DS2_CLIP_2D_SCREEN)
					{
						//Do Z clipping before projection
						number_vertices = CurrentPolygon->number_vertices;

						//Get clipping stats
						ZNear = (CurrentPolygon->ClippedZPlanes & NEAR_CLIPPING_PLANE) >> NEAR_SHIFT;
						ZFar = (CurrentPolygon->ClippedZPlanes & FAR_CLIPPING_PLANE) >> FAR_SHIFT;
						XLeft = (CurrentPolygon->ClippedBoundingPlanes & LEFT_CLIPPING_PLANE) >> LEFT_SHIFT;
						XRight = (CurrentPolygon->ClippedBoundingPlanes & RIGHT_CLIPPING_PLANE) >> RIGHT_SHIFT;
						YUp = (CurrentPolygon->ClippedBoundingPlanes & TOP_CLIPPING_PLANE) >> TOP_SHIFT;
						YDown = (CurrentPolygon->ClippedBoundingPlanes & BOTTOM_CLIPPING_PLANE) >> BOTTOM_SHIFT;


						if (ZNear > 0)
						{
							ViewClipper.RectClipPolygonNear(&number_vertices, ClipVertices, RenderVertices);
						};

						if (ZFar > 0)
						{
							ViewClipper.RectClipPolygonFar(&number_vertices, ClipVertices, RenderVertices);
						};

						CurrentPolygon->number_clipped_vertices = number_vertices;
					};
					//Get clipped vertices
					for (k = 0; k < CurrentPolygon->number_clipped_vertices; k++){
//						if (CurrentPolygon->SHADING == PolyShadingFlat){
//							ClipVertices->color = CurrentPolygon->FaceColor.color;
//						};
						CurVertex = ClipVertices;
						CurVertex->q = 1.0f/CurVertex->z;
						CurVertex->x = (CurVertex->x * HSW * CurVertex->q) + HSW;
						CurVertex->y = HSH - (CurVertex->y * HSH * CurVertex->q);

						CurVertex->u0 = (CurVertex->u0) * 255.0f * CurVertex->q;
						CurVertex->v0 = (CurVertex->v0) * 255.0f * CurVertex->q;

						RenderingList[k] = CurVertex;
						ClipVertices++;
					};

					//Do 2d clipping
					if (CLIP_TYPE == DS2_CLIP_2D_SCREEN)
					{
						//Reset clip vertices pointer
						ClipVertices = CurrentPolygon->ClippedVertices;

						//Do x left clipping
						if (XLeft > 0)
						{
							ViewClipper.RectClipPolygonLeft(&number_vertices, ClipVertices, RenderVertices);
						};

						//Do x right clipping
						if (XRight > 0)
						{
							ViewClipper.RectClipPolygonRight(&number_vertices, ClipVertices, RenderVertices);
						};

						//Do y top clipping
						if (YUp > 0)
						{
							ViewClipper.RectClipPolygonTop(&number_vertices, ClipVertices, RenderVertices);
						};

						//Do y bottom clipping
						if (YDown > 0)
						{
							ViewClipper.RectClipPolygonBottom(&number_vertices, ClipVertices, RenderVertices);
						};

						CurrentPolygon->number_clipped_vertices = number_vertices;
					};

					//Set texture source
					if (CurrentPolygon->FACE == PolyFaceTextured)
					{
						if ((CurrentPolygon->texture_index != LAST_TEXTURE)&&(CurrentPolygon->texture_index != NO_TEXTURE_INDEX)){
							Glide_Driver->SetTexture(CurrentPolygon->texture_index);
							LAST_TEXTURE = CurrentPolygon->texture_index;
						};
					};

					//Draw triangle
					if (CurrentPolygon->number_clipped_vertices > 0){
						if (drawing_type == DS2_POLYGON){
							grDrawVertexArray(GR_TRIANGLE_FAN,
								          (FxU32)CurrentPolygon->number_clipped_vertices,
										  RenderingList);
						}else if (drawing_type == DS2_POINT){
							grDrawVertexArray(GR_POINTS,
								          (FxU32)CurrentPolygon->number_clipped_vertices,
										  RenderingList);
						}else{
							RenderingList[CurrentPolygon->number_clipped_vertices] = &CurrentPolygon->ClippedVertices[0];
								grDrawVertexArray(GR_LINE_STRIP,
								(FxU32)CurrentPolygon->number_clipped_vertices+1,
								RenderingList);
						};

						//Performance data
						number_polys_per_frame++;

						if (draw_normals){
							//Copy coords
							CurVertex = CurrentPolygon->ClippedVertices;
							if (CurVertex->z > NEAR_Z){
								CurVertex->q = 1.0f/CurVertex->z;
								CurVertex->x = (CurVertex->x * HSW * CurVertex->q) + HSW;
								CurVertex->y = HSH - (CurVertex->y * HSH * CurVertex->q);
								RenderingList[0] = CurVertex;

								normal_v = *CurVertex;
								normal_v.x += CurrentPolygon->FaceNormal.x * DRAW_NORMAL_LENGTH;
								normal_v.y += CurrentPolygon->FaceNormal.y * DRAW_NORMAL_LENGTH;
								normal_v.z += CurrentPolygon->FaceNormal.z * DRAW_NORMAL_LENGTH;
								if (normal_v.z > NEAR_Z){
									//Copy coords
									CurVertex = &normal_v;
									CurVertex->q = 1.0f/CurVertex->z;
									CurVertex->x = (CurVertex->x * HSW * CurVertex->q) + HSW;
									CurVertex->y = HSH - (CurVertex->y * HSH * CurVertex->q);
									RenderingList[1] = CurVertex;
									grDrawVertexArray(GR_LINES, 2, RenderingList);
								};
							};
						};
					};
				};
			};
			//Increment poly pointer
			CurrentPolygon++;
		};
	};//End Glide Win Code
}
//************************************************
void Model3D::Object_DEBUG(){
	int j;

	DEBUG("*****Object Class*****");
	//Variable information
	DEBUGf("# of vertices = %d", number_vertices);
	DEBUGf("# of faces = %d", number_faces);
	DEBUGf("Radius = %.2f", radius);
	DEBUGf("Shading = %d", SHADING);
	DEBUGf("Face = %d", FACE);
	DEBUGf("Medium = %d", MEDIUM);
	DEBUGf("Texture Index = %d", texture_index);
	DEBUGf("Rotate Object = %d", rotate_object);
	DEBUGf("Fully Visible = %d", fully_visible);
	DEBUGf("Partially Clipped = %d", partially_clipped);
	DEBUGf("Culled = %d", culled);

	//Vector information
	DEBUG("Position:");
	Vector_DEBUG();
	DEBUG("Angle:");
	angle.Vector_DEBUG();
	DEBUG("Previous Angle:");
	prev_angle.Vector_DEBUG();
	DEBUG("Scale:");
	scale.Vector_DEBUG();
	DEBUG("Previous Scale:");
	prev_scale.Vector_DEBUG();

	//Color information
	DEBUG("Color:");
	ObjectColor.Color_DEBUG();

	//Vertex information
	DEBUG("Vertices:");
	for (j = 0; j < number_vertices; j++){
		DEBUGf("Vertex(#%d):", j);
		Vertex_DEBUG(LocalVertices[j]);
		DEBUGf("Translated Vertex(#%d):", j);
		Vertex_DEBUG(TransVertices[j]);
		DEBUGf("Vertex Normal(#%d):", j);
		VertexNormals[j].Vector_DEBUG();
	};

	//Polygon information
	DEBUG("Faces:");
	for (j = 0; j < number_faces; j++){
		DEBUGf("Face(#%d):", j);
		Faces[j].ObjectPolygon_DEBUG();
	};
}

//************************************************
void Model3D::HandleInput(DS2_Input &Input){
	short xr=0, yr=0, zr=0;

	if (Input.KEY(DIK_W)){
		xr--;
	}else if (Input.KEY(DIK_E)){
		xr++;
	};

	if (Input.KEY(DIK_S)){
		yr--;
	}else if (Input.KEY(DIK_D)){
		yr++;
	};

	if (Input.KEY(DIK_X)){
		zr--;
	}else if (Input.KEY(DIK_C)){
		zr++;
	};

	RotateXYZ(xr, yr, zr);
}
