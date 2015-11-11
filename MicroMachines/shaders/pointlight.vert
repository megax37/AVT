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
	float visibility;
} DataOut;

const float density = 0.035;
const float gradient = 1.5;

void main () {

	vec4 pos = m_viewModel * position;
	float distance = length(pos.xyz);
	DataOut.visibility = exp(-pow((distance*density), gradient));
	DataOut.visibility = clamp(DataOut.visibility, 0.0, 1.0);
	DataOut.pos = pos;
	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.eye = vec3(-pos);
	DataOut.tex_coord = texCoord.st;

	gl_Position = m_pvm * position;	
}