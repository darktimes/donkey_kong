#include <initializer_list>
#include <algorithm>
#include "math.hpp"
#include "utils.hpp"
#include <iostream>

using namespace Math;

//-----------------------------------------------------------------------------
// vec2f
//-----------------------------------------------------------------------------
vec2f::vec2f(): x(0.0f), y(0.0f)  {

}

vec2f::vec2f(float x, float y): x(x), y(y) {

}

vec2f::vec2f(const vec2f& other): x(other.x), y(other.y) {

}

float vec2f::calcLength() const {
	return sqrt(x * x + y * y);
}

vec2f& vec2f::normalize() {
	double l = calcLength();
	this->x /= l;
	this->y /= l;
	return *this;
}

vec2f& vec2f::operator+(const vec2f& v) {
	this->x += v.x;
	this->y += v.y;
	return *this;
}

vec2f& vec2f::operator-(const vec2f& v) {
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

vec2f& vec2f::operator*(float k) {
	this->x *= k;
	this->y *= k;
	return *this;
}

float vec2f::dot(const vec2f& v) {
	return acos(this->x * v.x + this->y * v.y);
}

//-----------------------------------------------------------------------------
// vec2ui
//-----------------------------------------------------------------------------
vec2ui::vec2ui(): x(0), y(0) {

}

vec2ui::vec2ui(unsigned x, unsigned y): x(x), y(y) {

}

//-----------------------------------------------------------------------------
// vec2i
//-----------------------------------------------------------------------------
vec2i::vec2i(): x(0), y(0) {

}

vec2i::vec2i(int x, int y): x(x), y(y) {

}



//-----------------------------------------------------------------------------
// vec3f
//-----------------------------------------------------------------------------
vec3f::vec3f(): x(0), y(0), z(0) {

}

vec3f::vec3f(float x, float y, float z): x(x), y(y), z(z) {

}

vec3f::vec3f(const vec3f& other): x(other.x), y(other.y), z(other.z) {

}

float vec3f::calcLength() const{
	return sqrt(x * x + y * y + z * z);
}

vec3f& vec3f::normalize() {
	float l = calcLength();
	this->x /= l;
	this->y /= l;
	this->z /= l;
	return *this;
}

vec3f& vec3f::operator+(const vec3f& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

vec3f& vec3f::operator-(const vec3f& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

vec3f& vec3f::operator*(float k) {
	this->x *= k;
	this->y *= k;
	this->z *= k;
	return *this;
}

float vec3f::dot(const vec3f& v) const {
	return acos(x * v.x + y * v.y + z * v.z);
}

vec3f vec3f::cross(const vec3f& v) const {
	return vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

//-----------------------------------------------------------------------------
// mat4
//-----------------------------------------------------------------------------

mat4::mat4() {

}

mat4 Math::ortho(float l, float width, float b, float height, float n, float f) {
	mat4 result;
	result.data[0][0] = 2 / (width - l);
	result.data[1][1] = 2 / (height - b);
	result.data[2][2] = -2 / (f - n);
	result.data[3][0] = - 1;
	result.data[3][1] = - 1;
	result.data[3][2] = - (f + n) / (f - n);

	return result;
}

mat4& mat4::translate(vec2f v) {
	this->data[3][0] = v.x;
	this->data[3][1] = v.y;
	return *this;
}

mat4& mat4::rotate(float rad) {
	this->data[0][0] = cos(rad);
	this->data[1][0] = -sin(rad);
	this->data[0][1] = sin(rad);
	this->data[1][1] = cos(rad);
	return *this;
}

mat4& mat4::scale(float k) {
	for (unsigned l = 0; l <= 2; l++) {
		this->data[l][l] *= k;
	}
	return *this;
}

mat4& mat4::scaleX(float k) {
	this->data[0][0] *= k;
	return *this;
}

mat4& mat4::scaleY(float k) {
	this->data[1][1] *= k;
	return *this;
}
