#include "MainGame.h"
#include <math.h>

#include <glide.h>

using namespace std;
GrTexInfo bgDecal;
void guLoadTexture(char* filename)
{
        Gu3dfInfo bgInfo;

        // Get header info
        gu3dfGetInfo( filename, &bgInfo );
        // Allocate some memory
        bgInfo.data = (char*)malloc( bgInfo.mem_required );
        // Load the texture
        gu3dfLoad( filename, &bgInfo);
        // Put everything from bgInfo to bgDecal ( the real texture var)
        bgDecal.smallLodLog2 = bgInfo.header.small_lod;
        bgDecal.largeLodLog2 = bgInfo.header.large_lod;
        bgDecal.aspectRatioLog2 =  bgInfo.header.aspect_ratio;
        bgDecal.data =		   bgInfo.data;
        bgDecal.format =	   bgInfo.header.format;

        // Download from System RAM to card
        grTexDownloadMipMap(GR_TMU0,grTexMinAddress(GR_TMU0),GR_MIPMAPLEVELMASK_BOTH,&bgDecal);
        // Tell Glide that the texture is at the MINADDRESS of TMU ZERO
        grTexSource(GR_TMU0,grTexMinAddress(GR_TMU0),GR_MIPMAPLEVELMASK_BOTH,&bgDecal);
        free( bgInfo.data );
}

MainGame::MainGame()
{
//    GlideEngine::LoadTexture("tex.3df");
    grVertexLayout(GR_PARAM_XY,    0,  GR_PARAM_ENABLE);
        grVertexLayout(GR_PARAM_Q,      8, GR_PARAM_ENABLE);
        grVertexLayout(GR_PARAM_ST0,   12, GR_PARAM_ENABLE);

        guLoadTexture("tex.3df");

        // No we don't want to alter texture : just use this.
        grTexCombine(GR_TMU0,
                     GR_COMBINE_FUNCTION_LOCAL,
                     GR_COMBINE_FACTOR_NONE,
                     GR_COMBINE_FUNCTION_LOCAL,
                     GR_COMBINE_FACTOR_NONE,
                     FXFALSE,
                     FXFALSE );

        // Set up iterated colors
        grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
                       GR_COMBINE_FACTOR_ONE,
                       GR_COMBINE_LOCAL_NONE,
                       GR_COMBINE_OTHER_TEXTURE,
                       FXFALSE);

        // Cull hiden faces
        grCullMode(GR_CULL_POSITIVE);

    // Set up coordinates and color
    v[0].x = -50.0f; v[0].y =  50.0f; v[0].z = -50.0f;
    v[1].x =  50.0f; v[1].y =  50.0f; v[1].z = -50.0f;
    v[2].x =  50.0f; v[2].y = -50.0f; v[2].z = -50.0f;
    v[3].x = -50.0f; v[3].y = -50.0f; v[3].z = -50.0f;
    v[4].x = -50.0f; v[4].y =  50.0f; v[4].z =  50.0f;
    v[5].x =  50.0f; v[5].y =  50.0f; v[5].z =  50.0f;
    v[6].x =  50.0f; v[6].y = -50.0f; v[6].z =  50.0f;
    v[7].x = -50.0f; v[7].y = -50.0f; v[7].z =  50.0f;
}

void DrawFace(Vertex a, Vertex b, Vertex c, Vertex d)
{

    // Perspective divisions
    // formulas : X2d = X3d / (Z3d * ZSCALE)
    //			  Y2d = Y3d / (Z3d * ZSCALE)
    // The +300 means the cube is at depth 300
    // The 320 and 240 set the cube in the center of the screen

    a.x2d = 320 + a.x / (a.z+300) * 512; // These are the 3D to 2D conversions
    a.y2d = 240 + a.y / (a.z+300) * 512;
    b.x2d = 320 + b.x / (b.z+300) * 512;
    b.y2d = 240 + b.y / (b.z+300) * 512;
    c.x2d = 320 + c.x / (c.z+300) * 512;
    c.y2d = 240 + c.y / (c.z+300) * 512;
    d.x2d = 320 + d.x / (d.z+300) * 512;
    d.y2d = 240 + d.y / (d.z+300) * 512;

    // Perspective correction : we need a q
    a.q = 1 / (a.z+300); // Q = 1 / Z
    b.q = 1 / (b.z+300);
    c.q = 1 / (c.z+300);
    d.q = 1 / (d.z+300);

    // Perspective
    a.u = 0   / a.z;    a.v = 0    / a.z;  // Give values to u and v AND DIVIDE THEM BY Z
    b.u = 0   * b.q;    b.v = 255  * b.q;  //              or multiply by Q (rather do this)
    c.u = 255 * c.q;    c.v = 255  * c.q;
    d.u = 255 * d.q;    d.v = 0    * d.q;

    grDrawTriangle(&a,&b,&c);
    grDrawTriangle(&a,&c,&d);
}



void MainGame::Update()
{
    FxI32 wLimits;
    grGet(GR_WDEPTH_MIN_MAX, 8, &wLimits);
    grBufferClear(0x00FF0000,  0, wLimits);

    FxFloat TempX;
    FxFloat TempY;
    FxFloat TempZ;
    int i;
    float rad = -3.14/180;

    for ( i = 0; i < 8; i++)
    {
        TempX = v[i].x;
        TempZ = v[i].z;
        v[i].x = (FxFloat) (cos(rad)*TempX+sin(rad)*TempZ);
        v[i].z = (FxFloat)(-sin(rad)*TempX+cos(rad)*TempZ);
    }

    for (i = 0; i < 8; i++)
    {
        TempY = v[i].y;
        TempZ = v[i].z;
        v[i].y = (FxFloat) (cos(rad)*TempY+sin(rad*1.2f)*TempZ);
        v[i].z = (FxFloat)(-sin(rad)*TempY+cos(rad*1.2f)*TempZ);
    }

    // Render all 12 triangles
    DrawFace(v[0], v[1], v[2], v[3]);
    DrawFace(v[6], v[5], v[4], v[7]);
    DrawFace(v[0], v[4], v[5], v[1]);
    DrawFace(v[2], v[6], v[7], v[3]);
    DrawFace(v[1], v[5], v[6], v[2]);
    DrawFace(v[0], v[3], v[7], v[4]);

    grBufferSwap(1);
}


MainGame::~MainGame()
{
    //dtor
}
