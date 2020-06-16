#include "MainGame.h"
#include <cmath>
#include <chrono>

#include <SDL/SDL.h>
#include <glide.h>

#include <MeshFilter.h>
#include <Matrix4.h>

using namespace std;
GrTexInfo bgDecal;
FxU32 decaladdress;
void guLoadTexture(const char* filename)
{
        Gu3dfInfo bgInfo;

        gu3dfGetInfo( filename, &bgInfo );
        bgInfo.data = (char*)malloc( bgInfo.mem_required );
        gu3dfLoad( filename, &bgInfo);
        bgDecal.smallLodLog2 = bgInfo.header.small_lod;
        bgDecal.largeLodLog2 = bgInfo.header.large_lod;
        bgDecal.aspectRatioLog2 =  bgInfo.header.aspect_ratio;
        bgDecal.data =		   bgInfo.data;
        bgDecal.format =	   bgInfo.header.format;

        // We'll discuss this in 'Texture memory management'
        decaladdress = grTexMinAddress(GR_TMU0) + grTexTextureMemRequired(GR_MIPMAPLEVELMASK_EVEN,
                                                                          &bgDecal);

        // Download from system memory to card for trilinear filtering
        grTexDownloadMipMap(GR_TMU0,grTexMinAddress(GR_TMU0),GR_MIPMAPLEVELMASK_EVEN,&bgDecal);
        grTexDownloadMipMap(GR_TMU1,grTexMinAddress(GR_TMU1),GR_MIPMAPLEVELMASK_ODD,&bgDecal);

        // Download it again, at a further address for decal texture mapping
        grTexDownloadMipMap(GR_TMU0,decaladdress,GR_MIPMAPLEVELMASK_BOTH,&bgDecal);

        free(bgInfo.data);
}



MainGame::MainGame()
{
//    GlideEngine::LoadTexture("tex.3df");
    grVertexLayout(GR_PARAM_XY,    0,  GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_Q,      8, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_ST0,   12, GR_PARAM_ENABLE);

    guLoadTexture("miro.3df");

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

    /** ENABLE TEXTURE FILTERING BILINEAR **/
    grTexFilterMode( 	GR_TMU0,
        GR_TEXTUREFILTER_BILINEAR,
        GR_TEXTUREFILTER_BILINEAR);

    /** ENABLE TEXTURE MIPMAP BILINEAR **/
    grTexMipMapMode( 	GR_TMU0,
        GR_MIPMAP_NEAREST,
        FXFALSE);

    // Set up coordinates and color
    v[0].x = -50.0f; v[0].y =  50.0f; v[0].z = -50.0f;
    v[1].x =  50.0f; v[1].y =  50.0f; v[1].z = -50.0f;
    v[2].x =  50.0f; v[2].y = -50.0f; v[2].z = -50.0f;
    v[3].x = -50.0f; v[3].y = -50.0f; v[3].z = -50.0f;
    v[4].x = -50.0f; v[4].y =  50.0f; v[4].z =  50.0f;
    v[5].x =  50.0f; v[5].y =  50.0f; v[5].z =  50.0f;
    v[6].x =  50.0f; v[6].y = -50.0f; v[6].z =  50.0f;
    v[7].x = -50.0f; v[7].y = -50.0f; v[7].z =  50.0f;
    meshFilter = newCube.AddComponent<MeshFilter>();
}

void DrawFace(Vertex a, Vertex b, Vertex c, Vertex d, const float depth)
{
    // Perspective divisions
    // formulas : X2d = X3d / (Z3d * ZSCALE)
    //			  Y2d = Y3d / (Z3d * ZSCALE)
    // The +300 means the cube is at depth 300
    // The 320 and 240 set the cube in the center of the screen

    a.x2d = 320 + a.x / (a.z+depth) * 512; // These are the 3D to 2D conversions
    a.y2d = 240 + a.y / (a.z+depth) * 512;
    b.x2d = 320 + b.x / (b.z+depth) * 512;
    b.y2d = 240 + b.y / (b.z+depth) * 512;
    c.x2d = 320 + c.x / (c.z+depth) * 512;
    c.y2d = 240 + c.y / (c.z+depth) * 512;
    d.x2d = 320 + d.x / (d.z+depth) * 512;
    d.y2d = 240 + d.y / (d.z+depth) * 512;

    // Perspective correction : we need a q
    a.q = 1 / (a.z+depth); // Q = 1 / Z
    b.q = 1 / (b.z+depth);
    c.q = 1 / (c.z+depth);
    d.q = 1 / (d.z+depth);

    // Perspective
    a.u = 0   / a.z;    a.v = 0    / a.z;  // Give values to u and v AND DIVIDE THEM BY Z
    b.u = 0   * b.q;    b.v = 255  * b.q;  //              or multiply by Q (rather do this)
    c.u = 255 * c.q;    c.v = 255  * c.q;
    d.u = 255 * d.q;    d.v = 0    * d.q;

    grDrawTriangle(&a,&b,&c);
    grDrawTriangle(&a,&c,&d);
}

