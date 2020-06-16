#ifndef MONOBEHAVIOUR_H
#define MONOBEHAVIOUR_H
#include <GlideEngine.h>
#include <Component.h>
class GameEngine;
class MonoBehaviour: public Component
{
    public:
        MonoBehaviour();
    private:


    protected:
        virtual void Start(){};
        virtual void Update(){};
        //virtual void FixedUpdate(){};
        //virtual void LateUpdate(){};
        //virtual void OnGUI(){};
        //virtual void OnDisable(){};
        //virtual void OnEnable(){};
        virtual ~MonoBehaviour();

        friend class GameEngine;
        friend class BehaviourList;
    private:
};

#endif // MONOBEHAVIOUR_H
