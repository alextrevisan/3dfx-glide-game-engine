#include "BehaviourList.h"

BehaviourList& BehaviourList::GetInstance()
{
    if(!Instance)
        Instance = new BehaviourList();
    return *Instance;
}

const std::vector<MonoBehaviour*>& BehaviourList::Items()
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
    MonoBehaviourListToRemove.push_back(it);
}

void BehaviourList::Apply()
{
    for(auto it: MonoBehaviourListToRemove)
    {
        MonoBehaviourList.erase(it);
    }
    MonoBehaviourListToRemove.clear();
}



BehaviourList::BehaviourList()
{

}

BehaviourList::~BehaviourList()
{
    for(auto it: MonoBehaviourList)
    {
        delete it;
    }
    MonoBehaviourList.clear();
}
BehaviourList* BehaviourList::Instance = nullptr;
