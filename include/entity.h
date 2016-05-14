#ifndef UGINE_ENTITY_H
#define UGINE_ENTITY_H

#include "types.h"

class Entity {
public:
	static Ptr<Entity> Create();

	const glm::vec3& GetPosition() const { return mPosition; }
	glm::vec3& GetPosition() { return mPosition; }

	const glm::quat& GetRotation() const { return mRotation; }
	glm::quat& GetRotation() { return mRotation; }

	const glm::vec3& GetScale() const { return mScale; }
	glm::vec3& GetScale() { return mScale; }

	void Move(const glm::vec3& speed);

	virtual void Update(float elapsed) {}
	virtual void Render();
protected:
	Entity();
	virtual ~Entity() {}
private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;
	friend class Ptr<Entity>;
	friend class Ptr<const Entity>;
};

#endif // UGINE_ENTITY_H
