#include "../include/camera.h"
#include "../include/renderer.h"

Ptr<Camera> Camera::Create() {
	return new Camera();
}

Camera::Camera() {

}

void Camera::Prepare() {
	if (mUsesTarget) {
		mViewMatrix = glm::lookAt(GetPosition(), mTarget, glm::vec3(0.f, 1.f, 0.f));
		//to allow rolling -> have to calculate up's vector with sin and cos relative to y
	} else {
		glm::mat4 modelMat;
		modelMat = glm::rotate(modelMat, -glm::angle(GetRotation()),
			glm::axis(GetRotation()));
		mViewMatrix = glm::translate(modelMat, -GetPosition());
	}

	//define viewport
	Renderer::Instance()->SetViewport(mVX, mVY, mVW, mVH);
	Renderer::Instance()->ClearColorBuffer(mColor);
	Renderer::Instance()->ClearDepthBuffer();
}

void Camera::SetViewport(int32 x, int32 y, uint16 w, uint16 h) {
	mVX = x;
	mVY = y;
	mVW = w;
	mVH = h;
}