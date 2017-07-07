#pragma once

namespace ImageProcessing {
	class Pixel
	{
		public:
			unsigned x;
			unsigned y;
			Pixel(unsigned x_value, unsigned y_value);
			Pixel(const Pixel& p);
	
			Pixel operator+(const Pixel& p);
			Pixel operator-(const Pixel& p);
			Pixel operator*(const unsigned& c);
	};
}
