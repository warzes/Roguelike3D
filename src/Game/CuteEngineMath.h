#pragma once

class Vector2 final
{
public:
	Vector2() = default;
	Vector2(const float ixy) : x(ixy), y(ixy) {}
	Vector2(const float ix, const float iy) : x(ix), y(iy) {}

	operator const float*() const { return &x; }
	operator float*() { return &x; }
	float &operator[](unsigned int index){ return *(((float *) &x) + index); }

	void operator+=(const float s);
	void operator+=(const Vector2& v);
	void operator-=(const float s);
	void operator-=(const Vector2& v);
	void operator*=(const float s);
	void operator*=(const Vector2& v);
	void operator/=(const float s);
	void operator/=(const Vector2& v);

	float x, y;
};

Vector2 operator+(const Vector2& u, const Vector2& v);
Vector2 operator+(const Vector2& v, const float s);
Vector2 operator+(const float s, const Vector2& v);

Vector2 operator-(const Vector2& u, const Vector2& v);
Vector2 operator-(const Vector2& v, const float s);
Vector2 operator-(const float s, const Vector2& v);

Vector2 operator-(const Vector2& v);

Vector2 operator*(const Vector2& u, const Vector2& v);
Vector2 operator*(const float s, const Vector2& v);
Vector2 operator*(const Vector2& v, const float s);

Vector2 operator/(const Vector2& u, const Vector2& v);
Vector2 operator/(const Vector2& v, const float s);
Vector2 operator/(const float s, const Vector2& v);

bool operator==(const Vector2& u, const Vector2& v);