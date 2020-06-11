#ifndef MAINGAME_H
#define MAINGAME_H
#include <Model3D.h>
#include <MonoBehaviour.h>
#include <glide3x_dll.h>


class MainGame: public MonoBehaviour
{
    public:
        MainGame();

        void Update();
    protected:
        virtual ~MainGame();
        Vertex Triangle[3];
        int vlist[3];
        GrTexInfo MipMap1;
        // texture memory startaddress on the TexelFx chip
        FxU32 TexStartAddress;
        Gu3dfInfo TexInfo;

        Model3D model;
};

#endif // MAINGAME_H
