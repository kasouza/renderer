#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coords;
layout (location = 2) in vec3 a_normal;
// in vec2 a_pos;

uniform mat4 modelview_matrix;
uniform mat4 projection;
uniform mat3 normal_matrix;

out vec2 tex_coords;
out vec3 normal;
out vec3 frag_pos;

void main() {
	vec4 vertex_pos = modelview_matrix * vec4(a_pos, 1.0f);
	frag_pos = vertex_pos.xyz;
	gl_Position = projection * vertex_pos;
	tex_coords = a_tex_coords;
	normal = normal_matrix * a_normal;
}
