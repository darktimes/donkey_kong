#include <cmath>
template<typename T>
vec4::vec4(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {

}

vec4::vec4(const vec4<T>& other): x(other.x), y(other.y), z(other.z), w(other.w) {

}

template<typename T>
double vec4<T>::calcLength() {
	return sqrt(x * x + y * y + z * z);
}

template<typename T>
vec4<T>* vec4<T>::normalize() {
	double l = calcLength();
	this->x /= l;
	this->y /= l;
	this->z /= l;
	return this;
}

/*
template<typename T>
vec4<T> vec4<T>::operator+(const vec4<T>& v1, const vec4<T>& v2) {
	return vec4<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template<typename T>
vec4<T> vec4<T>::operator-(const vec4<T>& v1, const vec4<T>& v2) {
	return vec4<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template<typename T>
vec4<T> vec4<T>::operator*(const vec4<T>& v1, const T& k) {
	return vec4<T>(v1.x * k, v1.y * k, v1.z * k);
}

template<typename T>
double vec4<T>::dot(const vec4<T>& v1, const vec4<T>& v2) {
	return acos(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

template<typename T>
vec4<T> vec4<T>::cross(const vec4<T>& v1, const vec4<T>& v2) {
	return vec4<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}
*/
