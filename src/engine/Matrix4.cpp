#include "Matrix4.h"

Matrix4::Matrix4()
{

}

Matrix4::Matrix4(std::array<float, 16> m)
    :_m(m)
{

}
Matrix4::Matrix4(std::array<std::array<float,4>, 4> m)
    :m(m)
{

}


Vector3 Matrix4::operator* ( const Vector3 &v ) const
{
    const Vector3 o {
        v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
        v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
        v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]
    };

    const float w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + m[3][3];

    if (w != 0.0f)
    {
        return {o.x/w, o.y / w, o.z / w};
    }
    return o;
}

