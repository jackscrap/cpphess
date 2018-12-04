#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "disp.h"
#include "mesh.h"
#include "shad.h"
#include "tex.h"
#include "trans.h"
#include "cam.h"

#include "col.h"

std::map<
	std::string,
	const float
> sz = {
	{
		"wd",
		800.0
	}, {
		"ht",
		600.0
	}
};

int main() {
	Disp disp(
		sz,
		"C++hess"
	);

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
		i < 9;
		i++
	) {
		// panel
		Vtx asdfVtx[] = {
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

		unsigned int asdfIdc[] = {
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

		Mesh tile(
			asdfVtx,
			sizeof asdfVtx / sizeof *asdfVtx,
			asdfIdc,
			sizeof asdfIdc / sizeof *asdfIdc
		);

		board.push_back(
			tile
		);
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

	Mesh tile(
		vtx,
		sizeof vtx / sizeof *vtx,
		idc,
		sizeof idc / sizeof *idc
	);

	Mesh monkey(
		"obj/cube.obj"
	);
	Shad checker(
		"res/purple/purple"
	);
	Tex tex(
		"res/bricks.jpg"
	);
	Trans trans;
	Cam cam(
		glm::vec3(
			0.0f,
			3.0f,
			-7.5f
		),
		70.0f,
		sz["wd"] / sz["ht"],
		0.1f,
		100.0f
	);

	SDL_Event e;
	bool run = true;
	float counter = 0;
	while(run) {
		while(SDL_PollEvent(
			&e
		)) {
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

		trans.getRot()->y = counter * 100;

		checker.bind();
		tex.bind();
		checker.update(
			trans,
			cam
		);

		for (
			int i = 0;
			i < 1;
			i++
		) {
			/* board[0].draw(); */
		}

		monkey.draw();
		/* tile.draw(); */

		disp.swpBuff();
		SDL_Delay(
			1
		);
	}

	return 0;
}
