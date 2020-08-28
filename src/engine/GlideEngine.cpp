#include "GlideEngine.h"
#include "glide3x_dll.h"

#include <string_view>
#include <cassert>
#include <stdexcept>

GlideEngine::GlideEngine()
{
    init_glide3x_dll();
    InitSDLSubsystem();
    InitGlideSubsystem();
}

int GlideEngine::VoodooType()
{
    const std::string_view vstr = grGetString(GR_HARDWARE);
    printf("voodoo found: %s\n",vstr.data());
    if (vstr == "Voodoo2" ||
            vstr == "Voodoo Banshee" ||
            vstr == "Voodoo Graphics" ||
            vstr == "Voodoo Rush" ||
            vstr == "Voodoo5 (tm)")
        return 1;
    return 0;
}

void GlideEngine::InitSDLSubsystem()
{
    switch(resolution)
    {
    case GR_RESOLUTION_640x400 :
        width=640;
        height=400;
        break;
    case GR_RESOLUTION_640x480 :
        width=640;
        height=480;
        break;
    case GR_RESOLUTION_800x600 :
        width=800;
        height=600;
        break;
    case GR_RESOLUTION_1024x768 :
        width=1024;
        height=768;
        break;
    };

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_SetVideoMode(width, height, 0, 0);

    SDL_SysWMinfo wmInfo;
    SDL_GetWMInfo(&wmInfo);
    hWndMain = wmInfo.window;
}

bool GlideEngine::Has3dfxHardware()
{
    //Check for 3dfx board

    long numBoards = 0;
    if (!grGet(GR_NUM_BOARDS, sizeof(numBoards), &numBoards))
    {
        throw std::runtime_error("Could not find any 3dfx hardware!");
    };
    return numBoards > 0;
}

void GlideEngine::SetupResolutions()
{

    for(int res = 0; res <= 0x19; ++res)
    {
        for(int refresh = 0; refresh <=0x8 ; ++refresh )
        {
            query.resolution = res;
            query.refresh = refresh;
            query.numColorBuffers = 2;
            query.numAuxBuffers = 1;
            //Query the resolution function
            const FxI32 list_size = grQueryResolutions(&query, NULL);
            GrResolution resolutionList[list_size];
            if (grQueryResolutions(&query, &resolutionList[0]) == 0)
            {
                continue;
            };
            for(int i=0; i<list_size; ++i)
            {
                if(resolutionList[i].resolution == res && resolutionList[i].refresh == refresh)
                    printf("RES: %d : REFRESH: %d\n", res, refresh);
            }
        }
    }
}


void GlideEngine::InitGlideSubsystem()
{
    if(!Has3dfxHardware())
        return;

    grGlideInit();
    grSstSelect(0);
    //SetupResolutions();
    VoodooType();
    glideContext = grSstWinOpen((FxU32)hWndMain,
                                resolution, GR_REFRESH_60Hz,
                                GR_COLORFORMAT_ARGB,
                                GR_ORIGIN_LOWER_LEFT,
                                2, 1);
    if (glideContext == 0)
    {
        throw std::runtime_error("Could not allocate glide fullscreen context.\n");
    }
    //SDL_RestoreWindow(screen);
    //FullScreenSwitch( );

    //Setup depth buffer
    /*grDepthBufferMode(GR_DEPTHBUFFER_WBUFFER);
    grDepthMask(FXTRUE);
    grDepthBufferFunction(GR_CMP_LEQUAL);
    SetFlatMode();

    //Setup misc stuff
    //grSstOrigin(GR_ORIGIN_UPPER_LEFT);
    //grCoordinateSpace(GR_CLIP_COORDS);

    GlideSetupVertexLayout();

    grViewport(0, 0, (FxI32)width,( FxI32)height);
    grDepthRange(0, 1);
    grGet(GR_WDEPTH_MIN_MAX, 8, (FxI32*)wLimits);
    grBufferClear(0, 0, wLimits[1]);
    grConstantColorValue(255<<24);

    SetupTextures();*/
}

void GlideEngine::SetTextureMode()
{
    //Configure color combine unit for texture iterated color
    grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL,
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE);

    //Turn off alpha combine unit(set to default)
    grAlphaCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE,
                    GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_CONSTANT, FXFALSE );
}

void GlideEngine::SetFlatMode()
{
    //Configure color combine unit for iterated color
    grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);

    //Turn off alpha combine unit(set to default)
    grAlphaCombine( GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_ONE,
                    GR_COMBINE_LOCAL_NONE, GR_COMBINE_OTHER_CONSTANT, FXFALSE );
}

