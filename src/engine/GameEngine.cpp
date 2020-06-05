#include "GameEngine.h"
#include <BehaviourList.h>

GameEngine::GameEngine()
:IsRunning(true),MainEngine(GlideEngine())
{
}

int GameEngine::Start()
{
    for(MonoBehaviour* item: BehaviourList::GetInstance().Items())
    {
        item->Start();
    }
    while(IsRunning)
    {
        for(MonoBehaviour* item: BehaviourList::GetInstance().Items())
        {
            item->Update();
        }
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
