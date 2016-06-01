uniform sampler2D texSampler;
uniform bool usesTexture;
varying vec2 fuv;
varying vec4 fcolor;

void main() {
	if (usesTexture)
		//gl_FragColor = texture2D(texSampler, fuv);
		gl_FragColor = texture2D(texSampler, fuv) * fcolor;
	else
		//gl_FragColor = fcolor;
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * fcolor;
}