#include "MonoBehaviour.h"
#include <BehaviourList.h>
#include <algorithm>
MonoBehaviour::MonoBehaviour()
{
    BehaviourList::GetInstance().Add(this);
}

MonoBehaviour::~MonoBehaviour()
{
    BehaviourList::GetInstance().Erase(this);
    //dtor
}
