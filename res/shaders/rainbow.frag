uniform sampler2D texture;
uniform float amount = 0.5;

void main() {
	vec4 pixel = texture2D(texture, gl_TexCoord[0].st);
	vec3 base_color = vec3(pixel.r, pixel.g, pixel.b);

	vec2 st = gl_FragCoord.xy/1000;
    vec3 rainbow_color = vec3(st.x - st.y, st.y, st.x);

	gl_FragColor = vec4(mix(base_color, rainbow_color, amount), pixel.a);
}
