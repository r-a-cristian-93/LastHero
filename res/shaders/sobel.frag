#version 120

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float amount = 0.5;

// NOTE: Add here your custom variables
vec2 resolution = vec2(800.0, 450.0);

void main() {
	vec2 fragTexCoord = gl_TexCoord[0].xy;
	vec4 fragColor = texture2D(texture0, gl_TexCoord[0].xy);

    float x = 1/resolution.x/amount;
    float y = 1/resolution.y/amount;

    vec4 horizEdge = vec4(0.0);
    horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y - y))*1.0;
    horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y    ))*2.0;
    horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y + y))*1.0;
    horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y - y))*1.0;
    horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y    ))*2.0;
    horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y))*1.0;

    vec4 vertEdge = vec4(0.0);
    vertEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y - y))*1.0;
    vertEdge -= texture2D(texture0, vec2(fragTexCoord.x    , fragTexCoord.y - y))*2.0;
    vertEdge -= texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y - y))*1.0;
    vertEdge += texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y + y))*1.0;
    vertEdge += texture2D(texture0, vec2(fragTexCoord.x    , fragTexCoord.y + y))*2.0;
    vertEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y))*1.0;

    vec3 edge = sqrt((horizEdge.rgb*horizEdge.rgb) + (vertEdge.rgb*vertEdge.rgb));

    gl_FragColor = vec4(edge, texture2D(texture0, fragTexCoord).a);
}
