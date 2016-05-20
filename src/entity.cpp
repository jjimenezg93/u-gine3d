#include "../include/entity.h"
#include "../include/scene.h"

Ptr<Entity> Entity::Create() {
	return Ptr<Entity>();
}

Entity::Entity() {

}

void Entity::Move(const glm::vec3 & speed) {
	/*glm::vec3 axis;
	axis.x = mRotation.x / sqrt(pow(mRotation.x, 2) + pow(mRotation.y, 2) + pow(mRotation.z, 2));
	axis.y = mRotation.y / sqrt(pow(mRotation.x, 2) + pow(mRotation.y, 2) + pow(mRotation.z, 2));
	axis.z = mRotation.z / sqrt(pow(mRotation.x, 2) + pow(mRotation.y, 2) + pow(mRotation.z, 2));
	*/
	mPosition += glm::axis(GetRotation()) * speed;
}

void Entity::Render() {
	/*float ang = acos(mRotation.w) * 2;
	glm::vec3 axis;
	axis.x = mRotation.x / sqrt(pow(mRotation.x, 2) + pow(mRotation.y, 2) + pow(mRotation.z, 2));
	axis.y = mRotation.y / sqrt(pow(mRotation.x, 2) + pow(mRotation.y, 2) + pow(mRotation.z, 2));
	axis.z = mRotation.z / sqrt(pow(mRotation.x, 2) + pow(mRotation.y, 2) + pow(mRotation.z, 2));
	*/
	glm::mat4 modelMat;
	glm::translate(modelMat, mPosition);
	glm::rotate(modelMat, glm::angle(GetRotation()), glm::axis(GetRotation()));
	glm::scale(modelMat, mScale);
	/*//translation matrix
	glm::mat4 transMat(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
		mPosition.x, mPosition.y, mPosition.z, 1);

	//rotation matrix
	float c = cos(ang);
	float s = sin(ang);
	glm::vec4 col1(pow(axis.x, 2) * (1 - c) + c,
		axis.x * axis.y * (1 - c) + axis.z * s,
		axis.x * axis.z * (1 - c) - axis.y * s,
		0.f);
	glm::vec4 col2(axis.x * axis.y * (1 - c) - axis.z * s,
		pow(axis.y, 2) * (1 - c) + s,
		axis.y * axis.z * (1 - c) + axis.x * s,
		0.f);
	glm::vec4 col3(axis.x * axis.z * (1 - c) + axis.y * s,
		axis.y * axis.z * (1 - c) - axis.x * s,
		pow(axis.z, 2) * (1 - c) + c,
		0.f);
	glm::vec4 col4(0.f, 0.f, 0.f, 1.f);
	glm::mat4 rotMat(col1, col2, col3, col4);
	

	//scale matrix
	glm::mat4 scaleMat(mScale.x, 0.f, 0.f, 0.f, 0.f, mScale.y, 0.f, 0.f, 0.f, 0.f, mScale.z, 0.f,
		0.f, 0.f, 0.f, 1.f);

	glm::mat4 modelMat(transMat * rotMat * scaleMat);*/
	Scene::Instance()->SetModel(modelMat);
}
