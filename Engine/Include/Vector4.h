#pragma once
#include "EngineMath.h"

struct Vector4
{
	static Vector4	black;
	static Vector4	white;
	static Vector4	red;
	static Vector4	green;
	static Vector4	blue;
	static Vector4	yellow;
	float	x, y, z, w;
	Vector4();
	Vector4(float _x, float _y, float _z, float _w);
	Vector4(const Vector4& v);
	Vector4(const DirectX::XMVECTOR& v);


	float& operator [](int index);

	// operator =
	Vector4& operator = (const Vector4& v);
	Vector4& operator = (const DirectX::XMVECTOR& v);
	Vector4& operator = (float f);

	// operator ==, !=
	bool operator == (const Vector4& v)	const;
	bool operator == (const DirectX::XMVECTOR& v)	const;
	bool operator != (const Vector4& v)	const;
	bool operator != (const DirectX::XMVECTOR& v)	const;

	// operator +
	Vector4 operator + (const Vector4& v)	const;
	Vector4 operator + (const DirectX::XMVECTOR& v)	const;
	Vector4 operator + (float f)	const;
	// operator +=
	void operator += (const Vector4& v);
	void operator += (const DirectX::XMVECTOR& v);
	void operator += (float f);
	// operator ++
	const Vector4& operator ++ ();
	const Vector4& operator ++ (int);

	// operator -
	Vector4 operator - (const Vector4& v)	const;
	Vector4 operator - (const DirectX::XMVECTOR& v)	const;
	Vector4 operator - (float f)	const;
	// operator -=
	void operator -= (const Vector4& v);
	void operator -= (const DirectX::XMVECTOR& v);
	void operator -= (float f);
	// operator --
	const Vector4& operator -- ();
	const Vector4& operator -- (int);

	// operator *
	Vector4 operator * (const Vector4& v)	const;
	Vector4 operator * (const DirectX::XMVECTOR& v)	const;
	Vector4 operator * (float f)	const;
	// operator *=
	void operator *= (const Vector4& v);
	void operator *= (const DirectX::XMVECTOR& v);
	void operator *= (float f);

	// operator /
	Vector4 operator / (const Vector4& v)	const;
	Vector4 operator / (const DirectX::XMVECTOR& v)	const;
	Vector4 operator / (float f)	const;
	// operator /=
	void operator /= (const Vector4& v);
	void operator /= (const DirectX::XMVECTOR& v);
	void operator /= (float f);

	DirectX::XMVECTOR Convert()	const;
	void Convert(const DirectX::XMVECTOR& v);
};

