#include "Vector3.h"

Vector3 Vector3::axis[(int)AXIS::AXIS_MAX] =
{
    Vector3(1.f, 0.f, 0.f),
    Vector3(0.f, 1.f, 0.f),
    Vector3(0.f, 0.f, 1.f)
};

Vector3::Vector3()
    : x(0.f)
    , y(0.f)
    , z(0.f)
{
}

Vector3::Vector3(float _x, float _y, float _z)
    : x(_x)
    , y(_y)
    , z(_z)
{
}

Vector3::Vector3(const Vector3& v)
    : x(v.x)
    , y(v.y)
    , z(v.z)
{
}

Vector3::Vector3(const DirectX::XMVECTOR& v)
    : x(0.f)
    , y(0.f)
    , z(0.f)
{
    DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

float& Vector3::operator[](int index)
{
    assert(index >= 0 && index <= 2);
    if (index == 0)
    {
        return x;
    }
    else if (index == 1)
    {
        return y;
    }
    return z;
}

Vector3& Vector3::operator=(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vector3& Vector3::operator=(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
    return *this;
}

Vector3& Vector3::operator=(float f)
{
    x = f;
    y = f;
    z = f;
    return *this;
}

bool Vector3::operator==(const Vector3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator==(const DirectX::XMVECTOR& v) const
{
    Vector3 v1;
    DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&v1, v);
    return x == v1.x && y == v1.y && z == v1.z;
}

bool Vector3::operator==(float f)
{
    return x == f && y == f && z == f;
}

bool Vector3::operator!=(const Vector3& v) const
{
    return x != v.x || y != v.y || z != v.z;
}

bool Vector3::operator!=(const DirectX::XMVECTOR& v) const
{
    Vector3 v1;
    DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)&v1, v);
    return x != v1.x || y != v1.y || z != v1.z;
}

bool Vector3::operator!=(float f)
{
    return x != f || y != f || z != f;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator+(const DirectX::XMVECTOR& v) const
{
    Vector3 v1(v);
    return Vector3(x + v1.x, y + v1.y, z + v1.z);
}

Vector3 Vector3::operator+(float f) const
{
    return Vector3(x + f, y + f, z + f);
}

void Vector3::operator+=(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector3::operator+=(const DirectX::XMVECTOR& v)
{
    Vector3 v1(v);
    x += v1.x;
    y += v1.y;
    z += v1.z;
}

void Vector3::operator+=(float f)
{
    x += f;
    y += f;
    z += f;
}

const Vector3& Vector3::operator++()
{
    x += 1.f;
    y += 1.f;
    z += 1.f;
    return *this;
}

const Vector3& Vector3::operator++(int)
{
    x += 1.f;
    y += 1.f;
    z += 1.f;
    return *this;
}

Vector3 Vector3::operator-()
{
    return Vector3(-x, -y,-z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-(const DirectX::XMVECTOR& v) const
{
    Vector3 v1(v);
    return Vector3(x - v1.x, y - v1.y, z - v1.z);
}

Vector3 Vector3::operator-(float f) const
{
    return Vector3(x - f, y - f, z - f);
}

void Vector3::operator-=(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vector3::operator-=(const DirectX::XMVECTOR& v)
{
    Vector3 v1(v);
    x -= v1.x;
    y -= v1.y;
    z -= v1.z;
}

void Vector3::operator-=(float f)
{
    x -= f;
    y -= f;
    z -= f;
}

const Vector3& Vector3::operator--()
{
    x -= 1.f;
    y -= 1.f;
    z -= 1.f;
    return *this;
}

const Vector3& Vector3::operator--(int)
{
    x -= 1.f;
    y -= 1.f;
    z -= 1.f;
    return *this;
}

Vector3 Vector3::operator*(const Vector3& v) const
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator*(const DirectX::XMVECTOR& v) const
{
    Vector3 v1(v);
    return Vector3(x * v1.x, y * v1.y, z * v1.z);
}

Vector3 Vector3::operator*(float f) const
{
    return Vector3(x * f, y * f, z * f);
}

void Vector3::operator*=(const Vector3& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
}

void Vector3::operator*=(const DirectX::XMVECTOR& v)
{
    Vector3 v1(v);
    x *= v1.x;
    y *= v1.y;
    z *= v1.z;
}

void Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
}

Vector3 Vector3::operator/(const Vector3& v) const
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3 Vector3::operator/(const DirectX::XMVECTOR& v) const
{
    Vector3 v1(v);
    return Vector3(x / v1.x, y / v1.y, z / v1.z);
}

Vector3 Vector3::operator/(float f) const
{
    return Vector3(x / f, y / f, z / f);
}

void Vector3::operator/=(const Vector3& v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

void Vector3::operator/=(const DirectX::XMVECTOR& v)
{
    Vector3 v1(v);
    x /= v1.x;
    y /= v1.y;
    z /= v1.z;
}

void Vector3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
}

bool Vector3::Iszero()
{
    if (x == 0.f && y == 0.f&& z == 0.f)
    {
        return true;
    }
    return false;
}

float Vector3::Length() const
{
    return DirectX::XMVectorGetX(DirectX::XMVector3Length(Convert()));
}

void Vector3::Normalize()
{
    *this = DirectX::XMVector3Normalize(Convert());
}

float Vector3::Distance(const Vector3& v) const
{
    Vector3 v1 = *this - v;
    return v1.Length();
}

float Vector3::Dot(const Vector3& v) const
{
    return DirectX::XMVectorGetX(DirectX::XMVector3Dot(Convert(), v.Convert()));
}

Vector3 Vector3::Cross(const Vector3& v) const
{
    return Vector3(DirectX::XMVector3Cross(Convert(), v.Convert()));
}

float Vector3::Angle(const Vector3& v) const
{
    Vector3 v1 = *this;
    Vector3 v2 = v;
    v1.Normalize();
    v2.Normalize();
    float angle = v1.Dot(v2);
    angle = RadianToDegree(acosf(angle));
    return angle;
}

Vector3 Vector3::ConvertAngle() const
{
    return Vector3(DegreeToRadian(x), DegreeToRadian(y), DegreeToRadian(z));
}

Vector3 Vector3::TransformNormal(const Matrix& m) const
{
    return Vector3(DirectX::XMVector3TransformNormal(Convert(), m.m));
}

Vector3 Vector3::TransformCoord(const Matrix& m) const
{
    return Vector3(DirectX::XMVector3TransformCoord(Convert(), m.m));
}

DirectX::XMVECTOR Vector3::Convert() const
{
    return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
}

void Vector3::Convert(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}
