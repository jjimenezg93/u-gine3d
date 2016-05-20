#include "../include/texture.h"

Ptr<Texture> Texture::Create(const String& filename) {
	Ptr<Texture> pTex = new Texture(filename);
	if (!pTex->mHandle) {
		pTex = nullptr;
		return nullptr;
	} else {
		return pTex;
	}
}

Texture::Texture(const String& filename) {
	mFilename = filename;
	mHandle = Renderer::Instance()->LoadTexture(mFilename, mWidth, mHeight);
}

Texture::~Texture() {

}
