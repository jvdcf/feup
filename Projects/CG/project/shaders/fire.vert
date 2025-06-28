attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec2 aTextureCoord;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat4 uNMatrix;
uniform float timeFactor;
uniform float random;

varying vec2 vTextureCoord;
varying vec3 vertex;

float rand(float x) {
	return fract(sin(x)* 10000.0);
}

void main() {
	vec3 offset=vec3(
		cos(timeFactor * 2.5) * 0.25 + sin(timeFactor * 1.5) * 0.5 + fract(sin(timeFactor * 0.05) * 0.75),
		sin(timeFactor * 1.5) * 0.75 + cos(timeFactor * 2.0) * 0.75 + fract(cos(timeFactor * 0.05) * 0.25),
		0.0
	);
	offset = offset + rand(random) / 4.0;
	gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition + offset, 1.0);
	vertex = aVertexPosition;
	vTextureCoord = aTextureCoord;
}

