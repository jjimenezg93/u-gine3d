uniform mat4 MVP;
attribute vec3 vpos;
attribute vec4 vcolor;
attribute vec2 vuv;
varying vec4 fcolor;
varying vec2 fuv;

void main() {
	gl_Position = MVP * vec4(vpos, 1);
	fcolor = vcolor;
	fuv = vuv;
}