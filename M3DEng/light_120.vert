#version 120

attribute vec4 position;
attribute vec3 normal;
attribute vec4 diffuseColor;

uniform mat4 modelToClip;
uniform vec3 lightDirection;
uniform vec4 lightIntensity;

varying out vec4 color;

void main(void){
	gl_Position = modelToClip * position;
	color = diffuseColor;
}