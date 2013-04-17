attribute vec4 vertex;
attribute vec3 vertNormal;
uniform mat4 mvp;
varying vec3 normal;

void main(void)
{
    gl_Position = mvp * vertex;
    normal = vertNormal;
}
