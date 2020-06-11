#ifndef GLIDEENGINE_H
#define GLIDEENGINE_H
#define WIN32_LEAN_AND_MEAN
#include <string_view>
#include <windows.h>
#include <glide.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

class GlideEngine
{
public:
    GlideEngine();
    ~GlideEngine();
    HWND hWndMain;
    GrContext_t glideContext;

    void SetTextureMode();
    void SetFlatMode();
    void setAlphaMode();
    void setTextureAlphaMode();

    static void LoadTexture(const char* filename);
protected:

private:
    int VoodooType();

    void InitSDLSubsystem();
    void InitGlideSubsystem();

    /** SDL Subsystem **/
    SDL_Surface* window;



    /** Glide Subsystem **/
    bool Has3dfxHardware();
    void SetupResolutions();
    void GlideSetupVertexLayout();
    void GlideSetupTexture();
    void SetupTextures();


    GrContext_t gc = 0;
    GrResolution query, *resolutionList;
    GrScreenResolution_t resolution = GR_RESOLUTION_1024x768;
    int width = 0, height = 0;
    FxI32 wLimits[2];

    static constexpr int GL_VOODOO_UNDEF  = 1;
    static constexpr int GL_VOODOO        = 1;
    static constexpr int GL_VOODOORUSH    = 2;
    static constexpr int GL_VOODOO2       = 3;
    static constexpr int GL_VOODOOBANSHEE = 4;

    BOOL IsWindowMode = FALSE;
    WINDOWPLACEMENT wpc;
    LONG HWNDStyle = 0;
    LONG HWNDStyleEx = 0;

    void FullScreenSwitch( );

    static constexpr auto INITIAL_TEXARRAY_SIZE = 10;
    struct Glide_Texture
    {
        GrTexInfo TexInfo;    //Glide texture size info
        FxU32 TMU0_MemoryAddress;  //Texture map unit 0 address
        FxU32 TMU1_MemoryAddress;  //Texture map unit 1 address
        void *TextureData;	//Actual texture data
    } ;
    static Glide_Texture TextureArray[INITIAL_TEXARRAY_SIZE];
    static unsigned short NumberOfTextures;
    FxI32 NumberOfTMUs = 0;
    static FxU32 TMU0_MemoryAddress;
    static FxU32 Next_TMU0_MemoryAddress;
};


#endif // GLIDEENGINE_H
