#include <vector>
#include <memory>
#include <MainGame.h>
#include <GameEngine.h>
#include <Vector3.h>
#include <SDL/SDL.h>

#undef main /* We don’t want SDL to override our main() */

int main(int argc, char** argv)
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
