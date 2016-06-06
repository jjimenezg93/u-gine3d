#include "../include/scene.h"
#include "../include/renderer.h"
#include "../include/entity.h"
#include "../include/light.h"
#include "../include/smartptr.h"

Ptr<Scene> Scene::mInstance = nullptr;

Scene::Scene() {
	Renderer::Instance()->Setup3D();
}

void Scene::SetModel(const glm::mat4 & m) {
	mModelMatrix = m;
	Renderer::Instance()->SetMatrices(mModelMatrix, mCurrentCamera->GetView(),
		mCurrentCamera->GetProjection());
}

void Scene::AddEntity(Ptr<Entity> entity) {
	if (entity != nullptr) {
		mEntities.Add(entity);
		if (entity.DownCast<Camera>() != nullptr) {
			mCameras.Add(entity.DownCast<Camera>());
		} else if (entity.DownCast<Light>() != nullptr) {
			mLights.Add(entity.DownCast<Light>());
		}
	}
}

void Scene::RemoveEntity(Ptr<Entity> entity) {
	if (entity != nullptr) {
		mEntities.Remove(entity);
		if (entity.DownCast<Camera>() != nullptr) {
			mCameras.Remove(entity.DownCast<Camera>());
		} else if (entity.DownCast<Light>() != nullptr) {
			mLights.Remove(entity.DownCast<Light>());
		}
	}
}

void Scene::Update(float elapsed) {
	uint16 numEntities = mEntities.Size();
	for (uint16 index = 0; index < numEntities; ++index) {
		mEntities[index]->Update(elapsed);
	}
}

void Scene::Render() {
	uint16 numCameras = mCameras.Size();
	uint16 numEntities = mEntities.Size();
	uint32 numLights = mLights.Size();
	for (uint16 iCamera = 0; iCamera < numCameras; ++iCamera) {
		mCurrentCamera = mCameras[iCamera];
		mCurrentCamera->Prepare();
		if (mLights.Size() != 0) {
			Renderer::Instance()->EnableLighting(true);
			for (uint32 iLights = 0; iLights < numLights; ++iLights) {
				mLights[iLights]->Prepare();
			}
		}
		for (uint16 iEntity = 0; iEntity < numEntities; ++iEntity) {
			mEntities[iEntity]->Render();
		}
	}


	for (uint32 iLights = 0; iLights < MAX_LIGHTS; ++iLights) {
		Renderer::Instance()->EnableLight(iLights, false);
	}
	Renderer::Instance()->EnableLighting(false);
}
