#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <cstdint>
#include <Vertex.h>


struct Vector3
{
    Vector3(float a, float b, float c)
        :x(a), y(b), z(c)
    {}

    float x, y, z;

    Vertex toVertex() const
    {
        if(z != 0.0f)
        return {x/z, y/z, 1.0f/z,0,0,0,0,0};
    }
};

#endif //__VECTOR3_H__
