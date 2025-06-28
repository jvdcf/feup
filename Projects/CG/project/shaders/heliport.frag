#ifdef GL_ES
precision highp float;
#endif

varying vec2 vTextureCoord;
uniform sampler2D uSamplerDefault;
uniform sampler2D uSamplerAction;
uniform float timeFactor;

void main() {
    vec4 mainTexture = texture2D(uSamplerDefault, vTextureCoord);
    vec4 mainWeight = vec4(1.0 - timeFactor, 1.0 - timeFactor, 1.0 - timeFactor, 1.0 - timeFactor);
    vec4 actionTexture = texture2D(uSamplerAction, vTextureCoord);
    vec4 actionWeight = vec4(timeFactor, timeFactor, timeFactor, timeFactor);
	gl_FragColor = (mainTexture * mainWeight) + (actionTexture * actionWeight);
}
