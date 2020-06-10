#ifndef MAINGAME_H
#define MAINGAME_H
#include <MonoBehaviour.h>
#include <glide3x_dll.h>
typedef struct {
        float sow;		/* s texture ordinate (s over w) */
        float tow;		/* t texture ordinate (t over w) */
        float oow;		/* 1/w (used mipmapping - really 0xfff/w) */
} GrTmuVertex;

typedef struct {
  float x, y, z;   /* x, y, z of screen space. z is ignored */
  float ooz;       /* 65535/z (used for z buffering) */
  float oow;       /* 1/w (used for w buffering) */
  float r, g, b, a; /* red, green, blue, and alpha ([0..255.0]) */
  GrTmuVertex tmuvtx[GLIDE_NUM_TMU];
} GrVertex;

typedef struct Vertex
{
	GrVertex v1;
	GrVertex v2;
	GrVertex v3;
} Vertex;

class MainGame: public MonoBehaviour
{
    public:
        MainGame();

        void Update();
    protected:
        virtual ~MainGame();
        Vertex GourTris;
        GrTexInfo MipMap1;
        // texture memory startaddress on the TexelFx chip
        FxU32 TexStartAddress;
        Gu3dfInfo TexInfo;
};

#endif // MAINGAME_H
