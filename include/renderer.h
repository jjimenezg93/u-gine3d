#ifndef UGINE_RENDERER_H
#define UGINE_RENDERER_H

#include "array.h"
#include "smartptr.h"
#include "string.h"
#include "types.h"

#ifndef MAX_LIGHTS
#define MAX_LIGHTS 8
#endif

class Renderer {
public:
	static Ptr<Renderer> Instance() { if (mInstance == 0) mInstance = new Renderer(); return mInstance; }

	// Setup
	void Setup3D();
	void SetMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void SetViewport(int x, int y, int w, int h);

	// Drawing
	void ClearColorBuffer(const glm::vec3& color);
	void ClearDepthBuffer();

	// Texture
	uint32 LoadTexture(const String& filename, uint32& width, uint32& height);
	void FreeTexture(uint32 tex);
	void SetTexture(uint32 tex);

	// Lights
	void SetDiffuse(const glm::vec3& color);
	void SetAmbient(const glm::vec3& color);
	void SetShininess(uint8 shininess);
	void EnableLighting(bool enable) { mLightingEnabled = enable; }
	void EnableLight(uint32 index, bool enabled);
	void SetLightData(uint32 index, const glm::vec4& vector,
		const glm::vec3& color, float attenuation);

	// VBO
	uint32 CreateBuffer();
	void FreeBuffer(uint32 buffer);
	void SetVertexBufferData(uint32 vertexBuffer, const void* data, uint32 dataSize);
	void SetIndexBufferData(uint32 indexBuffer, const void* data, uint32 dataSize);
	void DrawBuffers(uint32 vertexBuffer, uint32 indexBuffer, uint32 numIndices);

	// Shaders
	uint32 CreateProgram(const String& vertex, const String& fragment);
	void FreeProgram(uint32 program);
	void UseProgram(uint32 program);
	const String& GetProgramError();
protected:
	Renderer();
	virtual ~Renderer() {}
private:
	static Ptr<Renderer> mInstance;

	//Lights
	bool mLightsEnabled[MAX_LIGHTS];
	glm::vec4 mLightsPos[MAX_LIGHTS];
	glm::vec3 mLightsColor[MAX_LIGHTS];
	float mLightsAtt[MAX_LIGHTS];
	bool mLightingEnabled;

	//shaders' params
	int mMVPLoc;
	int mNormalMatLoc;
	int mModelViewMatLoc;
	int mTexSamplerLoc;
	int mVPosLoc;
	int mVTexLoc;
	int mVNormLoc;
	//int mLightsEnabledLoc[MAX_LIGHTS];
	int mLightsPosLoc[MAX_LIGHTS];
	int mLightsColorLoc[MAX_LIGHTS];
	int mLightsAttLoc[MAX_LIGHTS];
	int mDiffuseLoc;
	int mAmbientLoc;
	int mShininessLoc;
	int mUsesTextureLoc;

	uint32 mDefaultProgram;
	String mProgramError;
	friend class Ptr<Renderer>;
	friend class Ptr<const Renderer>;
};

#endif // UGINE_RENDERER_H
