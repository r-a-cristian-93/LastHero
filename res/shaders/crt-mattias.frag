// CRT Emulation
// inspired by crt-mattias
// https://github.com/libretro/glsl-shaders/blob/master/crt/shaders/crt-mattias.glsl
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
in vec4 COL0;
uniform COMPAT_PRECISION vec2 ScreenSize = vec2(1024, 768);
uniform sampler2D Texture;

#define FULL_RETRO

#ifdef FULL_RETRO
#define CURVE_SCREEN 1		// 0-OFF, 1-ON
#define BLUR_TYPE 4			// 0-NONE, 1-FAST, 2-FAST2, 3-ORIGINAL
#define BLUR_AMOUNT 0.1
#define CORNER_RADIUS 0.5
#define CORNER_STRENGTH 1
#define VIGNETTE_RADIUS 50
#define VIGNETTE_STRENGTH 0.08
#define BRIGHTNESS 1.0
#define CONTRAST 1.40
#define SCANLINE_INTENSITY 0.2
#else
#define CURVE_SCREEN 1		// 0-OFF, 1-ON
#define BLUR_TYPE 4			// 0-NONE, 1-FAST, 2-FAST2, 3-ORIGINAL
#define BLUR_AMOUNT 0.1
#define CORNER_RADIUS 0.5
#define CORNER_STRENGTH 3
#define VIGNETTE_RADIUS 50
#define VIGNETTE_STRENGTH 0.15
#define BRIGHTNESS 1.8
#define CONTRAST 1.0
#define SCANLINE_INTENSITY 0
#endif

vec2 curvature = vec2(4, 4);



vec3 sample( sampler2D tex, vec2 tc ) {
	vec3 s = pow(texture(tex,tc).rgb, vec3(2.2));
	return s;
}

vec3 blur_fast(sampler2D image, vec2 uv, vec2 resolution) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(BLUR_AMOUNT);
  color += texture2D(image, uv) * 0.29411764705882354;
  color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;
  color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;
  return color.xyz;
}

vec3 blur(sampler2D tex, vec2 tc, float offs) {
	vec4 xoffs = offs * vec4(-2.0, -1.0, 1.0, 2.0) / ScreenSize.x * BLUR_AMOUNT;
	vec4 yoffs = offs * vec4(-2.0, -1.0, 1.0, 2.0) / ScreenSize.y * BLUR_AMOUNT;

	vec3 color = vec3(0.0, 0.0, 0.0);
	color += sample(tex,tc + vec2(xoffs.x, yoffs.x)) * 0.00366;
	color += sample(tex,tc + vec2(xoffs.y, yoffs.x)) * 0.01465;
	color += sample(tex,tc + vec2(    0.0, yoffs.x)) * 0.02564;
	color += sample(tex,tc + vec2(xoffs.z, yoffs.x)) * 0.01465;
	color += sample(tex,tc + vec2(xoffs.w, yoffs.x)) * 0.00366;

	color += sample(tex,tc + vec2(xoffs.x, yoffs.y)) * 0.01465;
	color += sample(tex,tc + vec2(xoffs.y, yoffs.y)) * 0.05861;
	color += sample(tex,tc + vec2(    0.0, yoffs.y)) * 0.09524;
	color += sample(tex,tc + vec2(xoffs.z, yoffs.y)) * 0.05861;
	color += sample(tex,tc + vec2(xoffs.w, yoffs.y)) * 0.01465;

	color += sample(tex,tc + vec2(xoffs.x, 0.0)) * 0.02564;
	color += sample(tex,tc + vec2(xoffs.y, 0.0)) * 0.09524;
	color += sample(tex,tc + vec2(    0.0, 0.0)) * 0.15018;
	color += sample(tex,tc + vec2(xoffs.z, 0.0)) * 0.09524;
	color += sample(tex,tc + vec2(xoffs.w, 0.0)) * 0.02564;

	color += sample(tex,tc + vec2(xoffs.x, yoffs.z)) * 0.01465;
	color += sample(tex,tc + vec2(xoffs.y, yoffs.z)) * 0.05861;
	color += sample(tex,tc + vec2(    0.0, yoffs.z)) * 0.09524;
	color += sample(tex,tc + vec2(xoffs.z, yoffs.z)) * 0.05861;
	color += sample(tex,tc + vec2(xoffs.w, yoffs.z)) * 0.01465;

	color += sample(tex,tc + vec2(xoffs.x, yoffs.w)) * 0.00366;
	color += sample(tex,tc + vec2(xoffs.y, yoffs.w)) * 0.01465;
	color += sample(tex,tc + vec2(    0.0, yoffs.w)) * 0.02564;
	color += sample(tex,tc + vec2(xoffs.z, yoffs.w)) * 0.01465;
	color += sample(tex,tc + vec2(xoffs.w, yoffs.w)) * 0.00366;

	return color;
}

