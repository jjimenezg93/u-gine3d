#include "..\include\light.h"
#include "..\include\scene.h"
#include "..\include\camera.h"
#include "..\include\renderer.h"

bool Light::lightsUsed[MAX_LIGHTS];

Ptr<Light> Light::Create() {
	return new Light();
}

void Light::Prepare() {
	Ptr<Camera> currCamera = Scene::Instance()->GetCurrentCamera();
	Renderer::Instance()->SetAmbient(mColor);
	glm::vec4 position = glm::vec4(GetPosition().x, GetPosition().y,
		GetPosition().z, 1) * currCamera->GetView();
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
