#include "../include/scene.h"
#include "../include/renderer.h"
#include "../include/entity.h"

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
		}
	}
}

void Scene::RemoveEntity(Ptr<Entity> entity) {
	if (entity != nullptr) {
		mEntities.Remove(entity);
		if (entity.DownCast<Camera>() != nullptr) {
			mCameras.Remove(entity.DownCast<Camera>());
		}
	}
}

void Scene::Update(float elapsed) {
	uint16_t numEntities = mEntities.Size();
	for (uint16_t index = 0; index < numEntities; ++index) {
		mEntities[index]->Update(elapsed);
	}
}

void Scene::Render() {
	uint16_t numCameras = mCameras.Size();
	uint16_t numEntities = mEntities.Size();
	for (uint16_t iCamera = 0; iCamera < numCameras; ++iCamera) {
		mCurrentCamera = mCameras[iCamera];
		mCurrentCamera->Prepare();
		for (uint16_t iEntity = 0; iEntity < numEntities; ++iEntity) {
			mEntities[iEntity]->Render();
		}
	}
}
