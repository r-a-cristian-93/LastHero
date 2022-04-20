#version 130

out vec4 COL0;
out vec4 TEX0;

void main() {
  	mat4 MVPMatrix = gl_ModelViewProjectionMatrix;
		vec4 VertexCoord = gl_Vertex;
		vec4 TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	
    gl_Position = VertexCoord.x * MVPMatrix[0] + VertexCoord.y * MVPMatrix[1] + VertexCoord.z * MVPMatrix[2] + VertexCoord.w * MVPMatrix[3];
    TEX0.xy = TexCoord.xy;
    gl_FrontColor = gl_Color;
    COL0 = gl_Color;  
}
