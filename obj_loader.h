#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

struct ObjIdx {
    unsigned int
			vtxIdx,
			uvIndex,
			normIdx;

    bool operator<(
			const ObjIdx& r
		) const {
			return vtxIdx < r.vtxIdx;
		}
};

class IndexedModel {
	public:
		std::vector<
			glm::vec3
		> pos;
		std::vector<
			glm::vec2
		> texCoord;
		std::vector<
			glm::vec3
		> norms;
		std::vector<
			unsigned int
		> indices;

		void calcNorm();
};

class OBJModel {
	private:
		unsigned int findLastVtxIdx(
			const std::vector<
				ObjIdx*
			>& idxLookup,
			const ObjIdx* currIdx,
			const IndexedModel& result
		);
		void createObjFace(
			const std::string& l
		);

		glm::vec2 parseObjVec2(
			const std::string& l
		);
		glm::vec3 parseObjVec3(
			const std::string& l
		);
		ObjIdx parseObjIdx(
			const std::string& tok,
			bool* hasUv,
			bool* hasNorm
		);

	public:
		std::vector<
			ObjIdx
		> ObjIdc;
		std::vector<
			glm::vec3
		> vert;
		std::vector<
			glm::vec2
		> uv;
		std::vector<
			glm::vec3
		> norms;

		bool
			hasUv,
			hasNorm;

		OBJModel(
			const std::string& fName
		);

		IndexedModel toIdxedModel(
		);
};
