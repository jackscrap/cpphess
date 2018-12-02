#include <iostream>
#include <fstream>

#include "shader.h"

Shader::Shader(const std::string& fileName) {
}

Shader::~Shader() {
}

void Shader::Bind() {
}

void Shader::Update(const Transform& transform, const Camera& camera) {
}

std::string Shader::LoadShader(const std::string& fileName) {
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type) {
}
