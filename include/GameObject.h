#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include <vector>

#include <Vector3.h>
#include <Component.h>

class GameObject
{
    public:
    GameObject(const std::string& _name);

    template<class T, typename ...N>
    constexpr T& AddComponent(N...args)
    {
        auto item = new T(args...);
        objects.push_back(item);
        return *item;
    }

    struct transform
    {
        Vector3 position;
    }transform;

    //private:
    std::vector<Component*> objects;
    std::string name;

};

#endif // GAMEOBJECT_H
