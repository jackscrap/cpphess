#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "disp.h"
#include "mesh.h"
#include "shad.h"
#include "texture.h"
#include "trans.h"
#include "cam.h"

#include "col.h"

std::map<
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
		"C++hess"
	);

/* 	Vtx vtx[] = { */
/* 		Vtx(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)), */
/* 		Vtx(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)), */
/* 		Vtx(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)), */
/* 		Vtx(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)), */

/* 		Vtx(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)), */
/* 		Vtx(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)), */
/* 		Vtx(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)), */
/* 		Vtx(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)), */

/* 		Vtx(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)), */
/* 		Vtx(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0)), */
/* 		Vtx(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)), */
/* 		Vtx(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)), */

/* 		Vtx(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)), */
/* 		Vtx(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)), */
/* 		Vtx(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)), */
/* 		Vtx(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)), */

/* 		Vtx(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)), */
/* 		Vtx(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)), */
/* 		Vtx(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)), */
/* 		Vtx(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)), */

/* 		Vtx(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)), */
/* 		Vtx(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)), */
/* 		Vtx(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)), */
/* 		Vtx(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)), */
/* 	}; */

/* 	unsigned int idc[] = { */
/* 		0, 1, 2, */
/* 		0, 2, 3, */

/* 		6, 5, 4, */
/* 		7, 6, 4, */

/* 		10, 9, 8, */
/* 		11, 10, 8, */

/* 		12, 13, 14, */
/* 		12, 14, 15, */

/* 		16, 17, 18, */
/* 		16, 18, 19, */

/* 		22, 21, 20, */
/* 		23, 22, 20 */
/* 	}; */

/* 	Mesh mesh( */
/* 		vtx, */
/* 		sizeof vtx / sizeof *vtx, */
/* 		idc, */
/* 		sizeof idc / sizeof *idc */
/* 	); */

	// panel
	Vtx panelVtx[] = {
		Vtx(
			glm::vec3(-0.1, -0.1, -0.1),
			glm::vec2(1, 0),
			glm::vec3(0, 0, -1)
		),
		Vtx(
			glm::vec3(-0.1, 0.1, -0.1),
			glm::vec2(0, 0),
			glm::vec3(0, 0, -1)
		),
		Vtx(
			glm::vec3(0.1, 0.1, -0.1),
			glm::vec2(0, 1),
			glm::vec3(0, 0, -1)
		),
		Vtx(
			glm::vec3(0.1, -0.1, -0.1),
			glm::vec2(1, 1),
			glm::vec3(0, 0, -1)
		),
	};

	unsigned int panelIdc[] = {
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

	Mesh mesh(
		panelVtx,
		sizeof panelVtx / sizeof *panelVtx,
		panelIdc,
		sizeof panelIdc / sizeof *panelIdc
	);

	std::vector<
		Mesh
	> board;
	for (
		int i = 0;
		i < 3;
		i++
	) {
	Vtx panelVtx[] = {
		Vtx(
			glm::vec3(-0.1, -0.1, -0.1),
			glm::vec2(1, 0),
			glm::vec3(0, 0, -1)
		),
		Vtx(
			glm::vec3(-0.1, 0.1, -0.1),
			glm::vec2(0, 0),
			glm::vec3(0, 0, -1)
		),
		Vtx(
			glm::vec3(0.1, 0.1, -0.1),
			glm::vec2(0, 1),
			glm::vec3(0, 0, -1)
		),
		Vtx(
			glm::vec3(0.1, -0.1, -0.1),
			glm::vec2(1, 1),
			glm::vec3(0, 0, -1)
		),
	};

	unsigned int panelIdc[] = {
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

	Mesh mesh(
		panelVtx,
		sizeof panelVtx / sizeof *panelVtx,
		panelIdc,
		sizeof panelIdc / sizeof *panelIdc
	);

		board.push_back(mesh);
	}

		Vtx vtx[] = {
			Vtx(
				glm::vec3(-1, -1, -1),
				glm::vec2(1, 0),
				glm::vec3(0, 0, -3)
			),
			Vtx(
				glm::vec3(-1, 1, -1),
				glm::vec2(0, 0),
				glm::vec3(0, 0, -1)
			),
			Vtx(
				glm::vec3(1, 1, -1),
				glm::vec2(0, 1),
				glm::vec3(0, 0, -1)
			),
			Vtx(
				glm::vec3(1, -1, -1),
				glm::vec2(1, 1),
				glm::vec3(0, 0, -1)
			),
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

		Mesh panel(
			vtx,
			sizeof vtx / sizeof *vtx,
			idc,
			sizeof idc / sizeof *idc
		);

		board.push_back(panel);

	Mesh monkey(
		"res/monkey3.obj"
	);
	Shad shad(
		"res/basicShader"
	);
	Texture texture(
		"res/bricks.jpg"
	);
	Trans trans;
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

		disp.clear(
			col["purple0"][0] / 256.0,
			col["purple0"][1] / 256.0,
			col["purple0"][2] / 256.0,
			1
		);

		float
			sinCounter = sinf(
				counter
			),
			absSinCounter = abs(
				sinCounter
			);

		trans.GetRot()->y = counter * 100;

		shad.Bind();
		texture.Bind();
		shad.Update(
			trans,
			cam
		);

		/* monkey.Draw(); */

		/* for ( */
		/* 	int i = 0; */
		/* 	i < 1; */
		/* 	i++ */
		/* ) { */
		/* 	board[i].draw(); */
		/* } */

		mesh.draw();

		disp.SwapBuffers();
		SDL_Delay(
			1
		);
		/* counter += 0.001f; */
	}

	return 0;
}
