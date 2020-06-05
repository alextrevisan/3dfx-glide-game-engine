#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <vector>
#include <stdexcept>

#include <MonoBehaviour.h>

class GameEngine
{
    public:
        GameEngine();
        int Start();
        template<class T, typename ...N>
        constexpr T& AddComponent(N...args)
        {
            if(mainComponent != nullptr)
                throw std::logic_error("Only one component is allowed in GameEngine");
            mainComponent = new T(args...);
            return *static_cast<T*>(mainComponent);
        }
    virtual ~GameEngine();
    private:
        bool IsRunning;
        GlideEngine MainEngine;
        MonoBehaviour* mainComponent = nullptr;

};

#endif // GAMEENGINE_H
