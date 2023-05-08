#version 130

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

out COMPAT_PRECISION vec4 FragColor;

in vec4 TEX0;
uniform sampler2D Texture;

#define OUTLINE_WIDTH 2.0

void main() {
	vec2 uv = TEX0.xy;
	vec4 col = texture2D(Texture, uv);

	ivec2 tex_size = textureSize(Texture, 0);
	float tex_w = float(tex_size.x);
	float tex_h = float(tex_size.y);

	vec4 color_left = texture2D(Texture, vec2(TEX0.x - OUTLINE_WIDTH/tex_w, TEX0.y));
	vec4 color_right = texture2D(Texture, vec2(TEX0.x + OUTLINE_WIDTH/tex_w, TEX0.y));
	vec4 color_top = texture2D(Texture, vec2(TEX0.x, TEX0.y - OUTLINE_WIDTH/tex_h));
	vec4 color_bottom = texture2D(Texture, vec2(TEX0.x, TEX0.y + OUTLINE_WIDTH/tex_h));

	if ( (color_left.w < 0.5 && col.w > 0.5) ||
		(color_right.w < 0.5 && col.w > 0.5) ||
		(color_top.w < 0.5 && col.w > 0.5) ||
		(color_bottom.w < 0.5 && col.w > 0.5) )
	{
		col.w = 1.0;
	}
	else
		col.w = 0.0;

	FragColor = col;
}