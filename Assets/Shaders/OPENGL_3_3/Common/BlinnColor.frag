#version 330 core

in vec3 vNormal;

uniform vec4 uColor; 

out vec4 FragColor;

void main() {
	vec3 lightPos = vec3(1, -1, -2);
	vec3 lightDir = normalize(-lightPos);

	vec4 color = uColor;
	vec4 ambient = color * 0.5;
	vec4 diffuse = vec4(vec3(dot(lightDir, vNormal)), 1) * 0.5;

	FragColor = ambient + diffuse;
}
