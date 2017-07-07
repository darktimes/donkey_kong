#pragma once

#include <cmath>
namespace ImageProcessing {
	class Color
	{
		public:
			unsigned red;
			unsigned green;
			unsigned blue;

			Color();
			Color(unsigned r, unsigned g, unsigned b);
			Color(const Color& c);
		
			Color operator*(const unsigned& c);
			Color operator*(const float& c);
			Color operator+(const Color& c);
			Color operator-(const Color& c);
			Color operator/(const unsigned& c);
	};
}
