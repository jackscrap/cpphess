#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "obj_loader.h"

static bool compareObjIdxPtr(
	const ObjIdx* a,
	const ObjIdx* b
);
static inline unsigned int findNxtChar(
	unsigned int start,
	const char* str,
	unsigned int len,
	char tok
);
static inline unsigned int parseObjIdxVal(
	const std::string& tok,
	unsigned int start,
	unsigned int end
);
static inline float parseObjFloatVal(
	const std::string& tok,
	unsigned int start,
	unsigned int end
);
static inline std::vector<
	std::string
> splitStr(
	const std::string &s,
	char delim
);

OBJModel::OBJModel(
	const std::string& fName
) {
	hasUv = false;
	hasNorm = false;
	std::ifstream f;
	f.open(
		fName.c_str(
		)
	);

	std::string l;
	if (f.is_open(
		)) {
		while (f.good(
			)) {
				getline(
					f,
					l
				);

				unsigned int lLen = l.length(
				);

				if (lLen < 2) {
					continue;
				}

				const char* lCStr = l.c_str(
				);

				switch (lCStr[0]) {
					case 'v':
						if(lCStr[1] == 't') {
							this->uv.push_back(
								parseObjVec2(
									l
								)
							);
						} else if (lCStr[1] == 'n') {
							this->norms.push_back(
								parseObjVec3(
									l
								)
							);
						} else if (lCStr[1] == ' ' || lCStr[1] == '\t') {
							this->vert.push_back(
								parseObjVec3(
									l
								)
							);
						}

						break;

						case 'f':
							createObjFace(
								l
							);

						break;

						default:
							break;
					};
			}
		} else {
			std::cerr << "Unable to load mesh: " << fName << std::endl;
		}
}

void IndexedModel::calcNorm() {
	for (
		unsigned int i = 0;
		i < indices.size(
		);
		i += 3
	) {
		int
			i0 = indices[i],
			i1 = indices[i + 1],
			i2 = indices[i + 2];

		glm::vec3 v1 = pos[i1] - pos[i0];
		glm::vec3 v2 = pos[i2] - pos[i0];

		glm::vec3 norm = glm::normalize(
			glm::cross(
				v1,
				v2
			)
		);

		norms[i0] += norm;
		norms[i1] += norm;
		norms[i2] += norm;
	}

	for (
		unsigned int i = 0;
		i < pos.size();
		i++
	) {
		norms[i] = glm::normalize(
			norms[i]
		);
	}
}

IndexedModel OBJModel::toIdxedModel() {
	IndexedModel result;
	IndexedModel normalModel;

	unsigned int noIdc = ObjIdc.size(
	);

	std::vector<
		ObjIdx*
	> idxLookup;

	for (
		unsigned int i = 0;
		i < noIdc;
		i++
	) {
		idxLookup.push_back(
			&ObjIdc[i]
		);
	}

	std::sort(
		idxLookup.begin(),
		idxLookup.end(),
		compareObjIdxPtr
	);

	std::map<
		ObjIdx,
		unsigned int
	> normalModelIndexMap;
	std::map<
		unsigned int,
		unsigned int
	> idxMap;

	for (
		unsigned int i = 0;
		i < noIdc; i++
	) {
		ObjIdx* currIndex = &ObjIdc[i];

		glm::vec3 currPosition = vert[currIndex->vtxIdx];
		glm::vec2 currTexCoord;
		glm::vec3 currNormal;

		if (hasUv) {
			currTexCoord = uv[currIndex->uvIndex];
		} else {
			currTexCoord = glm::vec2(0,0);
		}

		if (hasNorm) {
			currNormal = norms[currIndex->normIdx];
		} else {
			currNormal = glm::vec3(0,0,0);
		}

		unsigned int
			normalModelIndex,
			resultModelIndex;

		std::map<
			ObjIdx,
			unsigned int
		>::iterator it = normalModelIndexMap.find(
			*currIndex
		);
		if (it == normalModelIndexMap.end(
		)) {
			normalModelIndex = normalModel.pos.size();

			normalModelIndexMap.insert(std::pair<ObjIdx, unsigned int>(*currIndex, normalModelIndex));
			normalModel.pos.push_back(currPosition);
			normalModel.texCoord.push_back(currTexCoord);
			normalModel.norms.push_back(currNormal);
		} else {
			normalModelIndex = it->second;
		}

		unsigned int prevVertexLocation = findLastVtxIdx(
			idxLookup,
			currIndex,
			result
		);

		if (prevVertexLocation == (unsigned int) -1) {
			resultModelIndex = result.pos.size();

			result.pos.push_back(currPosition);
			result.texCoord.push_back(currTexCoord);
			result.norms.push_back(currNormal);
		} else {
			resultModelIndex = prevVertexLocation;
		}

		normalModel.indices.push_back(
			normalModelIndex
		);
		result.indices.push_back(
			resultModelIndex
		);
		idxMap.insert(
			std::pair<
				unsigned int,
				unsigned int
			>(resultModelIndex, normalModelIndex));
	}

	if(!hasNorm) {
		normalModel.calcNorm();

		for (
			unsigned int i = 0;
			i < result.pos.size();
			i++
		) {
			result.norms[i] = normalModel.norms[idxMap[i]];
		}
	}

	return result;
};