void GlideEngine::setAlphaMode()
{
    //Configure color combine unit for iterated color
    grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);

    //Configure alpha combine and blend function
    grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_LOCAL_ALPHA,
                    GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE );
    grAlphaBlendFunction( GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA,
                          GR_BLEND_ZERO, GR_BLEND_ZERO );
}

void GlideEngine::setTextureAlphaMode()
{
    //Configure color combine unit for texture iterated color
    grColorCombine(GR_COMBINE_FUNCTION_SCALE_OTHER, GR_COMBINE_FACTOR_LOCAL,
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_TEXTURE, FXFALSE);

    //Configure alpha combine and blend function
    grAlphaCombine( GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_LOCAL_ALPHA,
                    GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE );
    grAlphaBlendFunction( GR_BLEND_SRC_ALPHA, GR_BLEND_ONE_MINUS_SRC_ALPHA,
                          GR_BLEND_ZERO, GR_BLEND_ZERO );
}


void GlideEngine::GlideSetupVertexLayout()
{
    //Setup vertex
    grVertexLayout(GR_PARAM_XY, 0, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_W, 8, GR_PARAM_ENABLE);
//	grVertexLayout(GR_PARAM_Q, 12, GR_PARAM_ENABLE);
//	grVertexLayout(GR_PARAM_RGB, 16, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_PARGB, 16, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_ST0, 20, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_ST1, 28, GR_PARAM_ENABLE);
}

void GlideEngine::GlideSetupTexture()
{
    grColorCombine( GR_COMBINE_FUNCTION_SCALE_OTHER,
                    GR_COMBINE_FACTOR_ONE,
                    GR_COMBINE_LOCAL_NONE,
                    GR_COMBINE_OTHER_TEXTURE,
                    FXFALSE );
    grTexMipMapMode( GR_TMU0,
                     GR_MIPMAP_NEAREST,
                     FXFALSE );
    grTexFilterMode( GR_TMU0,
                     GR_TEXTUREFILTER_BILINEAR,
                     GR_TEXTUREFILTER_BILINEAR );
}

void GlideEngine::FullScreenSwitch( )
{
    if ( IsWindowMode )
    {
        IsWindowMode = FALSE;
        GetWindowPlacement( hWndMain, &wpc );
        if ( HWNDStyle == 0 )
            HWNDStyle = GetWindowLong( hWndMain, GWL_STYLE );
        if ( HWNDStyleEx == 0 )
            HWNDStyleEx = GetWindowLong( hWndMain, GWL_EXSTYLE );

        LONG NewHWNDStyle = HWNDStyle;
        NewHWNDStyle &= ~WS_BORDER;
        NewHWNDStyle &= ~WS_DLGFRAME;
        NewHWNDStyle &= ~WS_THICKFRAME;

        LONG NewHWNDStyleEx =HWNDStyleEx;
        NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

        SetWindowLong( hWndMain, GWL_STYLE, NewHWNDStyle | WS_POPUP );
        SetWindowLong( hWndMain, GWL_EXSTYLE, NewHWNDStyleEx | WS_EX_TOPMOST );
        ShowWindow( hWndMain, SW_SHOWMAXIMIZED );
    }
    else
    {
        IsWindowMode = TRUE;
        SetWindowLong( hWndMain, GWL_STYLE, HWNDStyle );
        SetWindowLong( hWndMain, GWL_EXSTYLE, HWNDStyleEx );
        ShowWindow( hWndMain, SW_SHOWNORMAL );
        SetWindowPlacement( hWndMain, &wpc );
    }
}

