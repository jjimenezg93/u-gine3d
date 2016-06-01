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
		String tex;
		if (itr->HasMember("texture")) {
			tex = String(filename).ExtractDir() + "/" + (*itr)["texture"].GetString();
		} else tex = "";
		Ptr<Submesh> submesh = Submesh::Create(ResourceManager::Instance()->LoadTexture(tex));

		//indices
		uint32 i0, i1, i2;
		rapidjson::Value::ConstValueIterator indIt = (*itr)["indices"].Begin();
		while (indIt != (*itr)["indices"].End()) {
			i0 = static_cast<uint32>(indIt++->GetInt());
			i1 = static_cast<uint32>(indIt++->GetInt());
			i2 = static_cast<uint32>(indIt++->GetInt());
			submesh->AddTriangle(i0, i1, i2);
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
		if (itr->HasMember("texcoords") && tex != "") {
			uint32 i = 0;
			float tcU, tcV;
			indIt = (*itr)["texcoords"].Begin();
			while (indIt != (*itr)["texcoords"].End()) {
				tcU = static_cast<float>(indIt++->GetDouble());
				tcV = static_cast<float>(indIt++->GetDouble());
				submesh->GetVertices()[i++].mTexCoords = glm::vec2(tcU, tcV);
			}
		}

		//normals
		if (itr->HasMember("normals")) {
			uint32 iN = 0;
			indIt = (*itr)["normals"].Begin();
			if (indIt) {
				while (indIt != (*itr)["normals"].End()) {
					float normX = static_cast<float>(indIt++->GetDouble());
					float normY = static_cast<float>(indIt++->GetDouble());
					float normZ = static_cast<float>(indIt++->GetDouble());
					submesh->GetVertices()[iN++].mNormal = glm::vec3(normX, normY, normZ);
				}
			}
		}

		//diffuse color
		if (itr->HasMember("color")) {
			indIt = (*itr)["color"].End();
			if (indIt) {
				glm::vec3 diffuse((--indIt)->GetInt(), (--indIt)->GetInt(), (--indIt)->GetInt());
				submesh->SetColor(diffuse);
			}
		}

		//shininess
		if (itr->HasMember("shininess")) {
			submesh->SetShininess((*itr)["shininess"].GetInt());
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