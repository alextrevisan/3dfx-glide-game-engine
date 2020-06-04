#ifndef MONOBEHAVIOUR_H
#define MONOBEHAVIOUR_H
#include <GlideEngine.h>

class GameEngine;
class MonoBehaviour
{
    public:
        MonoBehaviour();
        virtual ~MonoBehaviour();

    protected:
        virtual void Start(){};
        virtual void Update(){};
        //virtual void FixedUpdate(){};
        //virtual void LateUpdate(){};
        //virtual void OnGUI(){};
        //virtual void OnDisable(){};
        //virtual void OnEnable(){};
    friend class GameEngine;
    private:
};

#endif // MONOBEHAVIOUR_H
