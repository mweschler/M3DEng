#version 120
attribute vec4 position;
uniform mat4 modelToClip;
void main(void){
	gl_Position = modelToClip * position;
}