#pragma once

#include <GL/glew.h>

namespace Renderer {
	class Texture2D
	{
		public:
			GLuint id;
			GLuint width, height;

			GLuint internalFormat;
			GLuint imageFormat;

			GLuint wrapS;
			GLuint wrapT;
			GLuint filterMin;
			GLuint filterMax;
			Texture2D(bool alpha);
			void generate(GLuint width, GLuint height, unsigned char* data);
			void bind() const;

			static Texture2D* fromFile(const std::string& fileName, bool alpha);
	};
}
