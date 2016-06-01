#include "../include/entity.h"
#include "../include/scene.h"

Ptr<Entity> Entity::Create() {
	return Ptr<Entity>();
}

Entity::Entity() : mPosition(0, 0, 0), mRotation(1, 0, 0, 0), mScale(1, 1, 1) {}

void Entity::Move(const glm::vec3 & speed) {
	mPosition += GetRotation() * speed;
}

void Entity::Render() {
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, mPosition);
	modelMat = glm::rotate(modelMat, glm::angle(GetRotation()), glm::axis(GetRotation()));
	modelMat = glm::scale(modelMat, mScale);

	Scene::Instance()->SetModel(modelMat);
}
