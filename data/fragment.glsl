uniform sampler2D texSampler;
uniform bool usesTexture;
varying vec2 fuv;
varying vec4 fcolor;

void main() {
	gl_FragColor = texture2D(texSampler, fuv) + fcolor;
}