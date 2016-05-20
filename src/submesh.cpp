#include "../include/submesh.h"

Ptr<Submesh> Submesh::Create(Ptr<Texture> tex) {
	return new Submesh(tex);
}

Submesh::Submesh(Ptr<Texture> tex) {
	mVertexBuffer = Renderer::Instance()->CreateBuffer();
	mIndexBuffer = Renderer::Instance()->CreateBuffer();
	Renderer::Instance()->SetVertexBufferData(mVertexBuffer, &mVertices[0], sizeof(mVertices[0]));
	Renderer::Instance()->SetIndexBufferData(mIndexBuffer, &mIndices[0], sizeof(mIndices[0]));
	SetTexture(tex);
}

Submesh::~Submesh() {
	Renderer::Instance()->FreeBuffer(mVertexBuffer);
	Renderer::Instance()->FreeBuffer(mIndexBuffer);
}

void Submesh::AddVertex(const Vertex & v) {
	mVertices.Add(v);
}

void Submesh::AddTriangle(uint32 v0, uint32 v1, uint32 v2) {
	mIndices.Add(v0);
	mIndices.Add(v1);
	mIndices.Add(v2);
}

void Submesh::Rebuild() {
	Renderer::Instance()->SetVertexBufferData(mVertexBuffer, &mVertices[0], sizeof(mVertices[0]));
	Renderer::Instance()->SetIndexBufferData(mIndexBuffer, &mIndices[0], sizeof(mIndices[0]));
}

void Submesh::Render() {
	if (mTexture->GetHandle()) {
		Renderer::Instance()->SetTexture(mTexture->GetHandle());
	} else {
		Renderer::Instance()->SetTexture(0);
	}
	Renderer::Instance()->DrawBuffers(mVertexBuffer, mIndexBuffer, mIndices.Size());
}
