#include <vector>
#include <memory>
#include <MainGame.h>
#include <GameEngine.h>
#include <Vector3.h>
#include <SDL/SDL.h>

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

int main(int argc, char* argv[])
{
    try
    {
        GameEngine engine;
        engine.AddComponent<MainGame>();
        return engine.Start();

    }
    catch(const std::exception &ex)
    {
        MessageBox( NULL, ex.what(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);

        printf("%s\n", ex.what());
    }

    return 0;
}
