#include "BehaviourList.h"

static BehaviourList& BehaviourList::GetInstance()
{
    if(!Instance)
        Instance = new BehaviourList();
    return *Instance;
}

std::vector<MonoBehaviour*>& BehaviourList::Items()
{
    return MonoBehaviourList;
}

void BehaviourList::Add(MonoBehaviour* item)
{
    MonoBehaviourList.push_back(item);
}

void BehaviourList::Erase(MonoBehaviour* item)
{
    const auto it = std::find(MonoBehaviourList.begin(), MonoBehaviourList.end(),item);
    if(it != MonoBehaviourList.end())
        MonoBehaviourList.erase(it);
}

BehaviourList::BehaviourList()
{

}
static BehaviourList* BehaviourList::Instance = nullptr;
