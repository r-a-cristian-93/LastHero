#version 120



uniform vec2 screenSize = vec2(2000,1000);

uniform sampler2D u_texture;
uniform vec4 v_time;// = vec2(0.5,0.5,0.5,0.5);

const float RADIUS = 5;

const float SOFTNESS = 3;

const float blurSize = 1.0/1000.0;

void main() {
	vec2 vTexCoord = gl_TexCoord[0].xy;
	vec4 vColor = texture2D(u_texture, vTexCoord);

    vec4 texColor = vec4(0.0); // texture2D(u_texture, vTexCoord)
    texColor += texture2D(u_texture, vTexCoord - 4.0*blurSize) * 0.05;
    texColor += texture2D(u_texture, vTexCoord - 3.0*blurSize) * 0.09;
    texColor += texture2D(u_texture, vTexCoord - 2.0*blurSize) * 0.12;
    texColor += texture2D(u_texture, vTexCoord - blurSize) * 0.15;
    texColor += texture2D(u_texture, vTexCoord) * 0.16;
    texColor += texture2D(u_texture, vTexCoord + blurSize) * 0.15;
    texColor += texture2D(u_texture, vTexCoord + 2.0*blurSize) * 0.12;
    texColor += texture2D(u_texture, vTexCoord + 3.0*blurSize) * 0.09;
    texColor += texture2D(u_texture, vTexCoord + 4.0*blurSize) * 0.05;

    vec4 timedColor = (vColor + v_time);

    vec2 position = (gl_FragCoord.xy / screenSize.xy) - vec2(0.5);
    float len = length(position);

    float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, len);

    texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.5);

    gl_FragColor = vec4(texColor.rgb * timedColor.rgb, texColor.a) *2;
}