void GlideEngine::SetupTextures()
{
    grGet(GR_NUM_TMU, sizeof(NumberOfTMUs),&NumberOfTMUs);

    //Setup bilinear filtering
    switch(NumberOfTMUs)
    {
    case 1 :
        //Setup texture unit 0 for bilinear filtering
        grTexFilterMode(GR_TMU0, GR_TEXTUREFILTER_BILINEAR, GR_TEXTUREFILTER_BILINEAR);

        //Setup for simple texture mapping
        grTexCombine(GR_TMU0, GR_COMBINE_FUNCTION_LOCAL,
                     GR_COMBINE_FACTOR_NONE,
                     GR_COMBINE_FUNCTION_LOCAL,
                     GR_COMBINE_FACTOR_NONE,
                     FXFALSE, FXFALSE);
        break;
    case 2 :
        //Setup texture unit 0/1 for bilinear filtering
        grTexFilterMode(GR_TMU0, GR_TEXTUREFILTER_BILINEAR, GR_TEXTUREFILTER_BILINEAR);
        grTexFilterMode(GR_TMU1, GR_TEXTUREFILTER_BILINEAR, GR_TEXTUREFILTER_BILINEAR);
        //Setup trilinear filtering
        grTexCombine(GR_TMU0, GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL,
                     GR_COMBINE_FACTOR_LOD_FRACTION,
                     GR_COMBINE_FUNCTION_SCALE_MINUS_LOCAL_ADD_LOCAL,
                     GR_COMBINE_FACTOR_LOD_FRACTION,
                     FXFALSE, FXFALSE);
        grTexCombine(GR_TMU1, GR_COMBINE_FUNCTION_LOCAL,
                     GR_COMBINE_FACTOR_NONE,
                     GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                     FXFALSE, FXFALSE);
        break;
    };

    //Set clamping mode
    grTexClampMode(GR_TMU0, GR_TEXTURECLAMP_CLAMP, GR_TEXTURECLAMP_CLAMP);

    //Setup texture downloads and memory addresses
    TMU0_MemoryAddress = grTexMinAddress(GR_TMU0);
    Next_TMU0_MemoryAddress = TMU0_MemoryAddress;

}

void GlideEngine::LoadTexture(const char* filename)
{
    Glide_Texture *currentTexture = nullptr;	//Pointer to a glide texture
    Gu3dfInfo TexInfo;
    GrTexInfo SizeInfo;
    FILE *fp;

    //Check to see if there is array room
    printf("Loading Texture::Glide Driver\n");
    printf("Filename:%s\n", filename);
    printf("# of textures:%d\n", NumberOfTextures);
    printf("Ready to start!\n");

    //Test to see if file is valid
    fp = fopen(filename, "r");
    if (!fp)
    {
        throw std::runtime_error("Texture name invalid!");
    };
    fclose(fp);


    if (NumberOfTextures < INITIAL_TEXARRAY_SIZE)
    {
        //Get pointer to current available texture
        currentTexture = &TextureArray[NumberOfTextures];

        //Get 3df file info
        gu3dfGetInfo(filename, &TexInfo);

        //Allocate memory for the texture
        //	TexInfo.data = (void *)malloc(TexInfo.mem_required);
        TexInfo.data = (void*) new char[TexInfo.mem_required];

        //Load actual file
        gu3dfLoad(filename, &TexInfo);

        //Transfer texture data into texture array
        currentTexture->TexInfo.data = TexInfo.data;
        currentTexture->TexInfo.format = TexInfo.header.format;
        currentTexture->TexInfo.aspectRatioLog2 = TexInfo.header.aspect_ratio;
        currentTexture->TexInfo.smallLodLog2 = TexInfo.header.small_lod;
        currentTexture->TexInfo.largeLodLog2 = TexInfo.header.large_lod;

        //Download texture
        grTexDownloadMipMap(GR_TMU0, Next_TMU0_MemoryAddress,
                            GR_MIPMAPLEVELMASK_BOTH, &currentTexture->TexInfo);

        //Get address
        currentTexture->TMU0_MemoryAddress = Next_TMU0_MemoryAddress;

        //Get size info information
        SizeInfo.data = TexInfo.data;
        SizeInfo.format	= TexInfo.header.format;
        SizeInfo.aspectRatioLog2 = TexInfo.header.aspect_ratio;
        SizeInfo.smallLodLog2 = TexInfo.header.small_lod;
        SizeInfo.largeLodLog2 = TexInfo.header.large_lod;

        //Increment download address
        Next_TMU0_MemoryAddress += grTexTextureMemRequired(GR_MIPMAPLEVELMASK_BOTH, &SizeInfo);

        //Increment number textures
        NumberOfTextures++;
    }
    else
    {

    };
}


GlideEngine::~GlideEngine()
{
    grGlideShutdown();
    SDL_Quit();
}

unsigned short GlideEngine::NumberOfTextures = 0;
GlideEngine::Glide_Texture GlideEngine::TextureArray[INITIAL_TEXARRAY_SIZE] = {};
FxU32 GlideEngine::TMU0_MemoryAddress = 0;
FxU32 GlideEngine::Next_TMU0_MemoryAddress = 0;
