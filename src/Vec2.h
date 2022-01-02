#ifndef VEC_2
#define VEC_2

class Vec2 {
public:
	float x = 0;
	float y = 0;

	Vec2();
	Vec2(float _x, float _y);

	bool operator == (const Vec2& rhs);
	bool operator != (const Vec2& rhs);
	
	Vec2 operator + (const Vec2& rhs);
	Vec2 operator - (const Vec2& rhs);
	
	Vec2 operator + (const float scalar);
	Vec2 operator - (const float scalar);
	Vec2 operator * (const float scalar);
	Vec2 operator / (const float scalar);
	
	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	
	void operator += (const float scalar);
	void operator -= (const float scalar);
	void operator *= (const float scalar);
	void operator /= (const float scalar);
};

#endif
