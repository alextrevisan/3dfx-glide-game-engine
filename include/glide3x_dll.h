#include <glide.h>
#ifndef __GLIDE3X_DLL__
#define __GLIDE3X_DLL__

#include <windows.h>
#include <stdexcept>
#ifdef __cplusplus
#define DECLTYPE(f) decltype(f)
extern "C" {
#else
#define DECLTYPE(v) void(*)(void)
#endif
    static HMODULE glide3xDllHandle = 0;
    static void init_glide3x_dll()
    {
        glide3xDllHandle = LoadLibrary("glide3x.dll");
        if(!glide3xDllHandle)
            throw std::runtime_error("No glide driver found!");
        //grAADrawTriangle = (DECLTYPE(grAADrawTriangle)) GetProcAddress(glide3xDllHandle, "_grAADrawTriangle@24");
        grAADrawTriangle = (DECLTYPE(grAADrawTriangle)) GetProcAddress(glide3xDllHandle, "_grAADrawTriangle@24");
        grAlphaBlendFunction = (DECLTYPE(grAlphaBlendFunction)) GetProcAddress(glide3xDllHandle, "_grAlphaBlendFunction@16");
        grAlphaCombine = (DECLTYPE(grAlphaCombine)) GetProcAddress(glide3xDllHandle, "_grAlphaCombine@20");
        grAlphaControlsITRGBLighting = (DECLTYPE(grAlphaControlsITRGBLighting)) GetProcAddress(glide3xDllHandle, "_grAlphaControlsITRGBLighting@4");
        grAlphaTestFunction = (DECLTYPE(grAlphaTestFunction)) GetProcAddress(glide3xDllHandle, "_grAlphaTestFunction@4");
        grAlphaTestReferenceValue = (DECLTYPE(grAlphaTestReferenceValue)) GetProcAddress(glide3xDllHandle, "_grAlphaTestReferenceValue@4");
        grBufferClear = (DECLTYPE(grBufferClear)) GetProcAddress(glide3xDllHandle, "_grBufferClear@12");
        grBufferSwap = (DECLTYPE(grBufferSwap)) GetProcAddress(glide3xDllHandle, "_grBufferSwap@4");
        //grCheckForRoom = (DECLTYPE(grCheckForRoom)) GetProcAddress(glide3xDllHandle, "_grCheckForRoom@4");
        grChromakeyMode = (DECLTYPE(grChromakeyMode)) GetProcAddress(glide3xDllHandle, "_grChromakeyMode@4");
        grChromakeyValue = (DECLTYPE(grChromakeyValue)) GetProcAddress(glide3xDllHandle, "_grChromakeyValue@4");
        grClipWindow = (DECLTYPE(grClipWindow)) GetProcAddress(glide3xDllHandle, "_grClipWindow@16");
        grColorCombine = (DECLTYPE(grColorCombine)) GetProcAddress(glide3xDllHandle, "_grColorCombine@20");
        grColorMask = (DECLTYPE(grColorMask)) GetProcAddress(glide3xDllHandle, "_grColorMask@8");
        grConstantColorValue = (DECLTYPE(grConstantColorValue)) GetProcAddress(glide3xDllHandle, "_grConstantColorValue@4");
        grCoordinateSpace = (DECLTYPE(grCoordinateSpace)) GetProcAddress(glide3xDllHandle, "_grCoordinateSpace@4");
        grCullMode = (DECLTYPE(grCullMode)) GetProcAddress(glide3xDllHandle, "_grCullMode@4");
        grDepthBiasLevel = (DECLTYPE(grDepthBiasLevel)) GetProcAddress(glide3xDllHandle, "_grDepthBiasLevel@4");
        grDepthBufferFunction = (DECLTYPE(grDepthBufferFunction)) GetProcAddress(glide3xDllHandle, "_grDepthBufferFunction@4");
        grDepthBufferMode = (DECLTYPE(grDepthBufferMode)) GetProcAddress(glide3xDllHandle, "_grDepthBufferMode@4");
        grDepthMask = (DECLTYPE(grDepthMask)) GetProcAddress(glide3xDllHandle, "_grDepthMask@4");
        grDepthRange = (DECLTYPE(grDepthRange)) GetProcAddress(glide3xDllHandle, "_grDepthRange@8");
        grDisable = (DECLTYPE(grDisable)) GetProcAddress(glide3xDllHandle, "_grDisable@4");
        grDisableAllEffects = (DECLTYPE(grDisableAllEffects)) GetProcAddress(glide3xDllHandle, "_grDisableAllEffects@0");
        grDitherMode = (DECLTYPE(grDitherMode)) GetProcAddress(glide3xDllHandle, "_grDitherMode@4");
        grDrawLine = (DECLTYPE(grDrawLine)) GetProcAddress(glide3xDllHandle, "_grDrawLine@8");
        grDrawPoint = (DECLTYPE(grDrawPoint)) GetProcAddress(glide3xDllHandle, "_grDrawPoint@4");
        grDrawTriangle = (DECLTYPE(grDrawTriangle)) GetProcAddress(glide3xDllHandle, "_grDrawTriangle@12");
        grDrawVertexArray = (DECLTYPE(grDrawVertexArray)) GetProcAddress(glide3xDllHandle, "_grDrawVertexArray@12");
        grDrawVertexArrayContiguous = (DECLTYPE(grDrawVertexArrayContiguous)) GetProcAddress(glide3xDllHandle, "_grDrawVertexArrayContiguous@16");
        grEnable = (DECLTYPE(grEnable)) GetProcAddress(glide3xDllHandle, "_grEnable@4");
        grErrorSetCallback = (DECLTYPE(grErrorSetCallback)) GetProcAddress(glide3xDllHandle, "_grErrorSetCallback@4");
        grFinish = (DECLTYPE(grFinish)) GetProcAddress(glide3xDllHandle, "_grFinish@0");
        grFlush = (DECLTYPE(grFlush)) GetProcAddress(glide3xDllHandle, "_grFlush@0");
        grFogColorValue = (DECLTYPE(grFogColorValue)) GetProcAddress(glide3xDllHandle, "_grFogColorValue@4");
        grFogMode = (DECLTYPE(grFogMode)) GetProcAddress(glide3xDllHandle, "_grFogMode@4");
        grFogTable = (DECLTYPE(grFogTable)) GetProcAddress(glide3xDllHandle, "_grFogTable@4");
        grGet = (DECLTYPE(grGet)) GetProcAddress(glide3xDllHandle, "_grGet@12");
        grGetProcAddress = (DECLTYPE(grGetProcAddress)) GetProcAddress(glide3xDllHandle, "_grGetProcAddress@4");
        grGetString = (DECLTYPE(grGetString)) GetProcAddress(glide3xDllHandle, "_grGetString@4");
        grGlideGetState = (DECLTYPE(grGlideGetState)) GetProcAddress(glide3xDllHandle, "_grGlideGetState@4");
        grGlideGetVertexLayout = (DECLTYPE(grGlideGetVertexLayout)) GetProcAddress(glide3xDllHandle, "_grGlideGetVertexLayout@4");
        grGlideInit = (DECLTYPE(grGlideInit)) GetProcAddress(glide3xDllHandle, "_grGlideInit@0");
        grGlideSetState = (DECLTYPE(grGlideSetState)) GetProcAddress(glide3xDllHandle, "_grGlideSetState@4");
        grGlideSetVertexLayout = (DECLTYPE(grGlideSetVertexLayout)) GetProcAddress(glide3xDllHandle, "_grGlideSetVertexLayout@4");
        grGlideShutdown = (DECLTYPE(grGlideShutdown)) GetProcAddress(glide3xDllHandle, "_grGlideShutdown@0");
        grLfbConstantAlpha = (DECLTYPE(grLfbConstantAlpha)) GetProcAddress(glide3xDllHandle, "_grLfbConstantAlpha@4");
        grLfbConstantDepth = (DECLTYPE(grLfbConstantDepth)) GetProcAddress(glide3xDllHandle, "_grLfbConstantDepth@4");
        grLfbLock = (DECLTYPE(grLfbLock)) GetProcAddress(glide3xDllHandle, "_grLfbLock@24");
        grLfbReadRegion = (DECLTYPE(grLfbReadRegion)) GetProcAddress(glide3xDllHandle, "_grLfbReadRegion@28");
        grLfbUnlock = (DECLTYPE(grLfbUnlock)) GetProcAddress(glide3xDllHandle, "_grLfbUnlock@8");
        grLfbWriteColorFormat = (DECLTYPE(grLfbWriteColorFormat)) GetProcAddress(glide3xDllHandle, "_grLfbWriteColorFormat@4");
        grLfbWriteColorSwizzle = (DECLTYPE(grLfbWriteColorSwizzle)) GetProcAddress(glide3xDllHandle, "_grLfbWriteColorSwizzle@8");
        grLfbWriteRegion = (DECLTYPE(grLfbWriteRegion)) GetProcAddress(glide3xDllHandle, "_grLfbWriteRegion@36");
        grLoadGammaTable = (DECLTYPE(grLoadGammaTable)) GetProcAddress(glide3xDllHandle, "_grLoadGammaTable@16");
        grQueryResolutions = (DECLTYPE(grQueryResolutions)) GetProcAddress(glide3xDllHandle, "_grQueryResolutions@8");
        grRenderBuffer = (DECLTYPE(grRenderBuffer)) GetProcAddress(glide3xDllHandle, "_grRenderBuffer@4");
        grReset = (DECLTYPE(grReset)) GetProcAddress(glide3xDllHandle, "_grReset@4");
        grSelectContext = (DECLTYPE(grSelectContext)) GetProcAddress(glide3xDllHandle, "_grSelectContext@4");
        grSetNumPendingBuffers = (DECLTYPE(grSetNumPendingBuffers)) GetProcAddress(glide3xDllHandle, "_grSetNumPendingBuffers@4");
        grSplash = (DECLTYPE(grSplash)) GetProcAddress(glide3xDllHandle, "_grSplash@20");
        //grSstConfigPipeline = (DECLTYPE(grSstConfigPipeline)) GetProcAddress(glide3xDllHandle, "_grSstConfigPipeline@12");
        grSstOrigin = (DECLTYPE(grSstOrigin)) GetProcAddress(glide3xDllHandle, "_grSstOrigin@4");
        grSstSelect = (DECLTYPE(grSstSelect)) GetProcAddress(glide3xDllHandle, "_grSstSelect@4");
        //grSstVidMode = (DECLTYPE(grSstVidMode)) GetProcAddress(glide3xDllHandle, "_grSstVidMode@8");
        grSstWinClose = (DECLTYPE(grSstWinClose)) GetProcAddress(glide3xDllHandle, "_grSstWinClose@4");
        grSstWinOpen = (DECLTYPE(grSstWinOpen)) GetProcAddress(glide3xDllHandle, "_grSstWinOpen@28");
        grStippleMode = (DECLTYPE(grStippleMode)) GetProcAddress(glide3xDllHandle, "_grStippleMode@4");
        grStipplePattern = (DECLTYPE(grStipplePattern)) GetProcAddress(glide3xDllHandle, "_grStipplePattern@4");
        grTexCalcMemRequired = (DECLTYPE(grTexCalcMemRequired)) GetProcAddress(glide3xDllHandle, "_grTexCalcMemRequired@16");
        grTexClampMode = (DECLTYPE(grTexClampMode)) GetProcAddress(glide3xDllHandle, "_grTexClampMode@12");
        grTexCombine = (DECLTYPE(grTexCombine)) GetProcAddress(glide3xDllHandle, "_grTexCombine@28");
        grTexDetailControl = (DECLTYPE(grTexDetailControl)) GetProcAddress(glide3xDllHandle, "_grTexDetailControl@16");
        grTexDownloadMipMap = (DECLTYPE(grTexDownloadMipMap)) GetProcAddress(glide3xDllHandle, "_grTexDownloadMipMap@16");
        grTexDownloadMipMapLevel = (DECLTYPE(grTexDownloadMipMapLevel)) GetProcAddress(glide3xDllHandle, "_grTexDownloadMipMapLevel@32");
        grTexDownloadMipMapLevelPartial = (DECLTYPE(grTexDownloadMipMapLevelPartial)) GetProcAddress(glide3xDllHandle, "_grTexDownloadMipMapLevelPartial@40");
        grTexDownloadTable = (DECLTYPE(grTexDownloadTable)) GetProcAddress(glide3xDllHandle, "_grTexDownloadTable@8");
        grTexDownloadTablePartial = (DECLTYPE(grTexDownloadTablePartial)) GetProcAddress(glide3xDllHandle, "_grTexDownloadTablePartial@16");
        grTexFilterMode = (DECLTYPE(grTexFilterMode)) GetProcAddress(glide3xDllHandle, "_grTexFilterMode@12");
        grTexLodBiasValue = (DECLTYPE(grTexLodBiasValue)) GetProcAddress(glide3xDllHandle, "_grTexLodBiasValue@8");
        grTexMaxAddress = (DECLTYPE(grTexMaxAddress)) GetProcAddress(glide3xDllHandle, "_grTexMaxAddress@4");
        grTexMinAddress = (DECLTYPE(grTexMinAddress)) GetProcAddress(glide3xDllHandle, "_grTexMinAddress@4");
        grTexMipMapMode = (DECLTYPE(grTexMipMapMode)) GetProcAddress(glide3xDllHandle, "_grTexMipMapMode@12");
        grTexMultibase = (DECLTYPE(grTexMultibase)) GetProcAddress(glide3xDllHandle, "_grTexMultibase@8");
        grTexMultibaseAddress = (DECLTYPE(grTexMultibaseAddress)) GetProcAddress(glide3xDllHandle, "_grTexMultibaseAddress@20");
        grTexNCCTable = (DECLTYPE(grTexNCCTable)) GetProcAddress(glide3xDllHandle, "_grTexNCCTable@4");
        grTexSource = (DECLTYPE(grTexSource)) GetProcAddress(glide3xDllHandle, "_grTexSource@16");
        grTexTextureMemRequired = (DECLTYPE(grTexTextureMemRequired)) GetProcAddress(glide3xDllHandle, "_grTexTextureMemRequired@8");
        grVertexLayout = (DECLTYPE(grVertexLayout)) GetProcAddress(glide3xDllHandle, "_grVertexLayout@12");
        grViewport = (DECLTYPE(grViewport)) GetProcAddress(glide3xDllHandle, "_grViewport@16");
        gu3dfGetInfo = (DECLTYPE(gu3dfGetInfo)) GetProcAddress(glide3xDllHandle, "_gu3dfGetInfo@8");
        gu3dfLoad = (DECLTYPE(gu3dfLoad)) GetProcAddress(glide3xDllHandle, "_gu3dfLoad@8");
        //guEncodeRLE16 = (DECLTYPE(guEncodeRLE16)) GetProcAddress(glide3xDllHandle, "_guEncodeRLE16@16");
        guFogGenerateExp2 = (DECLTYPE(guFogGenerateExp2)) GetProcAddress(glide3xDllHandle, "_guFogGenerateExp2@8");
        guFogGenerateExp = (DECLTYPE(guFogGenerateExp)) GetProcAddress(glide3xDllHandle, "_guFogGenerateExp@8");
        guFogGenerateLinear = (DECLTYPE(guFogGenerateLinear)) GetProcAddress(glide3xDllHandle, "_guFogGenerateLinear@12");
        guFogTableIndexToW = (DECLTYPE(guFogTableIndexToW)) GetProcAddress(glide3xDllHandle, "_guFogTableIndexToW@4");
        guGammaCorrectionRGB = (DECLTYPE(guGammaCorrectionRGB)) GetProcAddress(glide3xDllHandle, "_guGammaCorrectionRGB@12");
    }

#ifdef __cplusplus
}
#endif
#endif // __GLIDE3X_DLL__
