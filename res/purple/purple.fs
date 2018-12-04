#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D Texture0;
varying vec2 texCoord;

vec4 purple0 = vec4(
	69.0 / 256,
	59.0 / 256,
	97.0 / 256,
	1
);

vec4 purple1 = vec4(
	95.0 / 256,
	82.0 / 256,
	134.0 / 256,
	1
);

void main() {
	ivec2 size = textureSize2D(
		Texture0,
		0
	);
	float total = floor(
		texCoord.x * float(
			size.x
		)
	) + floor(
		texCoord.y * float(
			size.y
		)
	);

	bool isEven = mod(total, 2.0) == 0.0;

	gl_FragColor = (isEven) ? purple0 : purple1;
}