unsigned int OBJModel::findLastVtxIdx(
	const std::vector<
		ObjIdx*
	>& idxLookup,
	const ObjIdx* currIndex,
	const IndexedModel& result
) {
    unsigned int
			start = 0,
			end = idxLookup.size(),
			curr = (end - start) / 2 + start,
			prev = start;

    while(curr != prev) {
        ObjIdx* testIndex = idxLookup[curr];

        if (testIndex->vtxIdx == currIndex->vtxIdx) {
            unsigned int countStart = curr;

            for (
							unsigned int i = 0;
							i < curr;
							i++
						) {
							ObjIdx* possibleIndex = idxLookup[curr - i];

							if (possibleIndex == currIndex) {
								continue;
							}

							if (possibleIndex->vtxIdx != currIndex->vtxIdx) {
								break;
							}

							countStart--;
            }

            for (
							unsigned int i = countStart;
							i < idxLookup.size() - countStart;
							i++
						) {
							ObjIdx* possibleIndex = idxLookup[curr + i];

							if (possibleIndex == currIndex) {
								continue;
							}

							if (possibleIndex->vtxIdx != currIndex->vtxIdx) {
								break;
							} else if((!hasUv || possibleIndex->uvIndex == currIndex->uvIndex)
									&& (!hasNorm || possibleIndex->normIdx == currIndex->normIdx))
							{
								glm::vec3 currPosition = vert[currIndex->vtxIdx];
								glm::vec2 currTexCoord;
								glm::vec3 currNormal;

								if(hasUv) {
									currTexCoord = uv[currIndex->uvIndex];
								} else {
									currTexCoord = glm::vec2(0,0);
								}

								if (hasNorm) {
									currNormal = norms[currIndex->normIdx];
								} else {
									currNormal = glm::vec3(0,0,0);
								}

								for (
									unsigned int j = 0;
									j < result.pos.size();
									j++
								) {
									if (currPosition == result.pos[j] && ((!hasUv || currTexCoord == result.texCoord[j]) && (!hasNorm || currNormal == result.norms[j]))
									) {
										return j;
									}
								}
							}
            }

            return -1;
        } else {
					if (testIndex->vtxIdx < currIndex->vtxIdx) {
						start = curr;
					} else {
						end = curr;
					}
        }

        prev = curr;
        curr = (end - start) / 2 + start;
    }

    return -1;
}

void OBJModel::createObjFace(
	const std::string& l
) {
	std::vector<std::string> toks = splitStr(
		l, ' '
	);

	this->ObjIdc.push_back(
		parseObjIdx(
			toks[1],
			&this->hasUv,
			&this->hasNorm
		)
	);
	this->ObjIdc.push_back(
		parseObjIdx(
			toks[2],
			&this->hasUv,
			&this->hasNorm
		)
	);
	this->ObjIdc.push_back(
		parseObjIdx(
			toks[3],
			&this->hasUv,
			&this->hasNorm
		)
	);

	if ((int) toks.size() > 4) {
		this->ObjIdc.push_back(
			parseObjIdx(
				toks[1],
				&this->hasUv,
				&this->hasNorm
			)
		);
		this->ObjIdc.push_back(
			parseObjIdx(
				toks[3],
				&this->hasUv,
				&this->hasNorm
			)
		);
		this->ObjIdc.push_back(
			parseObjIdx(
				toks[4],
				&this->hasUv,
				&this->hasNorm
			)
		);
	}
}

