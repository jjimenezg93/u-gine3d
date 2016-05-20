#include "../include/model.h"
#include "../include/mesh.h"

Ptr<Model> Model::Create(Ptr<Mesh> mesh) {
	if (mesh != nullptr)
		return new Model(mesh);
	else
		return nullptr;
}

Model::Model(Ptr<Mesh> mesh) {
	mMesh = mesh;
}

void Model::Render() {
	Entity::Render();
	mMesh->Render();
}


