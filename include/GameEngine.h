#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <vector>
#include <MonoBehaviour.h>

class GameEngine
{
    public:
        GameEngine();
        int Start();
        virtual ~GameEngine();
    private:
        bool IsRunning;
        GlideEngine MainEngine;
};

#endif // GAMEENGINE_H
