#include "lve_game_object.hpp"

namespace lve {
	glm::mat4 TransformComponent::mat4() {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		return glm::mat4{
			{
				scale.x * (c1 * c3 + s1 * s2 * s3),
				scale.x * (c2 * s3),
				scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},

			{
				scale.y * (c3 * s1 * s2 - c1 * s3),
				scale.y * (c2 * c3),
				scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},

			{
				scale.z * (c2 * s1),
				scale.z * (-s2),
				scale.z * (c1 * c2),
				0.0f,
			},

			{translation.x, translation.y, translation.z, 1.0f} 
		
		}; // mat4

	} // mat4

	glm::mat3 TransformComponent::normalMatrix() {
		
		const glm::vec3 inverseScale = 1.0f / scale;

		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		return glm::mat3{
			{
				inverseScale.x * (c1 * c3 + s1 * s2 * s3),
				inverseScale.x * (c2 * s3),
				inverseScale.x * (c1 * s2 * s3 - c3 * s1),
			},

			{
				inverseScale.y * (c3 * s1 * s2 - c1 * s3),
				inverseScale.y * (c2 * c3),
				inverseScale.y * (c1 * c3 * s2 + s1 * s3),
			},

			{
				inverseScale.z * (c2 * s1),
				inverseScale.z * (-s2),
				inverseScale.z * (c1 * c2),
			},

		}; // mat3

	} // normalMatrix

	LveGameObject LveGameObject::makePointLight(float intensity, float radius, glm::vec3 color) {
		LveGameObject gameObj = LveGameObject::createGameObject();
		gameObj.color = color;
		gameObj.transform.scale.x = radius;
		gameObj.pointLight = std::make_unique<PointLightComponent>();
		gameObj.pointLight->lightIntensity = intensity;
		return gameObj;
		 
	} // makePointLight

}// lve