vec2 curve(vec2 uv) {
    uv = uv * 2.0 -1.0;
    vec2 offset = abs(uv.yx) / vec2(curvature.x, curvature.y);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

vec4 roundness(vec2 uv, vec2 resolution, float opacity, float roundness) {
    float intensity = uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
    return vec4(vec3(clamp(pow((resolution.x / roundness) * intensity, opacity), 0.0, 1.0)), 1.0);
}

void main() {
    vec2 uv = TEX0.xy;

	// blur image
    vec3 col;

    if (BLUR_TYPE == 0) col = texture2D(Texture, uv).xyz;
    if (BLUR_TYPE == 1) col = blur_fast(Texture, uv, ScreenSize);
    if (BLUR_TYPE == 2) {
		col.x = blur_fast(Texture, uv+0.002, ScreenSize).x;
		col.y = blur_fast(Texture, uv+0.000, ScreenSize).y;
		col.z = blur_fast(Texture, uv+0.001, ScreenSize).z;
	}
	if (BLUR_TYPE == 3) {
		col.r = blur(Texture,vec2(uv.x+0.0009,uv.y+0.0009),1.2).x+0.005;
		col.g = blur(Texture,vec2(uv.x+0.000,uv.y-0.0015),1.2).y+0.005;
		col.b = blur(Texture,vec2(uv.x-0.0015,uv.y+0.000),1.2).z+0.005;
	}
	if (BLUR_TYPE == 4) {
		col.r = blur(Texture,vec2(uv.x+0.0002,uv.y+0.0002),1.2).x+0.005;
		col.g = blur(Texture,vec2(uv.x+0.000,uv.y-0.0005),1.2).y+0.005;
		col.b = blur(Texture,vec2(uv.x-0.0005,uv.y+0.000),1.2).z+0.005;
	}

	// scanlines
	if (SCANLINE_INTENSITY > 0) {
		float scans = clamp( 0.35+0.15*sin(uv.y*ScreenSize.y*1.5), 0.0, 1.0);
		float s = pow(scans, SCANLINE_INTENSITY);
		col = col*vec3(s) ;
		col = pow(col, vec3(0.45));
	}

    // curve screen
    if (CURVE_SCREEN > 0) uv = curve(uv);

    // round corners
    if (CORNER_RADIUS > 0) col *= roundness(uv, ScreenSize, CORNER_STRENGTH, CORNER_RADIUS).xyz;

	// vignette
    if (VIGNETTE_RADIUS > 0) col *= roundness(uv, ScreenSize, VIGNETTE_STRENGTH, VIGNETTE_RADIUS).xyz;

	// brightness
	col = mix( col, col * col, 0.3) * BRIGHTNESS;

	//contrast
	col = ((col - 0.5) * max(CONTRAST, 0.0)) + 0.5;

	if (uv.x < 0.0 || uv.x > 1.0) col *= 0.0;
	if (uv.y < 0.0 || uv.y > 1.0) col *= 0.0;

    FragColor = vec4(col,1.0) * COL0;
}
