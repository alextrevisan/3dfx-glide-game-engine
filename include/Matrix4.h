#ifndef MATRIX4_H
#define MATRIX4_H

#include <Vector3.h>
#include <array>

class Matrix4
{
public:
    Matrix4();
    Matrix4(std::array<float, 16> m);
    Matrix4(std::array<std::array<float,4>, 4> m);
    Vector3 operator* ( const Vector3 &v ) const;
private:
    union
    {
        std::array<std::array<float,4>, 4> m;
        std::array<float, 16> _m;
    };
};

#endif // MATRIX4_H
