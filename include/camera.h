#ifndef UGINE_CAMERA_H
#define UGINE_CAMERA_H

#include "entity.h"

class Camera: public Entity {
public:
	static Ptr<Camera> Create();

	int32 GetViewportX() const { return mVX; }
	int32 GetViewportY() const { return mVY; }
	uint16 GetViewportWidth() const { return mVW; }
	uint16 GetViewportHeight() const { return mVH; }
	void SetViewport(int32 x, int32 y, uint16 w, uint16 h);

	void SetProjection(const glm::mat4& proj) { mProjMatrix = proj; }
	const glm::mat4& GetProjection() const { return mProjMatrix; }

	const glm::mat4& GetView() const { return mViewMatrix; }

	const glm::vec3& GetColor() const { return mColor; }
	void SetColor(const glm::vec3& color) { mColor = color; }

	bool GetUsesTarget() const { return mUsesTarget; }
	void SetUsesTarget(bool usesTarget) { mUsesTarget = usesTarget; }

	const glm::vec3& GetTarget() const { return mTarget; }
	glm::vec3& GetTarget() { return mTarget; }

	void Prepare();
	virtual void Render() {}
protected:
	Camera();
	virtual ~Camera() {}
private:
	glm::mat4 mProjMatrix;
	glm::mat4 mViewMatrix;
	int32 mVX, mVY;
	uint16 mVW, mVH;
	glm::vec3 mColor;
	bool mUsesTarget;
	glm::vec3 mTarget;
	friend class Ptr<Camera>;
	friend class Ptr<const Camera>;
};

#endif // UGINE_CAMERA_H