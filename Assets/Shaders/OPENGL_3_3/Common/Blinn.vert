#version 330 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 uWorld;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vNormal;
out vec2 vUV;

void main() {
	vec4 worldPos = uWorld * vec4(aPosition,1);
	gl_Position = (uProj * uView) * worldPos;

	vNormal = aNormal;
	vUV = aUV;
}
