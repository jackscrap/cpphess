#include <string>
#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

class Disp {
	private:
		SDL_Window* win;
		SDL_GLContext ctx;

	public:
		Disp(
			int wd,
			int ht,
			const std::string& title
		) {
			SDL_Init(
				SDL_INIT_EVERYTHING
			);

			SDL_GL_SetAttribute(
				SDL_GL_RED_SIZE,
				8
			);
			SDL_GL_SetAttribute(
				SDL_GL_GREEN_SIZE,
				8
			);
			SDL_GL_SetAttribute(
				SDL_GL_BLUE_SIZE,
				8
			);
			SDL_GL_SetAttribute(
				SDL_GL_ALPHA_SIZE,
				8
			);
			SDL_GL_SetAttribute(
				SDL_GL_BUFFER_SIZE,
				32
			);
			SDL_GL_SetAttribute(
				SDL_GL_DEPTH_SIZE,
				16
			);
			SDL_GL_SetAttribute(
				SDL_GL_DOUBLEBUFFER,
				1
			);

			win = SDL_CreateWindow(
				title.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				wd,
				ht,
				SDL_WINDOW_OPENGL
			);
			ctx = SDL_GL_CreateContext(
				win
			);

			GLenum res = glewInit(
			);
			if (res != GLEW_OK) {
				std::cerr << "Glew failed to initialize!" << std::endl;
			}

			glEnable(
				GL_DEPTH_TEST
			);

			glEnable(
				GL_CULL_FACE
			);
			glCullFace(
				GL_BACK
			);
		}

		~Disp() {
			SDL_GL_DeleteContext(
				ctx
			);
			SDL_DestroyWindow(
				win
			);
			SDL_Quit();
		}

		void clear(
			float r,
			float g,
			float b,
			float a
		) {
			glClearColor(
				r,
				g,
				b,
				a
			);
			glClear(
				GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
			);
		}

		void swpBuff() {
			SDL_GL_SwapWindow(
				win
			);
		}
};
