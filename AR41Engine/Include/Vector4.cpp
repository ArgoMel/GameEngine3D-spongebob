#include "Vector4.h"

Vector4 Vector4::black(0.f, 0.f, 0.f, 1.f);
Vector4 Vector4::white(1.f, 1.f, 1.f, 1.f);
Vector4 Vector4::red(1.f, 0.f, 0.f, 1.f);
Vector4 Vector4::green(0.f, 1.f, 0.f, 1.f);
Vector4 Vector4::blue(0.f, 0.f, 1.f, 1.f);
Vector4 Vector4::yellow(1.f, 1.f, 0.f, 1.f);

Vector4::Vector4()
    : x(0.f)
    , y(0.f)
    , z(0.f)
    , w(0.f)
{
}

Vector4::Vector4(float _x, float _y, float _z, float _w)
    : x(_x)
    , y(_y)
    , z(_z)
    , w(_w)
{
}

Vector4::Vector4(const Vector4& v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(v.w)
{
}

Vector4::Vector4(const DirectX::XMVECTOR& v)
    : x(0.f)
    , y(0.f)
    , z(0.f)
    , w(0.f)
{
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
}

float& Vector4::operator[](int index)
{
    assert(index >= 0 && index <= 3);
    if (index == 0)
    {
        return x;
    }
    else if (index == 1)
    {
        return y;
    }
    else if (index == 2)
    {
        return z;
    }
    return w;
}

Vector4& Vector4::operator=(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

Vector4& Vector4::operator=(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
    return *this;
}

Vector4& Vector4::operator=(float f)
{
    x = f;
    y = f;
    z = f;
    w = f;
    return *this;
}

bool Vector4::operator==(const Vector4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator==(const DirectX::XMVECTOR& v) const
{
    Vector4 v1;
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&v1, v);
    return x == v1.x && y == v1.y && z == v1.z && w == v1.w;
}

bool Vector4::operator!=(const Vector4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

bool Vector4::operator!=(const DirectX::XMVECTOR& v) const
{
    Vector4 v1;
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)&v1, v);
    return x != v1.x || y != v1.y || z != v1.z || w != v1.w;
}

Vector4 Vector4::operator+(const Vector4& v) const
{
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator+(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);
    return Vector4(x + v1.x, y + v1.y, z + v1.z, w + v1.w);
}

Vector4 Vector4::operator+(float f) const
{
    return Vector4(x + f, y + f, z + f, w + f);
}

void Vector4::operator+=(const Vector4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

void Vector4::operator+=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);
    x += v1.x;
    y += v1.y;
    z += v1.z;
    w += v1.w;
}

void Vector4::operator+=(float f)
{
    x += f;
    y += f;
    z += f;
    w += f;
}

const Vector4& Vector4::operator++()
{
    x += 1.f;
    y += 1.f;
    z += 1.f;
    w += 1.f;
    return *this;
}

const Vector4& Vector4::operator++(int)
{
    x += 1.f;
    y += 1.f;
    z += 1.f;
    w += 1.f;
    return *this;
}

Vector4 Vector4::operator-(const Vector4& v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator-(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);
    return Vector4(x - v1.x, y - v1.y, z - v1.z, w - v1.w);
}

Vector4 Vector4::operator-(float f) const
{
    return Vector4(x - f, y - f, z - f, w - f);
}

void Vector4::operator-=(const Vector4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

void Vector4::operator-=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);
    x -= v1.x;
    y -= v1.y;
    z -= v1.z;
    w -= v1.w;
}

void Vector4::operator-=(float f)
{
    x -= f;
    y -= f;
    z -= f;
    w -= f;
}

const Vector4& Vector4::operator--()
{
    x -= 1.f;
    y -= 1.f;
    z -= 1.f;
    w -= 1.f;
    return *this;
}

const Vector4& Vector4::operator--(int)
{
    x -= 1.f;
    y -= 1.f;
    z -= 1.f;
    w -= 1.f;
    return *this;
}

Vector4 Vector4::operator*(const Vector4& v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator*(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);
    return Vector4(x * v1.x, y * v1.y, z * v1.z, w * v1.w);
}

Vector4 Vector4::operator*(float f) const
{
    return Vector4(x * f, y * f, z * f, w * f);
}

void Vector4::operator*=(const Vector4& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
}

void Vector4::operator*=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);
    x *= v1.x;
    y *= v1.y;
    z *= v1.z;
    w *= v1.w;
}

void Vector4::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
}

Vector4 Vector4::operator/(const Vector4& v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4 Vector4::operator/(const DirectX::XMVECTOR& v) const
{
    Vector4 v1(v);
    return Vector4(x / v1.x, y / v1.y, z / v1.z, w / v1.w);
}

Vector4 Vector4::operator/(float f) const
{
    return Vector4(x / f, y / f, z / f, w / f);
}

void Vector4::operator/=(const Vector4& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
}

void Vector4::operator/=(const DirectX::XMVECTOR& v)
{
    Vector4 v1(v);
    x /= v1.x;
    y /= v1.y;
    z /= v1.z;
    w /= v1.w;
}

void Vector4::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    w /= f;
}

DirectX::XMVECTOR Vector4::Convert() const
{
    return DirectX::XMLoadFloat4((DirectX::XMFLOAT4*)this);
}

void Vector4::Convert(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat4((DirectX::XMFLOAT4*)this, v);
}
