#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;


layout(location = 0) out vec4 outColor;

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
	// to avoid calculating the normal each time
	vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3 specularLight = vec3(0.0);
	vec3 surfaceNormal = normalize(fragNormalWorld);

	vec3 cameraPosWorld = ubo.inView[3].xyz; 
	vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld); // surface world vector 

	for(int i = 0; i < ubo.numLights; i++) {
		PointLight light = ubo.pointLights[i];
		vec3 directionToLight = light.position.xyz - fragPosWorld;
		float attenuation = 1.0 / dot (directionToLight, directionToLight); // distance squared

		directionToLight = normalize(directionToLight);

		float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
		vec3 intensity = light.color.xyz * light.color.w * attenuation;

		diffuseLight += intensity * cosAngIncidence;

		// specular lighthing 
		vec3 halfAngle = normalize(directionToLight + viewDirection);
		float binnTerm = dot(surfaceNormal, halfAngle);
		binnTerm = clamp(binnTerm, 0, 1);
		binnTerm = pow(binnTerm, 512.0); // higher exponent -> sharper highlight

		specularLight += intensity * binnTerm;

	} // for
	
	outColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);

} // main