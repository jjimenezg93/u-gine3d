#include "../include/texture.h"
#include "../include/renderer.h"

Ptr<Texture> Texture::Create(const String& filename) {
	Ptr<Texture> pTex = new Texture(filename);
	if (!pTex->mHandle) {
		pTex = nullptr;
	}
	return pTex;
}

Texture::Texture(const String& filename) {
	mFilename = filename;
	mHandle = Renderer::Instance()->LoadTexture(mFilename, mWidth, mHeight);
}

Texture::~Texture() {
	Renderer::Instance()->FreeTexture(mHandle);
}
