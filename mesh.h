#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "obj_loader.h"
#include "util.h"
#include "debugTimer.h"

struct Vtx {
	public:
		Vtx(
			const glm::vec3& pos,
			const glm::vec2& texCoord,
			const glm::vec3& normal
		) {
			this->pos = pos;
			this->texCoord = texCoord;
			this->normal = normal;
		}

		glm::vec3* GetPos() {
			return &pos;
		}
		glm::vec2* GetTexCoord() {
			return &texCoord;
		}
		glm::vec3* GetNormal() {
			return &normal;
		}

	private:
		glm::vec3 pos;
		glm::vec2 texCoord;
		glm::vec3 normal;
};

enum MeshBufferpos {
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

class Mesh {
	public:
		Mesh(const Mesh& mesh) {
		}

		Mesh(
			const std::string& fileName
		) {
			InitMesh(OBJModel(fileName).toIdxedModel());
		}

		Mesh(
			Vtx* vertices,
			unsigned int numVertices,
			unsigned int* indices,
			unsigned int numIndices
		) {
			IndexedModel model;

			for (
				unsigned int i = 0;
				i < numVertices;
				i++
			) {
				model.pos.push_back(*vertices[i].GetPos());
				model.texCoord.push_back(*vertices[i].GetTexCoord());
				model.norms.push_back(*vertices[i].GetNormal());
			}
			
			for (
				unsigned int i = 0;
				i < numIndices;
				i++
			) {
				model.indices.push_back(
					indices[i]
				);
			}

			InitMesh(model);
		}

		~Mesh() {
			glDeleteBuffers(
				NUM_BUFFERS,
				m_vertexArrayBuffers
			);
			glDeleteVertexArrays(1, &m_vertexArrayObject);
		}

		void draw() {
			glBindVertexArray(
				m_vertexArrayObject
			);

			glDrawElementsBaseVertex(
				GL_TRIANGLES,
				m_numIndices,
				GL_UNSIGNED_INT,
				0,
				0
			);

			glBindVertexArray(
				0
			);
		}

		static const unsigned int NUM_BUFFERS = 4;

		void InitMesh(
			const IndexedModel& model
		) {
			m_numIndices = model.indices.size();

			glGenVertexArrays(1, &m_vertexArrayObject);
			glBindVertexArray(m_vertexArrayObject);

			glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
			
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof (model.pos[0]) * model.pos.size(), &model.pos[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof (model.texCoord[0]) * model.texCoord.size(), &model.texCoord[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof (model.norms[0]) * model.norms.size(), &model.norms[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

			glBindVertexArray(0);
		}

		GLuint m_vertexArrayObject;
		GLuint m_vertexArrayBuffers[NUM_BUFFERS];
		unsigned int m_numIndices;
};
