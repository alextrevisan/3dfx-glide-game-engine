#ifndef __VERTEX_H__
#define __VERTEX_H__
typedef struct {
  float  sow;                   /* s texture ordinate (s over w) */
  float  tow;                   /* t texture ordinate (t over w) */
  float  oow;                   /* 1/w (used mipmapping - really 0xfff/w) */
}  GrTmuVertex;

typedef struct
{
  float x, y, z;                /* X, Y, and Z of scrn space -- Z is ignored */
  float r, g, b;                /* R, G, B, ([0..255.0]) */
  float ooz;                    /* 65535/Z (used for Z-buffering) */
  float a;                      /* Alpha [0..255.0] */
  float oow;                    /* 1/W (used for W-buffering, texturing) */
  GrTmuVertex  tmuvtx[GLIDE_NUM_TMU];
} GrVertex;

typedef struct{
	float x, y, z;  //0-4-8	//X, Y, Z for w-buffering
	float q;		//12	//Q index for fogging
	//float r, g, b;//16-20-24 //RGB triplets for color
	FxU32 color;	//16-20   //Packed ARGB color
	float u0, v0;	//20-28	//Texture coords for TMU0
	float u1, v1;	//28-36 //Texture coords for TMU1
	bool part_of_visible_polygon;  //Bool value for lighting optimization
}Vertex;

enum PolyShadingType{PolyShadingFlat, PolyShadingGouraud};
enum PolyFaceType{PolyFacePlain, PolyFaceTextured};
enum PolyMediumType{PolyMediumSolid, PolyMediumAlpha};
enum LastStateType{Last_NONE, Last_FLAT, Last_TEXTURE, Last_ALPHA, Last_TEXTURE_ALPHA};
#endif // __VERTEX_H__
