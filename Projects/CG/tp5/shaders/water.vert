attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec2 aTextureCoord;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat4 uNMatrix;
uniform sampler2D uSampler3;
uniform float timeFactor;

varying vec2 vTextureCoord;

void main() {
	vec4 pos = vec4(aVertexPosition, 1.0);
	
	vTextureCoord = aTextureCoord * vec2(2, 2) * sin(timeFactor / 1000.0 * 3.14159) / 2.0;

	// A color component (e.g. red) of the texture is used for the height of the water.
	pos.z += texture2D(uSampler3, vTextureCoord).r * sin(timeFactor / 100.0 * 3.14159) / 5.0;

	gl_Position = uPMatrix * uMVMatrix * pos;
}

