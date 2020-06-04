#ifndef GLIDEENGINE_H
#define GLIDEENGINE_H
#define WIN32_LEAN_AND_MEAN
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
    GrContext_t gc = 0;
    GrResolution query, *resolutionList;
    GrScreenResolution_t resolution = GR_RESOLUTION_1024x768;

    static constexpr int GL_VOODOO_UNDEF  = 1;
    static constexpr int GL_VOODOO        = 1;
    static constexpr int GL_VOODOORUSH    = 2;
    static constexpr int GL_VOODOO2       = 3;
    static constexpr int GL_VOODOOBANSHEE = 4;

    float scrXScale = 1027.0f;
    float scrYScale = 768.0f;

    char *consoleGrid;
    int   consoleRows;
    int   consoleColumns;
    int   consoleX;
    int   consoleY;
    int   consoleColor;
    float consoleOriginX;
    float consoleOriginY;
    float consoleCharWidth;
    float consoleCharHeight;
    static constexpr unsigned char GR_VERTEX_X_OFFSET =            0;
    static constexpr unsigned char GR_VERTEX_Y_OFFSET =            1;
    static constexpr unsigned char GR_VERTEX_OOZ_OFFSET =          2;
    static constexpr unsigned char GR_VERTEX_OOW_OFFSET =          3;
    static constexpr unsigned char GR_VERTEX_R_OFFSET =            4;
    static constexpr unsigned char GR_VERTEX_G_OFFSET =            5;
    static constexpr unsigned char GR_VERTEX_B_OFFSET =            6;
    static constexpr unsigned char GR_VERTEX_A_OFFSET =            7;
    static constexpr unsigned char GR_VERTEX_Z_OFFSET =            8;
    static constexpr unsigned char GR_VERTEX_SOW_TMU0_OFFSET =     9;
    static constexpr unsigned char GR_VERTEX_TOW_TMU0_OFFSET =     10;
    static constexpr unsigned char GR_VERTEX_OOW_TMU0_OFFSET =     11;
    static constexpr unsigned char GR_VERTEX_SOW_TMU1_OFFSET =     12;
    static constexpr unsigned char GR_VERTEX_TOW_TMU1_OFFSET =     13;
    static constexpr unsigned char GR_VERTEX_OOW_TMU1_OFFSET =     14;
    #if (GLIDE_NUM_TMU > 2)
    static constexpr unsigned char GR_VERTEX_SOW_TMU2_OFFSET =     15;
    static constexpr unsigned char GR_VERTEX_TOW_TMU2_OFFSET =     16;
    static constexpr unsigned char GR_VERTEX_OOW_TMU2_OFFSET =     17;
    #endif
    BOOL IsWindowMode = FALSE;
    WINDOWPLACEMENT wpc;
    LONG HWNDStyle = 0;
    LONG HWNDStyleEx = 0;

    void FullScreenSwitch( );
};


#endif // GLIDEENGINE_H
