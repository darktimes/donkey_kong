#include "pixel.hpp"

using namespace ImageProcessing;

Pixel::Pixel(unsigned x_value, unsigned y_value):x(x_value), y(y_value){}

Pixel::Pixel(const Pixel& p): x(p.x), y(p.y){}
	
Pixel Pixel::operator+(const Pixel& p)
{
	this->x += p.x;
	this->y += p.y;
	return *this;
}

Pixel Pixel::operator-(const Pixel& p)
{
	this->x -= p.x;
	this->y -= p.y;
	return *this;
}
		
Pixel Pixel::operator*(const unsigned& c)
{
	this->x *= c;
	this->y *= c;
	return *this;
} 
		
