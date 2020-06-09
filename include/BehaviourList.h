#ifndef __BEHAVIOUR_LIST_H__
#define __BEHAVIOUR_LIST_H__

#include <vector>
#include <algorithm>
#include <MonoBehaviour.h>

class BehaviourList
{
public:
    static BehaviourList& GetInstance();

    const std::vector<MonoBehaviour*>& Items();

    void Add(MonoBehaviour* item);

    void Erase(MonoBehaviour* item);
    std::vector<MonoBehaviour*> MonoBehaviourList;
    std::vector<std::vector<MonoBehaviour*>::iterator> MonoBehaviourListToRemove;
    static BehaviourList* Instance;
    ~BehaviourList();
protected:
    void Apply();
    friend class GameEngine;
private:
    BehaviourList();
};

#endif // __BEHAVIOUR_LIST_H__
