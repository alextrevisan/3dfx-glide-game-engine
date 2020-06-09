#ifndef MAINGAME_H
#define MAINGAME_H
#include <MonoBehaviour.h>
#include <glide3x_dll.h>
typedef struct Vertex
{
  // x,y,z of vertex
  float x;    // 0
  float y;    // 4

  float z;    // 8

  float ooz;  // 12

  float q;    // 16

  // Vertex Color
  float r;    // 20
  float g;    // 24
  float b;    // 28

  // Texture Coordinates for tmu 0
  float u0,v0;  // 32

  // Texture Coordinates for tmu 1
  float u1,v1;  // 40

  // In how many polygons this vertex is present:
  int NumPolys;

  // pointer to list of polygon indices of polygons that have this
  // vertex in common.
  //fxePolygonIndex *PolygonIndices;

	// Flag that indicates if this vertex must be performed
	// lightning calculations on.
	int Used;
} Vertex;

typedef struct Triangle
{
	Vertex v1;
	Vertex v2;
	Vertex v3;
} Triangle;

class MainGame: public MonoBehaviour
{
    public:
        MainGame();

        void Update();
    protected:
        virtual ~MainGame();
        Triangle GourTris;
};

#endif // MAINGAME_H
