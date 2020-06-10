#include "MainGame.h"

MainGame::MainGame()
{
    int loop = 0;
    GourTris.v1.x = 200.0;
    GourTris.v1.y = 200.0;
    GourTris.v2.x = 400.0;
    GourTris.v2.y = 200.0;
    GourTris.v3.x = 300.0;
    GourTris.v3.y = 400.0;

    GourTris.v1.r = (float)(255);
    GourTris.v1.g = (float)(0);
    GourTris.v1.b = (float)(0);

    GourTris.v2.r = (float)(0);
    GourTris.v2.g = (float)(255);
    GourTris.v2.b = (float)(0);

    GourTris.v3.r = (float)(0);
    GourTris.v3.g = (float)(0);
    GourTris.v3.b = (float)(255);


    GourTris.v1.oow = 1/100.0f;

// get the texture info:
    if (gu3dfGetInfo("tex.3df",&TexInfo) == FXTRUE)
    {
        // allocate memory for the mipmap:
        TexInfo.data = (void*)malloc(TexInfo.mem_required);
        // Load the mipmap from disk:
        gu3dfLoad("tex.3df",&TexInfo);
        MipMap1.smallLodLog2 = TexInfo.header.small_lod;
        MipMap1.smallLodLog2 = TexInfo.header.large_lod;
        MipMap1.aspectRatioLog2 = TexInfo.header.aspect_ratio;
        MipMap1.data = TexInfo.data;
        MipMap1.format = TexInfo.header.format;

        // download the mipmap levels to texture unit 1

        // we must find out the startaddresses of the texture memory on the
        // two texture units to be able to download the mipmaplevels on the
        // texture memory
        TexStartAddress = grTexMinAddress(GR_TMU0);

        // the actual downloading goes here
        grTexDownloadMipMap(GR_TMU0,TexStartAddress,
                            GR_MIPMAPLEVELMASK_BOTH,&MipMap1);


        // After the mipmap have been downloaded to texture memory
        // we must tell the texture units where to look for the
        // mipmap and how it is composed, we do this here:

        // tell texture unit 1 we have put the mipmaps at startaddress.
        grTexSource(GR_TMU0,TexStartAddress,GR_MIPMAPLEVELMASK_BOTH,
                    &MipMap1);


        // Now we must configure the TexelFx chip to output the texture
        // rgb colors to the color combine unit.
        grTexCombine(GR_TMU0,GR_COMBINE_FUNCTION_LOCAL,GR_COMBINE_FACTOR_NONE,
                     GR_COMBINE_FUNCTION_LOCAL,GR_COMBINE_FACTOR_NONE,
                     FXFALSE,FXFALSE);

        // we must also configure the color combine unit to use the texture
        // rgb values as input to the color combine unit
        grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER,
                       GR_COMBINE_FACTOR_ONE,GR_COMBINE_LOCAL_NONE,
                       GR_COMBINE_OTHER_TEXTURE,FXFALSE);

    }

}

void MainGame::Update()
{
    FxI32 wLimits;
    grGet(GR_WDEPTH_MIN_MAX, 8, &wLimits);
    grBufferClear(0xFF000000, 0, wLimits);


    grDrawTriangle(&GourTris.v1,&GourTris.v2,
                   &GourTris.v3);
    grBufferSwap(1);
}


MainGame::~MainGame()
{
    //dtor
}
