#version 120

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse = vec4(1,1,1,1);

void main() {
	vec2 fragTexCoord = gl_TexCoord[0].xy;
	vec4 fragColor = texture2D(texture0, gl_TexCoord[0].xy);

    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragTexCoord)*colDiffuse*fragColor;

    // Convert texel color to grayscale using NTSC conversion weights
    float gray = dot(texelColor.rgb, vec3(0.299, 0.587, 0.114));

    // Calculate final fragment color
    gl_FragColor = vec4(gray, gray, gray, texelColor.a);
}
