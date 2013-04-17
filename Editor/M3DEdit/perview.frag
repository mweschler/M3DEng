uniform vec4 diffuse;
uniform vec3 lightDir;
uniform vec4 ambientLight;
uniform vec4 ambientIntensity;

varying vec3 normal;

void main(void)
{
    float cosAngIncidence = dot(normal, lightDir);
    cosAngIncidence = clamp(cosAngIncidence, 0.0, 1.0);
    gl_FragColor = (diffuse * cosAngIncidence) + (ambientLight * ambientIntensity);
}
