#include <iostream>

#include "../lib/rapidjson/document.h"
#include "../lib/rapidjson/filestream.h"
#include "../include/mesh.h"
#include "../include/resourcemanager.h"

Ptr<Mesh> Mesh::Create() {
	return Ptr<Mesh>();
}

Ptr<Mesh> Mesh::Create(const String & filename) {
	Ptr<Mesh> mesh = new Mesh(filename);

	FILE * meshFile = fopen(filename.ToCString(), "rb");
	rapidjson::Document document;
	rapidjson::FileStream fs(meshFile);
	document.ParseStream<0>(fs);

	if (document.HasParseError()) return nullptr;
	if (!document.IsObject()) return nullptr;

	if (!document.HasMember("submeshes") || !document["submeshes"].IsArray()) return nullptr;

	const rapidjson::Value &submeshes = document["submeshes"];
	rapidjson::SizeType numMeshes = submeshes.Size();
	//for each submesh
	for (rapidjson::Value::ConstValueIterator itr = submeshes.Begin();
	itr != submeshes.End(); ++itr) {
		//texture
		String tex = String("data/") + (*itr)["texture"].GetString();
		Ptr<Submesh> submesh = Submesh::Create(ResourceManager::Instance()->LoadTexture(tex));
		
		//indices
		rapidjson::Value::ConstValueIterator indIt = (*itr)["indices"].Begin();
		while (indIt != (*itr)["indices"].End()) {
			//it's storing them in inverse order -> first mesh: 2, 0, 1, 3, 2, 1...
			submesh->AddTriangle(indIt++->GetInt(), indIt++->GetInt(), indIt++->GetInt());
		}

		mesh->AddSubmesh(submesh);
	}


	return mesh;
}

Mesh::Mesh(const String & filename) {

}

void Mesh::Render() {
	uint16 numSubmeshes = mSubmeshes.Size();
	for (uint16 i = 0; i < numSubmeshes; ++i) {
		mSubmeshes[i]->Render();
	}
}
