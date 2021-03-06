#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

in vec4 position;
in vec4 normal;
in vec4 texCoord;

out Data {
	vec4 pos;
	vec3 normal;
	vec3 eye;
	vec2 tex_coord;
} DataOut;

void main () {

	vec4 pos = m_viewModel * position;
	DataOut.pos = pos;
	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.eye = vec3(-pos);
	DataOut.tex_coord = texCoord.st;

	gl_Position = m_pvm * position;	
}