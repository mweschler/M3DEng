#version 120

attribute vec4 position;
attribute vec3 normal;

uniform mat3 modelToCamera;
uniform mat4 modelToClip;
uniform mat3 normMatrix;

varying vec3 vertexNormal;
varying vec3 cameraPosition;

void main(void){
	gl_Position = modelToClip * position;
	
	vec3 pos3;
	pos3.xyz = position.xyz;
	
	cameraPosition = modelToCamera * pos3;
//	diffuse = diffuseColor;
	vertexNormal = normalize( normMatrix * normal);
}