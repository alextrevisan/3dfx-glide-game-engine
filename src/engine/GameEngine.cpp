#include "GameEngine.h"
#include <BehaviourList.h>

GameEngine::GameEngine()
:IsRunning(true),MainEngine(GlideEngine())
{
}

int GameEngine::Start()
{
    int totalFrames = 600;
    for(MonoBehaviour* item: BehaviourList::GetInstance().Items())
    {
        item->Start();
    }
    while(IsRunning && totalFrames--)
    {
        for(MonoBehaviour* item: BehaviourList::GetInstance().Items())
        {
            item->Update();
        }
        SDL_Delay(16);
        BehaviourList::GetInstance().Apply();
        //IsRunning = false;
    }

    return 0;
}

GameEngine::~GameEngine()
{
    for(auto* item: BehaviourList::GetInstance().Items())
    {
        delete item;
    }


    //dtor
}
