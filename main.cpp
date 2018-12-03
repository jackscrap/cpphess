#include <iostream>
#include <SDL2/SDL.h>

#include <map>
#include <string>

#include "disp.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "cam.h"

#include "col.h"

static std::map<
	std::string,
	const int
> sz = {
	{
		"wd",
		800
	}, {
		"ht",
		600
	}
};

int main() {
	Disp disp(
		sz["wd"],
		sz["ht"],
		"asdf"
	);

	Vtx vtx[] = {
		Vtx(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
		Vtx(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vtx(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
		Vtx(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)),

		Vtx(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)),
		Vtx(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)),
		Vtx(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
		Vtx(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)),

		Vtx(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
		Vtx(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
		Vtx(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
		Vtx(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)),

		Vtx(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
		Vtx(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
		Vtx(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
		Vtx(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)),

		Vtx(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
		Vtx(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
		Vtx(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
		Vtx(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),

		Vtx(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
		Vtx(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
		Vtx(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
		Vtx(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
	};

	Vtx panelVtx[] = {
		Vtx(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
		Vtx(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
		Vtx(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
		Vtx(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)),
	};

	unsigned int idc[] = {
		0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};

	unsigned int panelIdc[] = {
		0, 1, 2,
		0, 2, 3
	};

	Mesh mesh(
		vtx,
		sizeof vtx / sizeof vtx[0],
		idc,
		sizeof idc / sizeof idc[0]
	);

	Mesh panel(
		panelVtx,
		sizeof panelVtx / sizeof panelVtx[0],
		idc,
		sizeof panelIdc / sizeof panelIdc[0]
	);

	Mesh monkey(
		"res/monkey3.obj"
	);
	Shader shader(
		"res/basicShader"
	);
	Texture texture(
		"res/bricks.jpg"
	);
	Transform transform;
	Cam cam(
		glm::vec3(
			0.0f,
			0.0f,
			-5.0f
		),
		70.0f,
		(float) sz["wd"] / (float) sz["ht"],
		0.1f,
		100.0f
	);

	SDL_Event e;
	bool run = true;
	float counter = 0;
	while(run) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				run = false;
			}
		}

		disp.Clear(
			(col["purple0"][0] / 256.0),
			(col["purple0"][1] / 256.0),
			(col["purple0"][2] / 256.0),
			1.0f
		);

		float
			sinCounter = sinf(
				counter
			),
			absSinCounter = abs(
				sinCounter
			);

		transform.GetRot()->y = counter * 100;

		shader.Bind();
		texture.Bind();
		shader.Update(
			transform,
			cam
		);
		/* mesh.Draw(); */
		/* monkey.Draw(); */
		panel.Draw();

		disp.SwapBuffers();
		SDL_Delay(
			1
		);
		/* counter += 0.001f; */
	}

	return 0;
}
