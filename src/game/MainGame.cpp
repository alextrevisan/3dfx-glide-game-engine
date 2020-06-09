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
    GourTris.v1.r = (float)(255-loop*25);
	GourTris.v1.g = (float)(loop*25);
    GourTris.v1.b = (float)(loop*25);
    GourTris.v2.r = (float)(loop*25);
	GourTris.v2.g = (float)(255-loop*25);
    GourTris.v2.b = (float)(255-loop*25);
    GourTris.v3.r = (float)(100-loop*10);
	GourTris.v3.g = (float)(loop*10);
    GourTris.v3.b = (float)(loop*10);

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
