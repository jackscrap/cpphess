CC = g++

DEPS = main.cpp mesh.h disp.h camera.h shader.h texture.h obj_loader.h obj_loader.cpp

GLEW = -lGLEW -lGL
SDL = -lSDL2

make: main.cpp
	$(CC) $(DEPS) $(GLEW) $(SDL) -std=c++11 stb_image.c

clean:
	rm *.out
