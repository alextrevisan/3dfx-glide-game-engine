#include <vector>
#include <memory>
#include <MainGame.h>
#include <GameEngine.h>
#include <Vector3.h>

struct Color
{
    Color(const Color&) = delete;
    static constexpr int blue = 3;
};
class Component
{
    public:
    Component(){};
    Component(const Component&) = delete;
};
class Light: public Component
{
    public:
    Light():color(0){};
    Light(const Light&) = delete;
    int color;
};

class GameObject
{
    public:
    GameObject(const std::string& _name)
    :name(_name){}

    template<class T, typename ...N>
    constexpr T& AddComponent(N...args)
    {
        auto item = new T(args...);
        objects.emplace_back(item);
        return item;
    }

    struct transform
    {
        Vector3 position;
    }transform;

    //private:
    std::vector<Component*> objects;
    std::string name;

};

int main(int argc, char* argv[])
{
    try
    {
        GameEngine* engine = new GameEngine();
        MainGame* game = new MainGame();
        return engine->Start();
    }
    catch(const std::exception &ex)
    {
        printf("%s\n", ex.what());
    }

    return 0;
}
