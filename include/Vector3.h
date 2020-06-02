struct Vector3
{
    Vector3(const Vector3&) = delete;
    Vector3():x(0), y(0), z(0){};
    Vector3(float x, float y, float z)
    :x(x), y(y), z(z){};
    float x, y, z;
};