//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "../include/u-gine.h"
#define FULLSCREEN false

#define PRACTICE_1
//#define PRACTICE_2

int main() {
	if (FULLSCREEN)	Screen::Instance()->Open(Screen::Instance()->GetDesktopWidth(), Screen::Instance()->GetDesktopHeight(), true);
	else				Screen::Instance()->Open(800, 600, false);

#ifdef PRACTICE_1
	Screen::Instance()->ShowMouse(true);

	int32 prevMouseX = Screen::Instance()->GetMouseX(), prevMouseY = Screen::Instance()->GetMouseY();
	int32 mouseX = Screen::Instance()->GetMouseX(), mouseY = Screen::Instance()->GetMouseY();

	Ptr<Mesh> mesh = ResourceManager::Instance()->LoadMesh("data/box.msh");

	Ptr<Model> model = Model::Create(mesh);
	model->GetPosition() = glm::vec3(0.f, 0.f, 0.f);

	float fovX = 45.f;
	float fovY = 40.f;
	float farZ = 100.f;
	float nearZ = 0.1f;

	Ptr<Camera> camera = Camera::Create();
	camera->GetPosition() = glm::vec3(0.f, 0.f, 4.f);
	camera->GetRotation() = glm::quat(glm::vec3(0.f, 0.f, 0.f));
	camera->SetProjection(glm::perspective(glm::radians(fovY),
		Screen::Instance()->GetWidth() / static_cast<float>(Screen::Instance()->GetHeight()),
		nearZ, farZ));
	camera->GetTarget() = model->GetPosition();
	camera->SetUsesTarget(true);
	camera->SetColor(glm::vec3(1.f, 1.f, 1.f));
	//camera->SetColor(glm::vec3(0.2f, 0.2f, 0.2f));
	camera->SetViewport(0, 0, Screen::Instance()->GetWidth(), Screen::Instance()->GetHeight());

	Scene::Instance()->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));

	Scene::Instance()->AddEntity(model.UpCast<Entity>());
	Scene::Instance()->AddEntity(camera.UpCast<Entity>());

	while (!Screen::Instance()->ShouldClose() && !Screen::Instance()->IsKeyPressed(GLFW_KEY_ESCAPE)) {
		Scene::Instance()->Update(Screen::Instance()->GetElapsed());
		Scene::Instance()->Render();

		model->Move(glm::vec3(0.f, 1.f * Screen::Instance()->GetElapsed(), 0.f));

		camera->GetTarget() = model->GetPosition();
		model->GetRotation() *= glm::quat(glm::radians(glm::vec3(0,
			+32 * Screen::Instance()->GetElapsed(), 0)));
		//_sleep(500);
		Screen::Instance()->Refresh();
		Screen::Instance()->SetTitle(Renderer::Instance()->GetProgramError());
	}
#endif
#ifdef PRACTICE_2
	Screen::Instance()->ShowMouse(true);
	int32 prevMouseX = Screen::Instance()->GetMouseX(), prevMouseY = Screen::Instance()->GetMouseY();
	int32 mouseX = Screen::Instance()->GetMouseX(), mouseY = Screen::Instance()->GetMouseY();

	Ptr<Mesh> mesh = ResourceManager::Instance()->LoadMesh("data/teapot.msh");

	Ptr<Model> model = Model::Create(mesh);
	model->GetPosition() = glm::vec3(0.f, 0.f, 0.f);

	float fovY = 40.f;
	float farZ = 100.f;
	float nearZ = 0.1f;

	Ptr<Camera> camera = Camera::Create();
	camera->GetPosition() = glm::vec3(0.f, 2.f, 6.f);
	camera->GetRotation() = glm::quat(glm::vec3(0.f, 0.f, 0.f));
	camera->SetProjection(glm::perspective(glm::radians(fovY),
		Screen::Instance()->GetWidth() / static_cast<float>(Screen::Instance()->GetHeight()),
		nearZ, farZ));
	camera->GetTarget() = model->GetPosition();
	camera->SetUsesTarget(true);
	camera->SetColor(glm::vec3(0.2f, 0.2f, 0.2f));
	camera->SetViewport(0, 0, Screen::Instance()->GetWidth(), Screen::Instance()->GetHeight());

	Ptr<Light> blueLight = Light::Create();
	blueLight->SetColor(glm::vec3(0.f, 0.f, 1.f));
	blueLight->SetType(Light::Type::DIRECTIONAL);
	blueLight->GetPosition() = glm::vec3(1.f, 1.f, 1.f);
	blueLight->SetAttenuation(0.f);

	Ptr<Light> greenLight = Light::Create();
	greenLight->SetColor(glm::vec3(0.f, 1.f, 0.f));
	greenLight->SetType(Light::Type::POINT);
	greenLight->GetPosition() = camera->GetPosition();
	greenLight->SetAttenuation(0.05f);

	Scene::Instance()->AddEntity(model.UpCast<Entity>());
	Scene::Instance()->AddEntity(camera.UpCast<Entity>());
	Scene::Instance()->AddEntity(blueLight.UpCast<Entity>());
	Scene::Instance()->AddEntity(greenLight.UpCast<Entity>());

	Scene::Instance()->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));

	while ( !Screen::Instance()->ShouldClose() && !Screen::Instance()->IsKeyPressed(GLFW_KEY_ESCAPE) ) {
		Scene::Instance()->Update(Screen::Instance()->GetElapsed());
		Scene::Instance()->Render();

		camera->GetTarget() = model->GetPosition();
		model->GetRotation() *= glm::quat(glm::radians(glm::vec3(0,
			32 * Screen::Instance()->GetElapsed(), 0)));

		Screen::Instance()->Refresh();
		Screen::Instance()->SetTitle(Renderer::Instance()->GetProgramError());
	}
#endif
	ResourceManager::Instance()->FreeResources();

	return 0;
}
