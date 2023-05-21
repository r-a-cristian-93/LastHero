// CRT Emulation
// inspired by crt-mattias
// https://github.com/libretro/glsl-shaders/blob/master/crt/shaders/crt-mattias.glsl
#version 130

out vec4 COL0;
out vec4 TEX0;

void main() {
	mat4 MVPMatrix = gl_ModelViewProjectionMatrix;
	vec4 VertexCoord = gl_Vertex;
	vec4 TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_Position = MVPMatrix * VertexCoord;
    TEX0.xy = TexCoord.xy;
    COL0 = gl_Color;
}
