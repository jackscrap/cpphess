#version 150

varying vec2 texCoord0;
varying vec3 normal0;

uniform sampler2D sampler;
uniform vec3 lightDirection;

void main() {
	gl_FragColor = vec4(95.0 / 256, 82.0 / 256, 134.0 / 256, 1) * clamp(dot(-lightDirection, normal0), 0.0, 1.0);
}
