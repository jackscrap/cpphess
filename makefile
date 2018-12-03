CC = g++
V = -std=c++17

DEP = main.cpp mesh.h disp.h cam.h shader.h texture.h obj_loader.h obj_loader.cpp stb_image.c

GLEW = -lGLEW -lGL
SDL = -lSDL2

make: main.cpp
	$(CC) $(V) $(DEP) $(GLEW) $(SDL)

clean:
	rm *.out
