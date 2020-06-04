#include "GlideEngine.h"
#include "glide3x_dll.h"
#include <cassert>
#include <stdexcept>

GlideEngine::GlideEngine()
{
    init_glide3x_dll();
    InitSDLSubsystem();
    InitGlideSubsystem();
    GlideSetupVertexLayout();
    GlideSetupTexture();
}

int GlideEngine::VoodooType()
{
    const char *vstr;

    vstr = grGetString(GR_HARDWARE);
    if (!strcmp(vstr, "Voodoo2") ||
            !strcmp(vstr, "Voodoo Banshee") ||
            !strcmp(vstr, "Voodoo Graphics") ||
            !strcmp(vstr, "Voodoo Rush") ||
            !strcmp(vstr, "Voodoo5 (tm)"))
        return 1;
    return 0;
}

void GlideEngine::InitSDLSubsystem()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_SetVideoMode(1024, 768, 0, 0);

    SDL_SysWMinfo wmInfo;
    SDL_GetWMInfo(&wmInfo);
    hWndMain= wmInfo.window;
}

bool GlideEngine::Has3dfxHardware()
{
    //Check for 3dfx board

    long numBoards = 0;
	if (!grGet(GR_NUM_BOARDS, sizeof(numBoards), &numBoards)){
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
            if (grQueryResolutions(&query, &resolutionList[0]) == 0){
                continue;
            };
            for(int i=0;i<list_size;++i)
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
    SetupResolutions();
    assert( VoodooType() );
    glideContext = grSstWinOpen((FxU32)hWndMain,
                                GR_RESOLUTION_1024x768, GR_REFRESH_60Hz,
                                GR_COLORFORMAT_ABGR,
                                GR_ORIGIN_LOWER_LEFT,
                                2, 1);
    if (glideContext == 0)
    {
        printf("Could not allocate glide fullscreen context.\n");
        exit(-1);
    }
    //SDL_RestoreWindow(screen);
    //FullScreenSwitch( );
}

void GlideEngine::GlideSetupVertexLayout()
{
    FxU32 zrange[2]{0};
    grGet(GR_ZDEPTH_MIN_MAX, 8, (FxI32 *)zrange);
    grVertexLayout(GR_PARAM_XY,  0, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_Q,   GR_VERTEX_OOW_OFFSET << 2, GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_ST0, GR_VERTEX_SOW_TMU0_OFFSET << 2, GR_PARAM_ENABLE);
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


GlideEngine::~GlideEngine()
{
    grGlideShutdown();
    SDL_Quit();
}
