#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Cam {
	private:
		glm::mat4 proj;
		glm::vec3 pos;
		glm::vec3 forward;
		glm::vec3 up;

	public:
		Cam(
			const glm::vec3& pos,
			float fov,
			float aspect,
			float zNear,
			float zFar
		) {
			this->pos = pos;
			this->forward = glm::vec3(
				0.0f,
				0.0f,
				1.0f
			);
			this->up = glm::vec3(
				0.0f,
				1.0f,
				0.0f
			);
			this->proj = glm::perspective(
				fov,
				aspect,
				zNear,
				zFar
			);
		}

		inline glm::mat4 getViewProj(
		) const {
			return proj * glm::lookAt(
				pos,
				pos + forward,
				up
			);
		}
};
