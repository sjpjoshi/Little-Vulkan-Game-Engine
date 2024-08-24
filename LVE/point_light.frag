#version 450

layout(location = 0) in vec2 fragOffset;
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
	vec4 position;
	vec4 color;
	float radius;

} push;

#define M_PI 3.14159265358979323846

void main() {
	float dis = sqrt(dot(fragOffset, fragOffset));
	if(dis >= 1.0) {
		discard; 
	} // if

	float cosDis = 0.5 * ( cos(dis * M_PI) + 1.0 );
	outColor = vec4(push.color.xyz + cosDis, cosDis);
	// will turn the cos function to this
	// f(x) = (1/2) * ( cos(pi * x) + 1 ) 

} // main