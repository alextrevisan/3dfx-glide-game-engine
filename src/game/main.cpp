#include <vector>
#include <memory>

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
    
    template<class T>
    constexpr T& AddComponent()
    {
        objects[currentObjIdx] = T();
        auto* object = &objects[currentObjIdx++];
        return *static_cast<T*>(object);
    }

    struct transform
    {
        Vector3 position;
    }transform;

    //private:
    std::array<Component, 16> objects;
    uint8_t currentObjIdx = 0;
    std::string name;
    
};

int main()
{
    // Make a game object
    GameObject lightGameObject("The Light123412");// = new GameObject("The Light");

    // Add the light component
    auto& lightComp = lightGameObject.AddComponent<Light>();

    // Set color and position
    lightComp.color = Color::blue;

    // Set the position (or any transform property)
    lightGameObject.transform.position = Vector3(0, 5, 0);
    
    return sizeof(std::vector<Component>);
}