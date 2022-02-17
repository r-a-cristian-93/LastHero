#version 120

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// NOTE: Add here your custom variables

// NOTE: Render size values must be passed from code
const float renderWidth = 800;
const float renderHeight = 600;

float pixelWidth = 5.0;
float pixelHeight = 5.0;

void main() {
	vec2 fragTexCoord = gl_TexCoord[0].xy;
	vec4 fragColor = texture2D(texture0, gl_TexCoord[0].xy);

    float dx = pixelWidth*(1.0/renderWidth);
    float dy = pixelHeight*(1.0/renderHeight);

    vec2 coord = vec2(dx*floor(fragTexCoord.x/dx), dy*floor(fragTexCoord.y/dy));

    vec3 tc = texture2D(texture0, coord).rgb;

    gl_FragColor = vec4(tc, 1.0);
}
