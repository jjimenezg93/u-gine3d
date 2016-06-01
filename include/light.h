#ifndef UGINE_LIGHT_H
#define UGINE_LIGHT_H

#include "smartptr.h"
#include "entity.h"

#ifndef MAX_LIGHTS
#define MAX_LIGHTS 8
#endif

class Light: public Entity {
public:
	enum Type {
		DIRECTIONAL,
		POINT
	};

	static Ptr<Light> Create();
	Type GetType() const { return mType; }
	void SetType(Type type) { mType = type; }
	const glm::vec3& GetColor() const { return mColor; }
	void SetColor(const glm::vec3& color) { mColor = color; }
	float GetAttenuation() const { return mAttenuation; }
	void SetAttenuation(float att) { mAttenuation = att; }

	void Prepare();
	virtual void Render() {}
protected:
	Light();
	virtual ~Light();
private:
	static bool lightsUsed[MAX_LIGHTS];

	int mIndex;
	Type mType;
	glm::vec3 mColor;
	float mAttenuation;
	friend class Ptr<Light>;
	friend class Ptr<const Light>;
};

#endif // UGINE_LIGHT_H