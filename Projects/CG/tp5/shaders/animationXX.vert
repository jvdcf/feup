
attribute vec3 aVertexPosition;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform float timeFactor;

varying vec4 coords;

void main() {
	vec3 offset=vec3(sin(timeFactor) * 0.2,0.0,0.0);
	gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition+offset, 1.0);
    coords = gl_Position;
}

