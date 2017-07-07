#include <iostream>
#include <stdlib.h>
#include <memory>
#include <string.h>
#include <fstream>
#include <map>
#include <vector>
#include <ft2build.h>
#include <stdexcept>
#include FT_FREETYPE_H

using namespace std;


bool glyphBit(const FT_GlyphSlot &glyph, const int x, const int y) {
    int pitch = abs(glyph->bitmap.pitch);
    unsigned char *row = &glyph->bitmap.buffer[pitch * y];
    char cValue = row[x >> 3];

    return (cValue & (128 >> (x & 7))) != 0;
}

struct Character {
	pair<float, float> Size;    // Size of glyph
	pair<float, float> bearing;  // Offset from baseline to left/top of glyph
	int advance;    // Horizontal offset to advance to next glyph
};

std::map<char, Character> Characters;


int main(int argc, char** argv) {
	ofstream result;
	string outputFileName(argv[1]);
	outputFileName = string("output/t-") + outputFileName;
	result.open(outputFileName.c_str());	
	
	// All functions return a value different than 0 whenever an error occurred
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		throw runtime_error("Could not init freetype lib");
	

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, argv[1], 0, &face))
		throw runtime_error("Failed to load font");

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);
	int cur_advance = 0;
	
	// Load first 128 characters of ASCII set
	for (int c = 0; c < 256; c++)
	{
		// Load character glyph


		if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_MONOCHROME)) {
		    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		    continue;
		}
		result<<face->glyph->bitmap.width<<";";
		result<<face->glyph->bitmap.rows<<";";
		result<<face->glyph->bitmap_left<<";";
		result<<face->glyph->bitmap_top<<";";
		result<<face->glyph->advance.x<<";";
//		cur_advance += glyph->bitmap.width;
		
		for (unsigned i = 0; i < face->glyph->bitmap.rows; i++) {
			for (unsigned j = 0; j < face->glyph->bitmap.width; j++) {
				unsigned char bit = glyphBit(face->glyph, j, i);
				result<<((unsigned)bit == 0 ? "0" : "1")<<";";				
				//std::cout<<(((unsigned int)bit == 0) ? "" : "*" )<<" ";

				
			}


				//std::cout<<std::endl;
		}
//		result<<face->glyph->bitmap;

		/*
		glTexImage2D(
		    GL_TEXTURE_2D,
		    0,
		    GL_RED,
		    face->glyph->bitmap.width,
		    face->glyph->bitmap.rows,
		    0,
		    GL_RED,
		    GL_UNSIGNED_BYTE,
		    face->glyph->bitmap.buffer
		);

		// Now store character for later use
		Character character = {
		    pair<float, float>(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		    pair<float, float>(face->glyph->bitmap_left, face->glyph->bitmap_top),
		    face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));*/
	}

	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	result.close();
	return 0;
}
