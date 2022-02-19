#version 130

// Parameter lines go here:
#pragma parameter SCANTHICK "SCANLINE THICKNESS" 2.0 2.0 4.0 1.0
#pragma parameter INTENSITY "SCANLINE INTENSITY" 0.6 0.0 1.0 0.02
#pragma parameter BRIGHTBOOST "LUMINANCE BOOST" 0.25 0.0 1.0 0.01
#pragma parameter shadowmask "MASK TYPE 0:CGWG, 1:LOTTES" 0.0 -1.0 1.0 1.0
#pragma parameter msk_size "MASK SIZE" 1.0 1.0 2.0 1.0
#pragma parameter CGWG "MASK STRENGTH" 0.3 0.0 1.0 0.1
#pragma parameter BLUR "BLUR STRENGTH" 0.6 0.0 1.0 0.1
#pragma parameter GAMMA "GAMMA" 0.45 0.0 0.80 0.01
#pragma parameter SATURATION "SATURATION" 1.0 0.0 2.0 0.05

//#if defined(VERTEX)

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

COMPAT_ATTRIBUTE vec4 VertexCoord;
COMPAT_ATTRIBUTE vec4 COLOR;
COMPAT_ATTRIBUTE vec4 TexCoord;
COMPAT_VARYING vec4 COL0;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec2 invDims;

vec4 _oPosition1;
uniform mat4 MVPMatrix;
uniform COMPAT_PRECISION int FrameDirection;
uniform COMPAT_PRECISION int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize = vec2(2000, 1000);
uniform COMPAT_PRECISION vec2 TextureSize = vec2(2000, 1000);
uniform COMPAT_PRECISION vec2 InputSize = vec2(2000, 1000);

void main()
{
	mat4 MVPMatrix = gl_ModelViewProjectionMatrix;
	vec4 VertexCoord = gl_Vertex;
	vec4 TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_Position = MVPMatrix * VertexCoord;
    COL0 = COLOR;
    TEX0.xy = TexCoord.xy * 1.00001;
    invDims=1.0/TextureSize.xy;
}
