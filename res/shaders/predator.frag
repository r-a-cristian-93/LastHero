#version 120

uniform sampler2D texture;
uniform vec4 colDiffuse;

void main()
{
	vec2 fragTexCoord = gl_TexCoord[0].xy;
	vec4 fragColor = texture2D(texture, gl_TexCoord[0].xy);

    // Texel color fetching from texture sampler
    vec3 texelColor = fragColor.rgb;
    vec3 colors[3];
    colors[0] = vec3(0.0, 0.0, 1.0);
    colors[1] = vec3(1.0, 1.0, 0.0);
    colors[2] = vec3(1.0, 0.0, 0.0);

    float lum = (texelColor.r + texelColor.g + texelColor.b)/3.0;

    int ix = (lum < 0.5)? 0:1;

    vec3 tc = mix(colors[ix], colors[ix + 1], (lum - float(ix)*0.5)/0.5);

    gl_FragColor = vec4(tc, 1.0);
}
