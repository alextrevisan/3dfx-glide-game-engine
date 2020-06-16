#ifndef MAINGAME_H
#define MAINGAME_H

#include <MonoBehaviour.h>
#include <GameObject.h>
#include <glide.h>
#include <Vertex.h>
#include <MeshFilter.h>

class MainGame: public MonoBehaviour
{
    public:
        MainGame();

        void Update();
    protected:
        virtual ~MainGame();
        Vertex v[8];
        int tenPercentMax, tenPercentMin, Median = 0;
        GameObject newCube{"CUBE"};
        MeshFilter meshFilter;
};


#endif // MAINGAME_H
