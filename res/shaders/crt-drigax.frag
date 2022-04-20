//source https://github.com/Drigax/RooftopRampage_Source/commit/eb4acb1bd50439dccf843c14aa96e3c1c8a72276
#version 130

#ifdef GL_ES
   precision highp float;
#endif

#define USE_SCANLINES

in vec4 COL0;
in vec4 TEX0;

// Samplers
uniform sampler2D textureSampler;
// Parameters
vec2 curvature = vec2(4.0, 4.0);
float vignetteRoundness = 0.5;
float vignetteOpacity = 2.0;
vec2 screenResolution = vec2(1024, 768);
vec2 scanLineOpacity = vec2(0.06, 0);
vec2 scanLineScale = vec2(0.7, 0.2);
float brightness = 1.1;


vec2 curveRemapUV(vec2 in_uv) {
    // as we near the edge of our screen apply greater distortion using a cubic function
    vec2 uv = in_uv * 2.0 -1.0;
    vec2 offset = abs(uv.yx) / vec2(curvature.x, curvature.y);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

vec4 scanLineIntensity(float uv, float resolution, float opacity, float scale) {
     float intensity = sin(uv * resolution * scale * 3.14);
     intensity = ((0.5 * intensity) + 0.5) * 0.9 + 0.1;
     return vec4(vec3(pow(intensity, opacity)), 1.0);
}

vec4 vignetteIntensity(vec2 uv, vec2 resolution, float opacity, float roundness)
{
    float intensity = uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
    return vec4(vec3(clamp(pow((resolution.x / roundness) * intensity, opacity), 0.0, 1.0)), 1.0);
}

void main()
{
    vec2 remappedUV = curveRemapUV(vec2(TEX0.x, TEX0.y));
    vec4 baseColor = texture2D(textureSampler, remappedUV);

    baseColor *= vignetteIntensity(remappedUV, screenResolution, vignetteOpacity, vignetteRoundness);

    #ifdef USE_SCANLINES
			baseColor *= scanLineIntensity(remappedUV.x, screenResolution.y, scanLineOpacity.x, scanLineScale.x);
			baseColor *= scanLineIntensity(remappedUV.y, screenResolution.x, scanLineOpacity.y, scanLineScale.y);
			baseColor *= vec4(vec3(brightness), 1.0);
		#endif

    if (remappedUV.x < 0.0 || remappedUV.y < 0.0 || remappedUV.x > 1.0 || remappedUV.y > 1.0){
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        gl_FragColor = baseColor;
    }
}
