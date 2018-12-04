#pragma once

#include <SDL2/SDL.h>
#include <iostream>

class DebugTimer {
	public:
		void start(
		) {
			startTime = SDL_GetTicks();
		}
		
		void end(
			const std::string& msg
		) {
			unsigned int endTime = SDL_GetTicks(
			);
			std::cout << msg << (endTime - startTime) << "ms" << std::endl;
		}
			
	private:
			unsigned int startTime;
};
