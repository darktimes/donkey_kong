template<typename T>
vec2<T>::vec2(T x, T y): x(x), y(y) {

}

template<typename T>
vec2<T>::vec2(const vec2<T>& other): x(other.x), y(other.y) {

}

template<typename T>
double vec2<T>::calcLength() {
	return sqrt(x * x + y * y);
}

template<typename T>
vec2<T>* vec2<T>::normalize() {
	double l = calcLength();
	this->x /= l;
	this->y/= l;
	return this;
}

template<typename T>
vec2<T>& vec2<T>::operator+(const vec2& v) {
	this->x += v.x;
	this->y += v.y;
	return **this;
}
template<typename T>
vec2<T>& vec2<T>::operator-(const vec2& v) {
	this->x -= v.x;
	this->y -= v.y;
	return **this;
}
template<typename T>
vec2<T>& vec2<T>::operator*(const T& k) {
	this->x *= k;
	this->y *= k;
	return **this;
}

template<typename T>
double dot(const vec2<T>& v1, const vec2<T>& v2) {
	return acos(v1.x * v2.x + v1.y * v2.y);
}



/*
vec2 vec2::operator+(const vec2<T>& v1, const vec2<T>& v2) {
	return vec2<T>(v1.x + v2.x, v1.y + v2.y);
}


vec2 vec2::operator-(const vec2<T>& v1, const vec2<T>& v2) {
	return vec2<T>(v1.x - v2.x, v1.y - v2.y);
}

template<typename T>
vec2<T> vec2<T>::operator*(const vec2<T>& v1, const T& k) {
	return vec2<T>(v1.x * k, v1.y * k);
}

template<typename T>
double vec2<T>::dot(const vec2<T>& v1, const vec2<T>& v2) {
	return acos(v1.x * v2.x + v1.y * v2.y);
}*/




