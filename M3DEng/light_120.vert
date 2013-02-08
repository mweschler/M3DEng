#version 120

attribute vec4 position;
attribute vec3 normal;

//uniform mat3 modelToCamera;
uniform mat4 modelToClip;

uniform vec4 diffuseColor;

varying vec4 diffuse;
varying vec3 vertexNormal;
varying vec3 modelPosition;

void main(void){
	gl_Position = modelToClip * position;
	
	vec3 pos3;
	pos3.xyz = position.xyz;
	
	modelPosition = pos3;
	diffuse = diffuseColor;
	vertexNormal = normal;
}