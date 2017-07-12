#pragma once
#include <cmath>

namespace Math {

	template<typename T = float>
	struct point {
		T x;
		T y;
		point(T x, T y);

		point<T>& operator+(const point<T>& p);
		point<T>& operator-(const point<T>& p);

		bool inline operator==(const point<T>& p);
		bool inline operator!=(const point<T>& p);

	};
	//point template functions
	#include "point.inl"

	template<typename T = float>
	struct vec2 {

		T x;
		T y;
		vec2(T x, T y);

		vec2(const vec2<T>& other);

		double calcLength();
		vec2<T>* normalize();
		vec2<T>& operator+(const vec2& v);
		vec2<T>& operator-(const vec2& v);
		vec2<T>& operator*(const T& k);
	};

	template<typename T = float>
	double dot(const vec2<T>& v1, const vec2<T>& v2);

	#include "vec2.inl"


	template<typename T = float>
	struct vec3 {
		vec3(T x, T y, T z);
		vec3(const vec3<T>& other);
		T x;
		T y;
		T z;

		double calcLength();
		vec3<T>* normalize();

		vec3<T>& operator+(const vec3<T>& v);
		vec3<T>& operator-(const vec3<T>& v);
		vec3<T>& operator*(const T& v);

		double dot(const vec3<T>& v);
		vec3<T> cross(const vec3<T>& v);
	};
	//vec3 template functions
	#include "vec3.inl"
	/*

	template<typename T = float>
	struct vec4 {
		vec4(T x, T y, T z, T w);
		vec4(const vec4<T>& other);
		T x;
		T y;
		T z;
		T w;

		double calcLength();
		vec4<T>* normalize();
		friend vec4<T> operator+(const vec4<T>& v1, const vec4<T>& v2);
		friend vec4<T> operator-(const vec4<T>& v1, const vec4<T>& v2);
		friend vec4<T> operator*(const vec4<T>& v1, const T& k);
		friend double dot(const vec4<T>& v1, const vec4<T>& v2);
		friend vec4<T> cross(const vec4<T>& v1, const vec4<T>& v2);
	};
	//vec4 template functions
	#include "vec4.inl"*/

	struct mat4 {
		float data[4][4] = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
		};
		mat4();
	};

	mat4 ortho(float l, float width, float b, float height, float n, float f);
	mat4 scale(mat4 m, float k);
	mat4 translate(mat4 m, vec2<float> v);
	mat4 rotate(mat4, float rad);


}
