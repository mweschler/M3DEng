#version 120

void main(void){
	float lerpValue = gl_FragCoord.y / 600.0f;
	gl_FragColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.2f, 0.2f, 0.2f, 1.0f), lerpValue);
}