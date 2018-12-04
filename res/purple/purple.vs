varying vec2 texCoord;

void main(void) {
	gl_Position = vec4(
		gl_Vertex.xy,
		0.0,
		1.0
	);
	gl_Position = sign(
		gl_Position
	);

	texCoord = (
		vec2(
			gl_Position.x,
			gl_Position.y
		) + vec2(
			5.0
		)
	) / vec2(
		40.0
	);
}
