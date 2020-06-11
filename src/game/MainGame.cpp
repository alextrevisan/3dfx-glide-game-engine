#include "MainGame.h"

MainGame::MainGame()
{
    GlideEngine::LoadTexture("tex.3df");
}

void MainGame::Update()
{
    FxI32 wLimits;
    grGet(GR_WDEPTH_MIN_MAX, 8, &wLimits);
    grBufferClear(0xFF000000,  0, wLimits);

    grDrawVertexArray(GR_TRIANGLES, 1, Triangle);
    /*grDrawTriangle(&Triangle[0],&Triangle[1],
                   &Triangle[2]);*/
    grBufferSwap(1);
}


MainGame::~MainGame()
{
    //dtor
}
