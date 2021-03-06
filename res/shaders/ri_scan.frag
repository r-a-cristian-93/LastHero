#version 120

#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#define COMPAT_PRECISION mediump
#else
#define COMPAT_PRECISION
#endif

#if __VERSION__ >= 130
#define COMPAT_VARYING in
#define COMPAT_TEXTURE texture
out COMPAT_PRECISION vec4 FragColor;
#else
#define COMPAT_VARYING varying
#define FragColor gl_FragColor
#define COMPAT_TEXTURE texture2D
#endif

uniform COMPAT_PRECISION int FrameDirection;
uniform COMPAT_PRECISION int FrameCount;
uniform COMPAT_PRECISION vec2 OutputSize;
uniform COMPAT_PRECISION vec2 TextureSize;
uniform COMPAT_PRECISION vec2 InputSize;
uniform sampler2D Texture;
COMPAT_VARYING vec4 TEX0;
COMPAT_VARYING vec2 omega;

// compatibility #defines
#define Source Texture
#define vTexCoord TEX0.xy

#define SourceSize vec4(TextureSize, 1.0 / TextureSize) //either TextureSize or InputSize
#define OutSize vec4(OutputSize, 1.0 / OutputSize)


#ifdef PARAMETER_UNIFORM
uniform COMPAT_PRECISION float SCANLINE_SINE_COMP_B, scanline_size;
#else
#define SCANLINE_SINE_COMP_B 0.25
#define scanline_size 4.0
#endif

void main()
{
   vec2 sine_comp = vec2(0.0, SCANLINE_SINE_COMP_B);
   vec3 res = COMPAT_TEXTURE(Source, vTexCoord).xyz;
   vec2 fragcoord = fract(floor(gl_FragCoord.xy * 1.0001) / scanline_size);
   vec3 scanline = (fragcoord.y > 0.5)? res : vec3(0.0);//res * ((1. - 0.75 * SCANLINE_SINE_COMP_B) + dot(sine_comp * sin((fragcoord / scanline_size) * omega), vec2(1.0, 1.0)));
   FragColor = vec4(scanline.x, scanline.y, scanline.z, 1.0);
}
