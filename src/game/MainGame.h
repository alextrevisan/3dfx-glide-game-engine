#ifndef MAINGAME_H
#define MAINGAME_H

#include <MonoBehaviour.h>
#include <glide.h>

typedef struct
{
    FxFloat x2d,y2d,q;          // To 2D converted coordinates.  Q = 1 / Z
    FxFloat u,v;                // Texture Coordinates
    FxFloat x,y,z;              // X,Y,Z ( note that these coordinates are NOT used by Glide )
} Vertex;

class MainGame: public MonoBehaviour
{
    public:
        MainGame();

        void Update();
    protected:
        virtual ~MainGame();
        Vertex v[8];

};


#endif // MAINGAME_H
