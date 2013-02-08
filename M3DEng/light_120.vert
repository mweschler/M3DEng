#version 120

attribute vec4 position;
attribute vec3 normal;

uniform mat3 modelToCamera;
uniform mat4 modelToClip;

uniform vec4 diffuseColor;
uniform vec3 lightDirection;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

varying vec4 color;

void main(void){
	gl_Position = modelToClip * position;
	vec3 normalCameraSpace = normalize(modelToCamera * normal);
	float angleIncidence = dot(normalCameraSpace, lightDirection);
	angleIncidence = clamp(angleIncidence, 0, 1);
	color = (lightIntensity * diffuseColor * angleIncidence) + 
			(diffuseColor * ambientIntensity);
}