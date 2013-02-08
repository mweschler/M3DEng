#version 120

varying vec4 diffuse;
varying vec3 vertexNormal;
varying vec3 modelPosition;

uniform vec3 lightDirection;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

void main(void){

	float cosAngIncidence = dot(normalize(vertexNormal), lightDirection);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);
    
    gl_FragColor = (diffuse * lightIntensity * cosAngIncidence) +
        (diffuse * ambientIntensity);
}