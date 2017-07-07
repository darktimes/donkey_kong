#include "color.hpp"

using namespace ImageProcessing;


Color::Color(): red(0), green(0), blue(0){}

Color::Color(unsigned r, unsigned g, unsigned b):red(r), green(g), blue(b){}

Color::Color(const Color& c): red(c.red), green(c.green), blue(c.blue){}
		
Color Color::operator*(const unsigned& c)
{
	this->red *= c;
	this->green *= c;
	this->blue *= c;
	return *this;
}
	
Color Color::operator*(const float& c)
{
	this->red = (unsigned)(round((float)red * c));
	this->green = (unsigned)(round((float)green * c));
	this->blue = (unsigned)(round((float)blue * c));
	return *this;
}

Color Color::operator+(const Color& c)
{
	this->red += c.red;
	this->green += c.green;
	this->blue += c.blue;
	return *this;
}

Color Color::operator-(const Color& c)
{
	this->red -= c.red;
	this->green -= c.green;
	this->blue -= c.blue;
	return *this;
}

		
Color Color::operator/(const unsigned& c)
{
	this->red /= c;
	this->green /= c;
	this->blue /= c;
	return *this;
}
