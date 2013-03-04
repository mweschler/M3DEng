#version 120

attribute vec4 position;
attribute vec3 normal;

uniform mat4 modelToClip;
uniform mat3 normMatrix;

varying vec3 vertexNormal;

void main(void){
	gl_Position = modelToClip * position;
	
	vertexNormal = normalize( normMatrix * normal);
}