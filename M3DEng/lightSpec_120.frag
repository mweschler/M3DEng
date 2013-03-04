#version 120

//varying vec4 diffuse;
varying vec3 vertexNormal;
varying vec3 cameraPosition;

uniform vec3 lightDirection;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;
uniform float shininessFactor;
uniform vec4 diffuseColor;

const vec4 specularColor = vec4(0.25, 0.25, 0.25, 1.0);

void main(void){
	vec3 normal = normalize(vertexNormal);
	float cosAngIncidence = dot(normal, lightDirection);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

	vec3 viewDirection = normalize(-cameraPosition);
	vec3 reflectionDirection = reflect(lightDirection, normal);
	float phongTerm = dot(viewDirection, reflectionDirection);
	phongTerm = clamp(phongTerm, 0, 1);
	phongTerm = cosAngIncidence != 0.0 ? phongTerm : 0.0;

	phongTerm = pow(phongTerm, shininessFactor);
    
    gl_FragColor = (diffuseColor * lightIntensity * cosAngIncidence) +
		(specularColor * phongTerm) +
        (diffuseColor * ambientIntensity);
}