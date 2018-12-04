#include <string>
#include <iostream>

#include <GL/glew.h>

#include "stb_image.h"

class Tex {
	public:
		Tex(
			const std::string& fName
		) {
			int
				wd,
				ht,
				noComp;

			unsigned char* data = stbi_load(
				fName.c_str(),
				&wd,
				&ht,
				&noComp,
				4
			);

			if (!data) {
				std::cerr << "Unable to load texture: " << fName << std::endl;
			}
					
			glGenTextures(
				1,
				&tex
			);
			glBindTexture(
				GL_TEXTURE_2D,
				tex
			);
					
			glTexParameteri(
				GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_S,
				GL_REPEAT
			);
			glTexParameteri(
				GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_T,
				GL_REPEAT
			);
					
			glTexParameterf(
				GL_TEXTURE_2D,
				GL_TEXTURE_MIN_FILTER,
				GL_LINEAR
			);
			glTexParameterf(
				GL_TEXTURE_2D,
				GL_TEXTURE_MAG_FILTER,
				GL_LINEAR
			);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				wd,
				ht,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(
				data
			);
		}

		~Tex() {
			glDeleteTextures(
				1,
				&tex
			);
		}

		void bind() {
			glBindTexture(GL_TEXTURE_2D, tex);
		}

	private:
		Tex(
			const Tex& tex
		);

		GLuint tex;
};
