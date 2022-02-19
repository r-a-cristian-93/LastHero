// CRT Emulation
// by Mattias
// https://www.shadertoy.com/view/lsB3DV
#version 130

#pragma parameter CURVATURE "Curvature" 0.5 0.0 1.0 0.05
#pragma parameter SCANSPEED "Scanline Crawl Speed" 1.0 0.0 10.0 0.5

#if __VERSION__ >= 130
#define COMPAT_VARYING out
#define COMPAT_ATTRIBUTE in
#define COMPAT_TEXTURE texture
#else
#define COMPAT_VARYING varying
#define COMPAT_ATTRIBUTE attribute
#define COMPAT_TEXTURE texture2D
#endif

#ifdef GL_ES
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif

//COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
//COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
// out variables go here as COMPAT_VARYING whatever

vec4 _oPosition1;
//uniform mat4 MVPMatrix;
uniform COMPAT_PRECISION int FrameDirection;
uniform COMPAT_PRECISION int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize = vec2(2000, 1000);
uniform COMPAT_PRECISION vec2 TextureSize = vec2(2000, 1000);
uniform COMPAT_PRECISION vec2 InputSize = vec2(2000, 1000);

// compatibility #defines
#define vTexCoord TEX0.xy
#define SourceSize vec4(TextureSize, 1.0 / TextureSize) //either TextureSize or InputSize
#define OutSize vec4(OutputSize, 1.0 / OutputSize)

void main()
{
	mat4 MVPMatrix = gl_ModelViewProjectionMatrix;
	vec4 VertexCoord = gl_Vertex;
	vec4 TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_Position = MVPMatrix * VertexCoord;
    TEX0.xy = TexCoord.xy;
}
