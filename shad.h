#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>

#include "trans.h"

class Shad {
	private:
		static const unsigned int
			noShad = 2,
			noUni = 3;

		std::string loadShad(
			const std::string& fName
		) {
			std::ifstream f;
			f.open(
				fName.c_str()
			);

			std::string
				out,
				l;

			if (f.is_open()) {
				while (f.good()) {
					getline(
						f,
						l
					);

					out.append(
						l + "\n"
					);
				}
			} else {
				std::cerr << "Unable to load shader: " << fName << std::endl;
			}

			return out;
		}

		void checkShadErr(
			GLuint shad,
			GLuint flag,
			bool isProg,
			const std::string& errMsg
		) {
			GLint success = 0;
			GLchar err[1024] = {
				0
			};

			if (isProg) {
				glGetProgramiv(shad,
					flag,
					&success
				);
			} else {
				glGetShaderiv(
					shad,
					flag,
					&success
				);
			}

			if (success == GL_FALSE) {
				if (isProg) {
					glGetProgramInfoLog(
						shad,
						sizeof err,
						NULL,
						err
					);
				} else {
					glGetShaderInfoLog(
						shad,
						sizeof err,
						NULL,
						err
					);
				}

				std::cerr << errMsg << ": '" << err << "'" << std::endl;
			}
		}

		GLuint createShad(
			const std::string& text,
			unsigned int type
		) {
			GLuint shad = glCreateShader(
				type
			);

			if (!shad) {
				std::cerr << "Error compiling shader type " << type << std::endl;
			}

			const GLchar* p[1];
			p[0] = text.c_str();
			GLint len[1];
			len[0] = text.length();

			glShaderSource(
				shad,
				1,
				p,
				len
			);
			glCompileShader(
				shad
			);

			checkShadErr(
				shad,
				GL_COMPILE_STATUS,
				false,
				"Error compiling shader!"
			);

			return shad;
		}

		GLuint
			prog,
			shad[noShad],
			m_uniforms[noUni];

	public:
		Shad(
			const std::string& fName
		) {
			prog = glCreateProgram();
			shad[0] = createShad(
				loadShad(
					fName + ".vs"
				),
				GL_VERTEX_SHADER
			);
			shad[1] = createShad(
				loadShad(
					fName + ".fs"
				),
				GL_FRAGMENT_SHADER
			);

			for (
				unsigned int i = 0;
				i < noShad;
				i++
			) {
				glAttachShader(
					prog,
					shad[i]
				);
			}

			glBindAttribLocation(prog,
				0,
				"position"
			);
			glBindAttribLocation(prog,
				1,
				"texCoord"
			);
			glBindAttribLocation(prog,
				2,
				"normal"
			);

			glLinkProgram(prog);
			checkShadErr(
				prog,
				GL_LINK_STATUS,
				true,
				"Error linking shader program"
			);

			glValidateProgram(
				prog
			);

			checkShadErr(
				prog,
				GL_LINK_STATUS,
				true,
				"Invalid shader program"
			);

			m_uniforms[0] = glGetUniformLocation(
				prog,
				"mvp"
			);
			m_uniforms[1] = glGetUniformLocation(
				prog,
				"Norm"
			);
			m_uniforms[2] = glGetUniformLocation(
				prog,
				"lightDir"
			);
		}

		~Shad() {
			for(unsigned int i = 0; i < noShad; i++) {
				glDetachShader(prog, shad[i]);
				glDeleteShader(shad[i]);
			}

			glDeleteProgram(prog);
		}

		void bind() {
			glUseProgram(prog);
		}

		void update(
			const Trans& trans,
			const Cam& camera
		) {
			glm::mat4 MVP = trans.getMvp(camera);
			glm::mat4 Normal = trans.getModel();

			glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
			glUniform3f(m_uniforms[2], 0.0f, 0.0f, 1.0f);
		}
};
