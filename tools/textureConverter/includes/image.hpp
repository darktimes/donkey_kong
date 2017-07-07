#pragma once

#include "color.hpp"
#include "pixel.hpp"

namespace ImageProcessing {
 
	unsigned wrap(unsigned toWrap, const unsigned int& limit);

	class Image
	{
		private:
			int width;
			int height;
			unsigned char* content;		
		public:
	
			/* Constructors */
			Image(const Image& img);
			Image(const char* path);
			Image(const int width, const int height); 
			~Image();
		
			/* Function members*/
			int save(const char* name);
		
				
			/* Setters */
			void setColor(const Pixel& p, const Color& color);
			void setColor(const unsigned& where, const Color& c);
			void setContent(unsigned char* p);
		
			/* Getters */
			Color getColor(const Pixel& p) const;
			Color getColor(const unsigned& where) const;
			unsigned char* getContent() const;
			int getWidth() const;
			int getHeight() const;
	};
}
