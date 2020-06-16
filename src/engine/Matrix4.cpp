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
    const float fInvW = 1.0f / ( m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] );
    return {( m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] ) * fInvW,
            ( m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] ) * fInvW,
            ( m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] ) * fInvW};
}
