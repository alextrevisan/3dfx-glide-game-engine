#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <GameObject.h>
#include <MonoBehaviour.h>
class MeshRenderer: public GameObject, MonoBehaviour
{
public:
    MeshRenderer();
    ~MeshRenderer(){};
    void Update() final;

};

#endif // MESHRENDERER_H
