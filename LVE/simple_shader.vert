#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;
 

struct PointLight{
	vec4 position; // ignore w
	vec4 color; // w is intensity 

}; // PointLight

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projection;
	mat4 view;
	mat4 inView;
	vec4 ambientLightColor;
	
	// Vulkan has something called Specialization Constants
	// this is a method to pass constant values into shader programs at the time of pipeline creation
	PointLight pointLights[10]; // for now we have not implemented this just yet, so hardcoding for now is fine
	int numLights;

} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix; // proj * view * model
	mat4 normalMatrix;

} push;

void main() {
	vec4 positionWorld =  push.modelMatrix * vec4(position, 1.0);
	gl_Position = ubo.projection * ubo.view * positionWorld;

	fragNormalWorld = normalize( mat3(push.normalMatrix) * normal );
	fragPosWorld = positionWorld.xyz;
	fragColor = color;


}  // main