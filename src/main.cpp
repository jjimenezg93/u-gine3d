#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "../include/u-gine.h"
#define FULLSCREEN false

int main() {
	if ( FULLSCREEN )	Screen::Instance()->Open(Screen::Instance()->GetDesktopWidth(), Screen::Instance()->GetDesktopHeight(), true);
	else				Screen::Instance()->Open(800, 600, false);

	Screen::Instance()->ShowMouse(true);
	float prevMouseX = Screen::Instance()->GetMouseX(), prevMouseY = Screen::Instance()->GetMouseY();
	float mouseX = Screen::Instance()->GetMouseX(), mouseY = Screen::Instance()->GetMouseY();

	Ptr<Mesh> mesh = ResourceManager::Instance()->LoadMesh("data/box.msh");

	Ptr<Model> model = Model::Create(mesh);
	model->GetPosition() = glm::vec3(0.f, 0.f, 0.f);

	float fovX = 45.f;
	float fovY = 45.f;
	float farZ = 1.f;
	float nearZ = 0.1f;
	/*glm::mat4 projMat = glm::mat4(atan2f(fovX, 2), 0.f, 0.f, 0.f,
		0.f, atan2f(fovY, 2), 0.f, 0.f,
		0.f, 0.f, -(farZ + nearZ) / farZ - nearZ, -1.f,
		0.f, 0.f, -2 * (nearZ * farZ) / farZ - nearZ, 0.f);*/
	Ptr<Camera> camera = Camera::Create();
	camera->GetPosition() = glm::vec3(0.f, 0.f, 4.f);
	//camera->SetProjection(projMat);
	camera->SetProjection(glm::perspective(fovY, fovX, nearZ, farZ));
	camera->GetTarget() = model->GetPosition();
	camera->SetUsesTarget(true);
	camera->SetColor(glm::vec3(1.f, 1.f, 1.f));
	camera->SetViewport(0, 0, Screen::Instance()->GetWidth(), Screen::Instance()->GetHeight());

	Scene::Instance()->AddEntity(model.UpCast<Entity>());
	Scene::Instance()->AddEntity(camera.UpCast<Entity>());

	while ( !Screen::Instance()->ShouldClose() && !Screen::Instance()->IsKeyPressed(GLFW_KEY_ESCAPE) ) {
		Scene::Instance()->Update(Screen::Instance()->GetElapsed());
		Scene::Instance()->Render();

		/*glm::rotate(camera->GetRotation(), glm::radians(static_cast<float>(1)),
			glm::vec3(0.f, 0.f, 1.f));*/
		//camera->Move(glm::vec3(0.f, 0.f, -1.f));

		Screen::Instance()->Refresh();
		Screen::Instance()->SetTitle(Renderer::Instance()->GetProgramError());
	}

	ResourceManager::Instance()->FreeResources();

	return 0;
}
