uniform sampler2D texSampler;
varying vec4 fcolor;
varying vec2 fuv;

void main() {
	gl_FragColor = texture2D(texSampler, fuv) * fcolor;
}