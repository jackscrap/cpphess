#version 150

attribute vec3 pos;
attribute vec2 texCoord;
attribute vec3 norm;

varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 MVP;
uniform mat4 Normal;

void main() {
	gl_Position = MVP * vec4(pos, 1.0);

	texCoord0 = texCoord;
	normal0 = (Normal * vec4(norm, 0.0)).xyz;
}
