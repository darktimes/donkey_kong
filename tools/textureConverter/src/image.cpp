#include <string.h>
#include <math.h>
#include <stdio.h>
#include <SOIL/SOIL.h>

#include "image.hpp"


using namespace ImageProcessing;

Image::Image(const Image& img)
{
	width = img.getWidth();
	height = img.getHeight();
	size_t size = width * height * 3;
	content = new unsigned char[size];
	memcpy(content, img.getContent(), size);
}

Image::Image(const int w, const int h): width(w), height(h)
{
	content = new unsigned char[width * height * 3];
	memset(content, 0, width * height * 3);
}

Image::Image(const char* path)
{
	/* width and height are adjusted with this function call aswell*/
	content = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
}

Image::~Image() 
{
	SOIL_free_image_data(content);
}

int Image::save(const char* name) 
{
	return SOIL_save_image(name, SOIL_SAVE_TYPE_TGA, width, height, 3, content);
}

void Image::setColor(const Pixel& p, const Color& c)
{
	unsigned index = (width * p.y + p.x) * 3;
	content[index]		= c.red;
	content[index + 1]	= c.green;
	content[index + 2]	= c.blue;
}

int Image::getWidth() const
{
	return width;
} 

int Image::getHeight() const
{
	return height;
}

unsigned char* Image::getContent() const
{
	return content;
}

void Image::setContent(unsigned char* p)
{
	content = p;
}

void Image::setColor(const unsigned& where, const Color& c)
{
	content[where * 3] = c.red;
	content[where * 3 + 1] = c.green;
	content[where * 3 + 2] = c.blue;
}

Color Image::getColor(const Pixel& p) const
{
	unsigned index = (width * (wrap(p.y, (unsigned)height)) + (wrap(p.x, (unsigned)width))) * 3;
//	unsigned b = Utils::wrap(p.x, 200)
//	unsigned index = (width * p.y + p.x) * 3;
	return Color(content[index], content[index + 1], content[index + 2]);
}

Color Image::getColor(const unsigned& where) const
{
	return Color(content[where * 3], content[where * 3 + 1], content[where * 3 + 2]);
}

unsigned ImageProcessing::wrap(unsigned toWrap, const unsigned int& limit) {
	if (toWrap >= limit)
	{
		toWrap %= limit;
		return toWrap;
	} 
	while (toWrap < 0)
		toWrap += limit;
	return toWrap;
}

