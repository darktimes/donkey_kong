#include <cmath>
template<typename T>
vec3<T>::vec3(T x, T y, T z): x(x), y(y), z(z) {

}
template<typename T>
vec3<T>::vec3(const vec3<T>& other): x(other.x), y(other.y), z(other.z) {

}

template<typename T>
double vec3<T>::calcLength() {
	return sqrt(x * x + y * y + z * z);
}

template<typename T>
vec3<T>* vec3<T>::normalize() {
	double l = calcLength();
	this->x /= l;
	this->y /= l;
	this->z /= l;
	return this;
}

template<typename T>
vec3<T>& vec3<T>::operator+(const vec3<T>& v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return **this;
}

template<typename T>
vec3<T>& vec3<T>::operator-(const vec3<T>& v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return **this;
}

template<typename T>
vec3<T>& vec3<T>::operator*(const T& k) {
	this->x *= k;
	this->y *= k;
	this->z *= k;
	return **this;
}

template<typename T>
double vec3<T>::dot(const vec3<T>& v) {
	return acos(x * v.x + y * v.y + z * v.z);
}

template<typename T>
vec3<T> vec3<T>::cross(const vec3<T>& v) {
	return vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

