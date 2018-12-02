#include <string>

#include <SDL2/SDL.h>

class Disp {
public:
	Disp(
		int width,
		int height,
		const std::string& title
	);

	~Disp();

	void Clear(
		float r,
		float g,
		float b,
		float a
	);

	void SwapBuffers();

private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
};
