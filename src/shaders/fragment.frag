#version 330 core

in vec2 tex_coords;
in vec3 normal;
in vec3 frag_pos;

uniform vec4 u_color;
uniform sampler2D tex;

const vec3 AMBIENT_COLOR = vec3(1.0, 1.0, 1.0);
const float AMBIENT_STRENGTH = 0.1;
const float DIFFUSE_STRENGTH = 0.4;
const float SPECULAR_STRENGTH = 0.4;

struct DirectionalLight {
	vec3 dir;
	vec3 color;
};

uniform DirectionalLight directional_light;

struct SpotLight {
	vec3 position;
	vec3 color;
};

uniform SpotLight spotlights[10];
uniform int num_spotlights;

out vec4 FragColor;

vec3 calculate_directional_light(vec3 normal);
vec3 calculate_spot_light(SpotLight light, vec3 normal);

void main() {

	vec3 n_normal = normalize(normal);

	vec3 albedo = texture(tex, tex_coords).rgb;

	// Ambient Lighting
	vec3 ambient = AMBIENT_COLOR * AMBIENT_STRENGTH;

	// Dir light
	vec3 directional_light = calculate_directional_light(n_normal);
	directional_light = vec3(0);

	// Spot Lights
	vec3 spot_light_result = vec3(0);
	for (int i = 0; i < num_spotlights; i++) {
		spot_light_result += calculate_spot_light(spotlights[i], n_normal);
	}

	// Resulting color
	vec3 lighting = (ambient + directional_light + spot_light_result) * albedo;
	FragColor = vec4(lighting, 1.0);
}

vec3 calculate_directional_light(vec3 normal) {
	float diffuse_factor = max(dot(normal, directional_light.dir), 0.0);
	return directional_light.color * diffuse_factor * DIFFUSE_STRENGTH;
}

vec3 calculate_spot_light(SpotLight light, vec3 normal) {
	vec3 dir_light_to_frag = normalize(frag_pos - light.position);

	// Diffuse
	float diffuse_factor = max(dot(normal, dir_light_to_frag), 0.0);
	vec3 diffuse = light.color * diffuse_factor * DIFFUSE_STRENGTH;

	// Specular
	vec3 reflected = reflect(dir_light_to_frag, normal);
	float specular_factor = pow(max(dot(normalize(frag_pos), reflected), 0.0), 2);
	vec3 specular = vec3(1.0, 1.0, 1.0) * specular_factor * SPECULAR_STRENGTH;

	return diffuse + specular;
}
