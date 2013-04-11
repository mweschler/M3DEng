attribute vec4 vertex;
uniform mat4 modelToCamera;
//varying vec4 qt_TexCoord0;

void main(void)
{
    gl_Position = /*modelToCamera **/ vertex;
    //qt_TexCoord0 = qt_MultiTexCoord0;
}