enum MipMapMode
{
    DISABLE = 0,
    NEAREST = 1,
    TRILINEAR = 2
};

long count = 0;
void MainGame::Update()
{
    auto now = SDL_GetTicks();
    const auto MipMapMode = count++ < 200? DISABLE : count < 400 ? NEAREST : TRILINEAR;
    switch (MipMapMode)
    {
        // No mapmaps, so Decal texture mapping on TMU_0 means :
        case DISABLE : // No mipmaps for TMU 0
                       grTexMipMapMode(GR_TMU0,
                                       GR_MIPMAP_DISABLE,
                                       FXFALSE);
                       // Set the texture source on TMU 0 to decal
                       grTexSource(GR_TMU0,
                                   decaladdress,
                                   GR_MIPMAPLEVELMASK_BOTH,
                                   &bgDecal);
                       // TMU 0 : perform decal texture mapping
                       grTexCombine(GR_TMU0,
                                    GR_COMBINE_FUNCTION_LOCAL,
                                    GR_COMBINE_FACTOR_NONE,
                                    GR_COMBINE_FUNCTION_LOCAL,
                                    GR_COMBINE_FACTOR_NONE,
                                    FXFALSE,
                                    FXFALSE);
                       // TMU 1 : goto sleep ( at least, the texture combine unit of TMU 1 does )
                       grTexCombine(GR_TMU1,
                                    GR_COMBINE_FUNCTION_NONE,
                                    GR_COMBINE_FACTOR_NONE,
                                    GR_COMBINE_FUNCTION_NONE,
                                    GR_COMBINE_FACTOR_NONE,
                                    FXFALSE,
                                    FXFALSE);
                       break;
        // Nearest mipmapping on TMU_0 means :
        case NEAREST : // Choose nearest mipmap on TMU 0
                       grTexMipMapMode(GR_TMU0,
                                       GR_MIPMAP_NEAREST,
                                       FXFALSE);
                       // Set the texture source on TMU 0 to decal
                       grTexSource(GR_TMU0,
                                   decaladdress,
                                   GR_MIPMAPLEVELMASK_BOTH,
                                   &bgDecal);
                       // TMU 0 : perform decal texture mapping
                       grTexCombine(GR_TMU0,
                                    GR_COMBINE_FUNCTION_LOCAL,
                                    GR_COMBINE_FACTOR_NONE,
                                    GR_COMBINE_FUNCTION_LOCAL,
                                    GR_COMBINE_FACTOR_NONE,
                                    FXFALSE,
                                    FXFALSE);
                       // TMU 1 : goto sleep ( at least, the texture combine unit of TMU 1 does )
                       grTexCombine(GR_TMU1,
                                    GR_COMBINE_FUNCTION_NONE,
                                    GR_COMBINE_FACTOR_NONE,
                                    GR_COMBINE_FUNCTION_NONE,
                                    GR_COMBINE_FACTOR_NONE,
                                    FXFALSE,
                                    FXFALSE);
                       break;
        // Nearest mipmapping on both TMUs + LOD blending means
        case TRILINEAR : // Both TMUs perform nearest mipmapping + LOD blending
                         grTexMipMapMode(GR_TMU0,
                                         GR_MIPMAP_NEAREST,
                                         FXTRUE);
                         grTexMipMapMode(GR_TMU1,
                                         GR_MIPMAP_NEAREST,
                                         FXTRUE);
                         // Even for TMU 0
                         grTexSource(GR_TMU0,
                                     grTexMinAddress(GR_TMU0),
                                     GR_MIPMAPLEVELMASK_EVEN,
                                     &bgDecal);
                         // Odd for TMU 1
                         grTexSource(GR_TMU1,
                                     grTexMinAddress(GR_TMU1),
                                     GR_MIPMAPLEVELMASK_ODD ,
                                     &bgDecal);
                         // Tmu 0 : Do that blending !
                         grTexCombine(GR_TMU0,
                                      GR_COMBINE_FUNCTION_BLEND,
                                      GR_COMBINE_FACTOR_LOD_FRACTION,
                                      GR_COMBINE_FUNCTION_BLEND,
                                      GR_COMBINE_FACTOR_LOD_FRACTION,
                                      FXFALSE,
                                      FXFALSE);
                         // Upstream TMU : decal
                         grTexCombine(GR_TMU1,
                                      GR_COMBINE_FUNCTION_LOCAL,
                                      GR_COMBINE_FACTOR_NONE,
                                      GR_COMBINE_FUNCTION_LOCAL,
                                      GR_COMBINE_FACTOR_NONE,
                                      FXFALSE,
                                      FXFALSE);
                       break;
    }

    FxI32 wLimits;
    grGet(GR_WDEPTH_MIN_MAX, 8, &wLimits);
    grBufferClear(0x00FF0000,  0, wLimits);

    const float rad = count < 200? -3.14/180 : count < 400 ? 3.14/180 : -3.14/180;

    for (int  i = 0; i < 8; i++)
    {
        const FxFloat TempX = v[i].x;
        const FxFloat TempZ = v[i].z;
        v[i].x = (FxFloat) (cos(rad)*TempX+sin(rad)*TempZ);
        v[i].z = (FxFloat)(-sin(rad)*TempX+cos(rad)*TempZ);
    }

    for (int i = 0; i < 8; i++)
    {
        const FxFloat TempY = v[i].y;
        const FxFloat TempZ = v[i].z;
        v[i].y = (FxFloat) (cos(rad)*TempY+sin(rad*1.2f)*TempZ);
        v[i].z = (FxFloat)(-sin(rad)*TempY+cos(rad*1.2f)*TempZ);
    }
    float depth = (count < 300 ? count : 600 - count)/2.0f+150;
    // Render all 12 triangles
    DrawFace(v[0], v[1], v[2], v[3], depth);
    DrawFace(v[6], v[5], v[4], v[7], depth);
    DrawFace(v[0], v[4], v[5], v[1], depth);
    DrawFace(v[2], v[6], v[7], v[3], depth);
    DrawFace(v[1], v[5], v[6], v[2], depth);
    DrawFace(v[0], v[3], v[7], v[4], depth);
    const float fNear = 0.1f;
    const float fFar = 1000.f;
    const float fFov = 90.f;
    const float fAspectRatio = 800.f/600.f;
    const float fFovRad = 1.f / std::tan(fFov * 0.5f / 180.f * 3.1415f);
    const float fTheta = 0.1;
    const Matrix4 matProj({{
                              {{fAspectRatio * fFovRad,0,0,0}},
                              {{0,fFovRad,0,0}},
                              {{0,0,fFar / (fFar - fNear), 1.0f}},
                              {{0,0,(-fFar * fNear) / (fFar - fNear),0}},
                          }});
    const Matrix4 matRotZ({{
                              {{std::cos(fTheta),std::sin(fTheta),0,0}},
                              {{-std::sin(fTheta),std::cos(fTheta),0,0}},
                              {{0,0,1,0}},
                              {{0,0,0,1}},
                          }}),
                  MatRotX({{
                              {{1,0,0,0}},
                              {{0,std::cos(fTheta*0.5),std::sin(fTheta*0.5),0}},
                              {{0,-std::sin(fTheta*0.5),std::cos(fTheta*0.5),0}},
                              {{0,0,0,1}},
                          }});


    for(auto& tri: meshFilter.mesh)
    {
        auto translated = tri;
        translated[0].z += 3;
        translated[1].z += 3;
        translated[2].z += 3;
        Triangle projected{matProj * translated[0], matProj * translated[1], matProj * translated[2]};

        projected = {matRotZ * projected[0], matRotZ * projected[1], matRotZ * projected[2]};
        projected = {MatRotX * projected[0], MatRotX * projected[1], MatRotX * projected[2]};

        projected[0].x += 1.0f; projected[0].y += 1.0f; projected[0].z += 1.0f;
        projected[1].x += 1.0f; projected[1].y += 1.0f; projected[1].z += 1.0f;
        projected[2].x += 1.0f; projected[2].y += 1.0f; projected[2].z += 1.0f;

        projected[0].x *= 0.5f *300 ; projected[0].y *= 0.5f*400;
        projected[1].x *= 0.5f *300 ; projected[1].y *= 0.5f*400;
        projected[2].x *= 0.5f *300 ; projected[2].y *= 0.5f*400;


        Vertex v1[3] {
            {.x2d = projected[0].x, .y2d = projected[0].y, .q = projected[0].z},
            {.x2d = projected[1].x, .y2d = projected[1].y, .q = projected[1].z},
            {.x2d = projected[2].x, .y2d = projected[2].y, .q = projected[2].z},
            };
            v1[0].u = 0;    v1[0].v = 0;  // Give values to u and v AND DIVIDE THEM BY Z
            v1[1].u = 0;    v1[1].v = 255  * v1[1].q;  //              or multiply by Q (rather do this)
            v1[2].u = 255 * v1[2].q;    v1[2].v = 255  * v1[2].q;
        grDrawTriangle(&v1[0],&v1[1],&v1[2]);
    }
    auto end = SDL_GetTicks();
    grBufferSwap(1);

    Median += end - now;

    //SDL_WM_SetCaption(title, nullptr);
}


MainGame::~MainGame()
{
    char title[100];
    snprintf(title, 100, "Execution ticks = %d\n", Median);
    printf(title);
    //dtor
}
