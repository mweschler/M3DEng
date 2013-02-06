#version 120
attribute vec4 position;
uniform mat4 perspective;
void main(void){
	gl_Position = perspective * position;
}