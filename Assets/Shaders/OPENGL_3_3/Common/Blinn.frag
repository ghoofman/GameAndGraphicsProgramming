#version 330 core

in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uColorTexture;

out vec4 FragColor;

void main() {
	vec3 lightPos = vec3(1, -1, -2);
	vec3 lightDir = normalize(-lightPos);

	vec4 color = texture2D(uColorTexture, vUV);
	vec4 diffuse = vec4(vec3(dot(lightDir, vNormal)), 1);

	FragColor = vec4((color.xyz * diffuse.xyz), 1);
}
