#include <string>
#include <iostream>

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
	public:
		Texture(
			const std::string& fileName
		);

		~Texture();

		void Bind();

	private:
		Texture(
			const Texture& texture
		);

		GLuint m_texture;
};
