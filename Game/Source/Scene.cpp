#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	iPoint t1 = { 1 * 100, 1 * 100 };
	iPoint t2 = { 3 * 100, 1 * 100 };
	iPoint t3 = { 2 * 100, 3 * 100 };
	iPoint t4 = { 5 * 100, 1 * 100 };
	iPoint t5 = { 4 * 100, 3 * 100 };

	Triangle tri = { t1,t2,t3 };
	Triangle tri2 = { t2,t4,t5 };

	random.AddTriangle(tri);
	random.AddTriangle(tri2);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += 1;

	SDL_Rect rect = { 300,100,50,50 };
	app->render->DrawRectangle(rect, 255, 255, 255, 255, true, true);

	random.Update();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
