#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

struct OBJIndex {
    unsigned int
			vertexIndex,
			uvIndex,
			normalIndex;

    bool operator<(
			const OBJIndex& r
		) const {
			return vertexIndex < r.vertexIndex;
		}
};

class IndexedModel {
	public:
		std::vector<
			glm::vec3
		> positions;
		std::vector<
			glm::vec2
		> texCoords;
		std::vector<
			glm::vec3
		> normals;
		std::vector<
			unsigned int
		> indices;

		void CalcNormals();
};

class OBJModel {
	private:
		unsigned int FindLastVertexIndex(
			const std::vector<OBJIndex*>& indexLookup,
			const OBJIndex* currentIndex,
			const IndexedModel& result
		);
		void CreateOBJFace(
			const std::string& line
		);

		glm::vec2 ParseOBJVec2(
			const std::string& line
		);
		glm::vec3 ParseOBJVec3(
			const std::string& line
		);
		OBJIndex ParseOBJIndex(
			const std::string& token,
			bool* hasUVs,
			bool* hasNormals
		);

	public:
		std::vector<
			OBJIndex
		> OBJIndices;
		std::vector<
			glm::vec3
		> vertices;
		std::vector<
			glm::vec2
		> uvs;
		std::vector<
			glm::vec3
		> normals;

		bool
			hasUVs,
			hasNormals;

		OBJModel(
			const std::string& fileName
		);

		IndexedModel ToIndexedModel(
		);
};