ObjIdx OBJModel::parseObjIdx(
	const std::string& tok,
	bool* hasUv,
	bool* hasNorm
) {
	unsigned int tokLen = tok.length();
	const char* tokStr = tok.c_str();

	unsigned int
		vertIdxStart = 0,
		vertIdxEnd = findNxtChar(
			vertIdxStart,
			tokStr,
			tokLen,
			'/'
		);

	ObjIdx result;
	result.vtxIdx = parseObjIdxVal(
		tok,
		vertIdxStart,
		vertIdxEnd
	);
	result.uvIndex = 0;
	result.normIdx = 0;

	if (vertIdxEnd >= tokLen) {
		return result;
	}

	vertIdxStart = vertIdxEnd + 1;
	vertIdxEnd = findNxtChar(
		vertIdxStart,
		tokStr,
		tokLen,
		'/'
	);

	result.uvIndex = parseObjIdxVal(
		tok,
		vertIdxStart,
		vertIdxEnd
	);
	*hasUv = true;

	if (vertIdxEnd >= tokLen) {
		return result;
	}

	vertIdxStart = vertIdxEnd + 1;
	vertIdxEnd = findNxtChar(
		vertIdxStart,
		tokStr,
		tokLen,
		'/'
	);

	result.normIdx = parseObjIdxVal(
		tok,
		vertIdxStart,
		vertIdxEnd
	);
	*hasNorm = true;

	return result;
}

glm::vec3 OBJModel::parseObjVec3(
	const std::string& l
) {
	unsigned int tokLen = l.length(
	);
	const char* tokStr = l.c_str(
	);

	unsigned int vertIdxStart = 2;

	while(vertIdxStart < tokLen) {
		if (tokStr[vertIdxStart] != ' ') {
			break;
		}

		vertIdxStart++;
	}

	unsigned int vertIdxEnd = findNxtChar(
		vertIdxStart,
		tokStr,
		tokLen,
		' '
	);

	float x = parseObjFloatVal(
		l,
		vertIdxStart,
		vertIdxEnd
	);

	vertIdxStart = vertIdxEnd + 1;
	vertIdxEnd = findNxtChar(
		vertIdxStart,
		tokStr,
		tokLen,
		' '
	);

	float y = parseObjFloatVal(
		l,
		vertIdxStart,
		vertIdxEnd
	);

	vertIdxStart = vertIdxEnd + 1;
	vertIdxEnd = findNxtChar(
		vertIdxStart,
		tokStr,
		tokLen,
		' '
	);

	float z = parseObjFloatVal(
		l,
		vertIdxStart,
		vertIdxEnd
	);

	return glm::vec3(
		x,
		y,
		z
	);
}

glm::vec2 OBJModel::parseObjVec2(
	const std::string& l
) {
unsigned int tokLen = l.length();
	const char* tokStr = l.c_str();

	unsigned int vertIdxStart = 3;

	while(vertIdxStart < tokLen) {
		if(tokStr[vertIdxStart] != ' ') {
			break;
		}

		vertIdxStart++;
	}

	unsigned int vertIdxEnd = findNxtChar(
		vertIdxStart,
		tokStr,
		tokLen,
		' '
	);

	float x = parseObjFloatVal(
		l,
		vertIdxStart,
		vertIdxEnd
	);

	vertIdxStart = vertIdxEnd + 1;
	vertIdxEnd = findNxtChar(
		vertIdxStart,
		tokStr,
		tokLen,
		' '
	);

	float y = parseObjFloatVal(
		l,
		vertIdxStart,
		vertIdxEnd
	);

	return glm::vec2(
		x,
		y
	);
}

static bool compareObjIdxPtr(
	const ObjIdx* a,
	const ObjIdx* b
) {
	return a->vtxIdx < b->vtxIdx;
}

static inline unsigned int findNxtChar(
	unsigned int start,
	const char* str,
	unsigned int len,
	char tok
) {
	unsigned int result = start;
	while(result < len) {
		result++;

		if(str[result] == tok) {
			break;
		}
	}

	return result;
}

static inline unsigned int parseObjIdxVal(
	const std::string& tok,
	unsigned int start,
	unsigned int end
) {
	return atoi(
		tok.substr(start,
		end - start
	).c_str()) - 1;
}

static inline float parseObjFloatVal(
	const std::string& tok,
	unsigned int start,
	unsigned int end
) {
	return atof(
		tok.substr(
			start,
			end - start
		).c_str()
	);
}

static inline std::vector<
	std::string
> splitStr(
	const std::string &s,
	char delim
) {
	std::vector<
		std::string
	> elems;

	const char* cstr = s.c_str(
	);
	unsigned int
		strLen = s.length(),
		start = 0,
		end = 0;

	while(end <= strLen) {
		while (end <= strLen) {
			if (cstr[end] == delim) {
				break;
			}

			end++;
		}

		elems.push_back(
			s.substr(
				start,
				end - start
			)
		);
		start = end + 1;
		end = start;
	}

	return elems;
}
