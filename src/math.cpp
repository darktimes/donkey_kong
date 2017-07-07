#include <initializer_list>
#include <algorithm>
#include "math.hpp"
#include "utils.hpp"
#include <iostream>

using namespace Math;

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

mat4 Math::translate(mat4 m, vec2<float> v) {	
	m.data[3][0] = v.x;
	m.data[3][1] = v.y;
	return m;
}

mat4 Math::rotate(mat4 m, float rad) {
	m.data[0][0] = cos(rad);
	m.data[1][0] = -sin(rad);
	m.data[0][1] = sin(rad);
	m.data[1][1] = cos(rad);
	return m;
}

mat4 Math::scale(mat4 m, float k) {
	for (unsigned l = 0; l <= 2; l++) {
		m.data[l][l] *= k;
	}
	return m;
}
