#include "Vector2.h"
#include "Vector3.h"

Vector2::Vector2()
	: x(0.f)
	, y(0.f)
{
}

Vector2::Vector2(float _x, float _y)
	: x(_x)
	, y(_y)
{
}

Vector2::Vector2(const Vector2& v)
	: x(v.x)
	, y(v.y)
{
}

Vector2::Vector2(const Vector3& v)
	: x(v.x)
	, y(v.y)
{
}

Vector2::Vector2(const DirectX::XMVECTOR& v)
	: x(0.f)
	, y(0.f)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
}

float& Vector2::operator[](int index)
{
	assert(index >= 0 && index <= 1);
	if (index == 0)
	{
		return x;
	}
	return y;
}

Vector2& Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Vector2& Vector2::operator=(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
	return *this;
}

Vector2& Vector2::operator=(float f)
{
	x = f;
	y = f;
	return *this;
}

Vector2& Vector2::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
	return x == v.x && y == v.y;
}

bool Vector2::operator==(const DirectX::XMVECTOR& v) const
{
	Vector2 v1;
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)&v1, v);
	return x == v1.x && y == v1.y;
}

bool Vector2::operator!=(const Vector2& v) const
{
	return x != v.x || y != v.y;
}

bool Vector2::operator!=(const DirectX::XMVECTOR& v) const
{
	Vector2 v1;
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)&v1, v);
	return x != v1.x || y != v1.y;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator+(const DirectX::XMVECTOR& v) const
{
	Vector2 v1(v);
	return Vector2(x + v1.x, y + v1.y);
}

Vector2 Vector2::operator+(float f) const
{
	return Vector2(x + f, y + f);
}

void Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
}

void Vector2::operator+=(const DirectX::XMVECTOR& v)
{
	Vector2 v1(v);
	x += v1.x;
	y += v1.y;
}

void Vector2::operator+=(float f)
{
	x += f;
	y += f;
}

const Vector2& Vector2::operator++()
{
	x += 1.f;
	y += 1.f;
	return *this;
}

const Vector2& Vector2::operator++(int)
{
	x += 1.f;
	y += 1.f;
	return *this;
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator-(const DirectX::XMVECTOR& v) const
{
	Vector2 v1(v);
	return Vector2(x - v1.x, y - v1.y);
}

Vector2 Vector2::operator-(float f) const
{
	return Vector2(x - f, y - f);
}

void Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2::operator-=(const DirectX::XMVECTOR& v)
{
	Vector2 v1(v);
	x -= v1.x;
	y -= v1.y;
}

void Vector2::operator-=(float f)
{
	x -= f;
	y -= f;
}

const Vector2& Vector2::operator--()
{
	x -= 1.f;
	y -= 1.f;
	return *this;
}

const Vector2& Vector2::operator--(int)
{
	x -= 1.f;
	y -= 1.f;
	return *this;
}

Vector2 Vector2::operator*(const Vector2& v) const
{
	return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator*(const DirectX::XMVECTOR& v) const
{
	Vector2 v1(v);
	return Vector2(x * v1.x, y * v1.y);
}

Vector2 Vector2::operator*(float f) const
{
	return Vector2(x * f, y * f);
}

void Vector2::operator*=(const Vector2& v)
{
	x *= v.x;
	y *= v.y;
}

void Vector2::operator*=(const DirectX::XMVECTOR& v)
{
	Vector2 v1(v);
	x *= v1.x;
	y *= v1.y;
}

void Vector2::operator*=(float f)
{
	x *= f;
	y *= f;
}

Vector2 Vector2::operator/(const Vector2& v) const
{
	return Vector2(x / v.x, y / v.y);
}

Vector2 Vector2::operator/(const DirectX::XMVECTOR& v) const
{
	Vector2 v1(v);
	return Vector2(x / v1.x, y / v1.y);
}

Vector2 Vector2::operator/(float f) const
{
	return Vector2(x / f, y / f);
}

void Vector2::operator/=(const Vector2& v)
{
	x /= v.x;
	y /= v.y;
}

void Vector2::operator/=(const DirectX::XMVECTOR& v)
{
	Vector2 v1(v);
	x /= v1.x;
	y /= v1.y;
}

void Vector2::operator/=(float f)
{
	x /= f;
	y /= f;
}

bool Vector2::Iszero() const
{
	if (x == 0.f && y == 0.f)
	{
		return true;
	}
	return false;
}

float Vector2::Length() const
{
	return DirectX::XMVectorGetX(DirectX::XMVector2Length(Convert()));
}

void Vector2::Normalize()
{
	*this = DirectX::XMVector2Normalize(Convert());
}

float Vector2::Distance(const Vector2& v) const
{
	Vector2 v1 = *this - v;
	return v1.Length();
}

float Vector2::Dot(const Vector2& v) const
{
	return DirectX::XMVectorGetX(DirectX::XMVector2Dot(Convert(), v.Convert()));
}

float Vector2::Angle(const Vector2& v) const
{
	float width = v.x - x;
	float height = v.y - y;
	float hypotenuse = sqrtf(width * width + height * height);	//ºøº¯
	float angle = width / hypotenuse;
	angle = acosf(angle) / PI * 180.f;
	if (v.y < y)
	{
		angle = 360.f - angle;
	}
	return angle;
}

DirectX::XMVECTOR Vector2::Convert() const
{
	return DirectX::XMLoadFloat2((DirectX::XMFLOAT2*)this);
}

void Vector2::Convert(const DirectX::XMVECTOR& v)
{
	DirectX::XMStoreFloat2((DirectX::XMFLOAT2*)this, v);
}
