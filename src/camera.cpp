#include "../include/camera.h"
#include "../include/renderer.h"

Ptr<Camera> Camera::Create() {
	return new Camera();
}

Camera::Camera() {

}

void Camera::Prepare() {
	if (mUsesTarget) {
		glm::lookAt(GetPosition(), mTarget, glm::vec3(0.f, 1.f, 0.f));
	} else {
		/*glm::vec3 axis;
		axis.x = GetRotation().x / sqrt(pow(GetRotation().x, 2) + pow(GetRotation().y, 2) + pow(GetRotation().z, 2));
		axis.y = GetRotation().y / sqrt(pow(GetRotation().x, 2) + pow(GetRotation().y, 2) + pow(GetRotation().z, 2));
		axis.z = GetRotation().z / sqrt(pow(GetRotation().x, 2) + pow(GetRotation().y, 2) + pow(GetRotation().z, 2));
		glm::normalize(axis);
		glm::lookAt(GetPosition(), GetPosition() + axis, glm::vec3(0, 1, 0));*/
		glm::lookAt(GetPosition(), GetPosition() + glm::normalize(glm::axis(GetRotation())),
			glm::vec3(0.f, 0.f, 1.f));
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