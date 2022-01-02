#include "Vec2.h"


Vec2::Vec2(float _x, float _y)
	: x(_x)
	, y(_y)
	{}

bool Vec2::operator == (const Vec2& rhs) {
	return (x==rhs.x && y==rhs.y);
}
	
bool Vec2::operator != (const Vec2& rhs) {
	return !(*this == rhs);
}


Vec2 Vec2::operator + (const Vec2& rhs) {
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) {
	return Vec2(x - rhs.x, y - rhs.y);
}


Vec2 Vec2::operator + (const float scalar) {
	return Vec2(x+scalar, y+scalar);
}

Vec2 Vec2::operator - (const float scalar) {
	return Vec2(x-scalar, y-scalar);
}

Vec2 Vec2::operator * (const float scalar) {
	return Vec2(x*scalar, y*scalar);
}

Vec2 Vec2::operator / (const float scalar) {
	return Vec2(x/scalar, y/scalar);
}


void Vec2::operator += (const Vec2& rhs) {
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs) {
	x -= rhs.x;
	y -= rhs.y;	
}


void Vec2::operator += (const float scalar) {
	x += scalar;
	y += scalar;
}

void Vec2::operator -= (const float scalar) {
	x -= scalar;
	y -= scalar;
}

void Vec2::operator *= (const float scalar) {
	x *= scalar;
	y *= scalar;
}

void Vec2::operator /= (const float scalar) {
	x /= scalar;
	y /= scalar;
}
