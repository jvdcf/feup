#ifdef GL_ES
precision highp float;
#endif

varying vec3 vertex;
varying vec2 vTextureCoord;
uniform sampler2D uSampler;
uniform float timeFactor;

void main() {
	gl_FragColor = texture2D(uSampler, vTextureCoord) * 
        vec4(
            ((sin(timeFactor * 2.0) + 1.0) / 2.0) * 0.3 + 0.6,
            ((cos(timeFactor * 2.0) + 1.0) / 2.0) * 0.3 * normalize(vertex).y, 
            0.0, 
            0.90
        );
}
