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
		uint32 iX, iY, iZ;
		rapidjson::Value::ConstValueIterator indIt = (*itr)["indices"].Begin();
		while (indIt != (*itr)["indices"].End()) {
			iX = static_cast<uint32>(indIt++->GetInt());
			iY = static_cast<uint32>(indIt++->GetInt());
			iZ = static_cast<uint32>(indIt++->GetInt());
			submesh->AddTriangle(iX, iY, iZ);
		}

		//coords
		Vertex v;
		float cX, cY, cZ;
		indIt = (*itr)["coords"].Begin();
		while (indIt != (*itr)["coords"].End()) {
			cX = static_cast<float>(indIt++->GetDouble());
			cY = static_cast<float>(indIt++->GetDouble());
			cZ = static_cast<float>(indIt++->GetDouble());
			v.mPosition = glm::vec3(cX, cY, cZ);
			submesh->AddVertex(v);
		}

		//texcoords
		uint32 i = 0;
		float tcU, tcV;
		indIt = (*itr)["texcoords"].Begin();
		while (indIt != (*itr)["texcoords"].End()) {
			tcU = static_cast<float>(indIt++->GetDouble());
			tcV = static_cast<float>(indIt++->GetDouble());
			submesh->GetVertices()[i++].mTexCoords = glm::vec2(tcU, tcV);
		}

		mesh->AddSubmesh(submesh);
	}

	return mesh;
}

Mesh::Mesh(const String & filename) {
	mFilename = filename;
}

void Mesh::Render() {
	uint16 numSubmeshes = mSubmeshes.Size();
	for (uint16 i = 0; i < numSubmeshes; ++i) {
		mSubmeshes[i]->Render();
	}
}
