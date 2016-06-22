#include "..\include\light.h"
#include "..\include\scene.h"
#include "..\include\camera.h"
#include "..\include\renderer.h"
#include "../lib/glm/gtx/transform.hpp"

bool Light::lightsUsed[MAX_LIGHTS] = {false, false, false, false, false, false, false, false};

Ptr<Light> Light::Create() {
	Ptr<Light> newLight = new Light();
	if (newLight->mIndex < MAX_LIGHTS) {
		return newLight;
	} else
		return nullptr;
}

void Light::Prepare() {
	Ptr<Camera> currCamera = Scene::Instance()->GetCurrentCamera();
	glm::vec4 position;
	if (mType == Type::DIRECTIONAL) {
		position = currCamera->GetView() * glm::vec4(GetPosition().x, GetPosition().y,
			GetPosition().z, 0);
	} else if (mType == Type::POINT) {
		position = currCamera->GetView() * glm::vec4(GetPosition().x, GetPosition().y,
			GetPosition().z, 1);
	}
	Renderer::Instance()->EnableLight(mIndex, true);
	Renderer::Instance()->SetLightData(mIndex, position, mColor, mAttenuation);
}

Light::Light(): mIndex(-1), mColor(1.f, 1.f, 1.f),
mType(Light::Type::DIRECTIONAL), mAttenuation(0.f) {
	uint32 i = 0;
	uint32 size = sizeof(lightsUsed);
	while (i < size) {
		if (lightsUsed[i] == false) {
			mIndex = i;
			lightsUsed[mIndex] = true;
			break;
		}
		i++;
	}
}

Light::~Light() {
	lightsUsed[mIndex] = false;
}