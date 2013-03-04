#version 120

varying vec3 vertexNormal;

uniform vec4 diffuseColor;
uniform vec3 lightDirection;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

void main(void){

	float cosAngIncidence = dot(normalize(vertexNormal), lightDirection);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);
    
    gl_FragColor = (diffuseColor * lightIntensity * cosAngIncidence) +
        (diffuseColor * ambientIntensity);
}