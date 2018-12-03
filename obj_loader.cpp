#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "obj_loader.h"

static bool CompareOBJIndexPtr(
	const OBJIndex* a,
	const OBJIndex* b
);
static inline unsigned int FindNextChar(
	unsigned int start,
	const char* str,
	unsigned int length,
	char token
);
static inline unsigned int ParseOBJIndexValue(
	const std::string& token,
	unsigned int start,
	unsigned int end
);
static inline float ParseOBJFloatValue(
	const std::string& token,
	unsigned int start,
	unsigned int end
);
static inline std::vector<
	std::string
> SplitString(
	const std::string &s,
	char delim
);

OBJModel::OBJModel(
	const std::string& fName
) {
	hasUVs = false;
	hasNormals = false;
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

				unsigned int lineLength = l.length();

				if (lineLength < 2) {
					continue;
				}

				const char* lineCStr = l.c_str(
				);

				switch(lineCStr[0]) {
					case 'v':
						if(lineCStr[1] == 't') {
							this->uvs.push_back(ParseOBJVec2(l));
						} else if (lineCStr[1] == 'n') {
							this->normals.push_back(
								ParseOBJVec3(
									l
								)
							);
						} else if (lineCStr[1] == ' ' || lineCStr[1] == '\t') {
							this->vertices.push_back(
								ParseOBJVec3(
									l
								)
							);
						}

						break;

						case 'f':
							CreateOBJFace(
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

void IndexedModel::CalcNormals() {
	for (
		unsigned int i = 0;
		i < indices.size();
		i += 3
	) {
		int
			i0 = indices[i],
			i1 = indices[i + 1],
			i2 = indices[i + 2];

		glm::vec3 v1 = positions[i1] - positions[i0];
		glm::vec3 v2 = positions[i2] - positions[i0];

		glm::vec3 norm = glm::normalize(
			glm::cross(
				v1,
				v2
			)
		);

		normals[i0] += norm;
		normals[i1] += norm;
		normals[i2] += norm;
	}

	for (
		unsigned int i = 0;
		i < positions.size();
		i++
	) {
		normals[i] = glm::normalize(
			normals[i]
		);
	}
}

IndexedModel OBJModel::ToIndexedModel() {
	IndexedModel result;
	IndexedModel normalModel;

	unsigned int numIdc = OBJIndices.size(
	);

	std::vector<
		OBJIndex*
	> indexLookup;

	for (
		unsigned int i = 0;
		i < numIdc;
		i++
	) {
		indexLookup.push_back(
			&OBJIndices[i]
		);
	}

	std::sort(
		indexLookup.begin(),
		indexLookup.end(),
		CompareOBJIndexPtr
	);

	std::map<
		OBJIndex,
		unsigned int
	> normalModelIndexMap;
	std::map<
		unsigned int,
		unsigned int
	> indexMap;

	for (
		unsigned int i = 0;
		i < numIdc; i++
	) {
		OBJIndex* currIndex = &OBJIndices[i];

		glm::vec3 currPosition = vertices[currIndex->vertexIndex];
		glm::vec2 currTexCoord;
		glm::vec3 currNormal;

		if (hasUVs) {
			currTexCoord = uvs[currIndex->uvIndex];
		} else {
			currTexCoord = glm::vec2(0,0);
		} 

		if (hasNormals) {
			currNormal = normals[currIndex->normalIndex];
		} else {
			currNormal = glm::vec3(0,0,0);
		}

		unsigned int
			normalModelIndex,
			resultModelIndex;

		std::map<
			OBJIndex,
			unsigned int
		>::iterator it = normalModelIndexMap.find(
			*currIndex
		);
		if (it == normalModelIndexMap.end(
		)) {
			normalModelIndex = normalModel.positions.size();

			normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currIndex, normalModelIndex));
			normalModel.positions.push_back(currPosition);
			normalModel.texCoords.push_back(currTexCoord);
			normalModel.normals.push_back(currNormal);
		} else {
			normalModelIndex = it->second;
		}

		unsigned int prevVertexLocation = FindLastVertexIndex(
			indexLookup,
			currIndex,
			result
		);

		if (prevVertexLocation == (unsigned int) -1) {
			resultModelIndex = result.positions.size();

			result.positions.push_back(currPosition);
			result.texCoords.push_back(currTexCoord);
			result.normals.push_back(currNormal);
		} else {
			resultModelIndex = prevVertexLocation;
		}

		normalModel.indices.push_back(
			normalModelIndex
		);
		result.indices.push_back(
			resultModelIndex
		);
		indexMap.insert(
			std::pair<
				unsigned int,
				unsigned int
			>(resultModelIndex, normalModelIndex));
	}

	if(!hasNormals) {
		normalModel.CalcNormals();

		for (
			unsigned int i = 0;
			i < result.positions.size();
			i++
		) {
			result.normals[i] = normalModel.normals[indexMap[i]];
		}
	}

	return result;
};

unsigned int OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currIndex, const IndexedModel& result) {
    unsigned int
			start = 0,
			end = indexLookup.size(),
			curr = (end - start) / 2 + start,
			prev = start;

    while(curr != prev) {
        OBJIndex* testIndex = indexLookup[curr];

        if (testIndex->vertexIndex == currIndex->vertexIndex) {
            unsigned int countStart = curr;

            for (
							unsigned int i = 0;
							i < curr;
							i++
						) {
							OBJIndex* possibleIndex = indexLookup[curr - i];

							if (possibleIndex == currIndex) {
								continue;
							}

							if (possibleIndex->vertexIndex != currIndex->vertexIndex) {
								break;
							}

							countStart--;
            }

            for (
							unsigned int i = countStart;
							i < indexLookup.size() - countStart;
							i++
						) {
							OBJIndex* possibleIndex = indexLookup[curr + i];

							if (possibleIndex == currIndex) {
								continue;
							}

							if (possibleIndex->vertexIndex != currIndex->vertexIndex) {
								break;
							} else if((!hasUVs || possibleIndex->uvIndex == currIndex->uvIndex)
									&& (!hasNormals || possibleIndex->normalIndex == currIndex->normalIndex))
							{
								glm::vec3 currPosition = vertices[currIndex->vertexIndex];
								glm::vec2 currTexCoord;
								glm::vec3 currNormal;

								if(hasUVs) {
									currTexCoord = uvs[currIndex->uvIndex];
								} else {
									currTexCoord = glm::vec2(0,0);
								}

								if (hasNormals) {
									currNormal = normals[currIndex->normalIndex];
								} else {
									currNormal = glm::vec3(0,0,0);
								}

								for (
									unsigned int j = 0;
									j < result.positions.size();
									j++
								) {
									if (currPosition == result.positions[j] && ((!hasUVs || currTexCoord == result.texCoords[j]) && (!hasNormals || currNormal == result.normals[j]))
									) {
										return j;
									}
								}
							}
            }

            return -1;
        } else {
					if (testIndex->vertexIndex < currIndex->vertexIndex) {
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

void OBJModel::CreateOBJFace(
	const std::string& line
) {
	std::vector<std::string> tokens = SplitString(
		line, ' '
	);

	this->OBJIndices.push_back(
		ParseOBJIndex(
			tokens[1],
			&this->hasUVs,
			&this->hasNormals
		)
	);
	this->OBJIndices.push_back(
		ParseOBJIndex(
			tokens[2],
			&this->hasUVs,
			&this->hasNormals
		)
	);
	this->OBJIndices.push_back(
		ParseOBJIndex(
			tokens[3],
			&this->hasUVs,
			&this->hasNormals
		)
	);

	if ((int) tokens.size() > 4) {
		this->OBJIndices.push_back(
			ParseOBJIndex(
				tokens[1],
				&this->hasUVs,
				&this->hasNormals
			)
		);
		this->OBJIndices.push_back(
			ParseOBJIndex(
				tokens[3],
				&this->hasUVs,
				&this->hasNormals
			)
		);
		this->OBJIndices.push_back(
			ParseOBJIndex(
				tokens[4],
				&this->hasUVs,
				&this->hasNormals
			)
		);
	}
}

OBJIndex OBJModel::ParseOBJIndex(
	const std::string& token,
	bool* hasUVs,
	bool* hasNormals
) {
	unsigned int tokenLength = token.length();
	const char* tokenString = token.c_str();

	unsigned int
		vertIndexStart = 0,
		vertIndexEnd = FindNextChar(
			vertIndexStart,
			tokenString,
			tokenLength,
			'/'
		);

	OBJIndex result;
	result.vertexIndex = ParseOBJIndexValue(
		token,
		vertIndexStart,
		vertIndexEnd
	);
	result.uvIndex = 0;
	result.normalIndex = 0;

	if (vertIndexEnd >= tokenLength) {
		return result;
	}

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(
		vertIndexStart,
		tokenString,
		tokenLength,
		'/'
	);

	result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasUVs = true;

	if(vertIndexEnd >= tokenLength) {
		return result;
	}

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(
		vertIndexStart,
		tokenString,
		tokenLength,
		'/'
	);

	result.normalIndex = ParseOBJIndexValue(
		token,
		vertIndexStart,
		vertIndexEnd
	);
	*hasNormals = true;

	return result;
}

glm::vec3 OBJModel::ParseOBJVec3(
	const std::string& line
) {
	unsigned int tokenLength = line.length(
	);
	const char* tokenString = line.c_str(
	);

	unsigned int vertIndexStart = 2;

	while(vertIndexStart < tokenLength) {
		if (tokenString[vertIndexStart] != ' ') {
			break;
		}

		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(
		vertIndexStart,
		tokenString,
		tokenLength,
		' '
	);

	float x = ParseOBJFloatValue(
		line,
		vertIndexStart,
		vertIndexEnd
	);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(
		vertIndexStart,
		tokenString,
		tokenLength,
		' '
	);

	float y = ParseOBJFloatValue(
		line,
		vertIndexStart,
		vertIndexEnd
	);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(
		vertIndexStart,
		tokenString,
		tokenLength,
		' '
	);

	float z = ParseOBJFloatValue(
		line,
		vertIndexStart,
		vertIndexEnd
	);

	return glm::vec3(
		x,
		y,
		z
	);
}

glm::vec2 OBJModel::ParseOBJVec2(
	const std::string& line
) {
unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 3;

	while(vertIndexStart < tokenLength) {
		if(tokenString[vertIndexStart] != ' ') {
			break;
		}

		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(
		vertIndexStart,
		tokenString,
		tokenLength,
		' '
	);

	float x = ParseOBJFloatValue(
		line,
		vertIndexStart,
		vertIndexEnd
	);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(
		vertIndexStart,
		tokenString,
		tokenLength,
		' '
	);

	float y = ParseOBJFloatValue(
		line,
		vertIndexStart,
		vertIndexEnd
	);

	return glm::vec2(
		x,
		y
	);
}

static bool CompareOBJIndexPtr(
	const OBJIndex* a,
	const OBJIndex* b
) {
	return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(
	unsigned int start,
	const char* str,
	unsigned int length,
	char token
) {
	unsigned int result = start;
	while(result < length) {
		result++;

		if(str[result] == token) {
			break;
		}
	}

	return result;
}

static inline unsigned int ParseOBJIndexValue(
	const std::string& token,
	unsigned int start,
	unsigned int end
) {
	return atoi(
		token.substr(start,
		end - start
	).c_str()) - 1;
}

static inline float ParseOBJFloatValue(
	const std::string& token,
	unsigned int start,
	unsigned int end
) {
	return atof(
		token.substr(
			start,
			end - start
		).c_str()
	);
}

static inline std::vector<std::string> SplitString(
	const std::string &s,
	char delim
) {
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int
		strLength = s.length(),
		start = 0,
		end = 0;

	while(end <= strLength) {
		while (end <= strLength) {
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
