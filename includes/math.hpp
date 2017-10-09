#pragma once
#include <cmath>

namespace Math {

	struct vec2f {
		float x;
		float y;

		vec2f();
		vec2f(float x, float y);
		vec2f(const vec2f& other);

		float calcLength() const;
		vec2f& normalize();
		vec2f& operator+(const vec2f& v);
		vec2f& operator-(const vec2f& v);
		vec2f& operator*(float k);

		float dot(const vec2f& other);
	};

	struct vec2i {
		int x;
		int y;

		vec2i();
		vec2i(int x, int y);
	};

	struct vec2ui {
		unsigned x;
		unsigned y;

		vec2ui();
		vec2ui(unsigned x, unsigned y);
	};

	struct vec3f {
		vec3f();
		vec3f(float x, float y, float z);
		vec3f(const vec3f& other);
		float x;
		float y;
		float z;

		float calcLength() const;
		vec3f& normalize();

		vec3f& operator+(const vec3f&);
		vec3f& operator-(const vec3f&);
		vec3f& operator*(float);

		float dot(const vec3f& v) const;
		vec3f cross(const vec3f& v) const;
	};

	struct mat4 {
		float data[4][4] = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
		};
		mat4();
		mat4& scale(float k);
		mat4& scaleX(float k);
		mat4& scaleY(float k);
		mat4& translate(vec2f v);
		mat4& rotate(float rad);
	};

	mat4 ortho(float l, float width, float b, float height, float n, float f);

}
