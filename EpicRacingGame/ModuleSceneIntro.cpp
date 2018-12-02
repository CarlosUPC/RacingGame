#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	LoadCircuit();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	PrintCircuit();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::LoadCircuit() {

	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y, vec3_zero.x), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y, vec3_zero.x + ROAD_DIM.z), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y, vec3_zero.x + ROAD_DIM.z *2), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 0.8f, vec3_zero.x + ROAD_DIM.z * 3 - 0.25f), White, -20.0f, axis_x);
}

void ModuleSceneIntro::PrintCircuit() {
	
	p2List_item<Cube>* cubes_item = cubes.getFirst();

	while (cubes_item != nullptr) {
		cubes_item->data.Render();

		cubes_item = cubes_item->next;
	}
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}


void ModuleSceneIntro::CreateCube(vec3 dim, vec3 pos, Color color, float angle, vec3 u, float mass)
{
	Cube c(dim.x, dim.y, dim.z);
	c.SetPos(pos.x, pos.y, pos.z);
	c.color = color;

	if (angle != 0)
		c.SetRotation(angle, vec3(u.x, u.y, u.z));

	
	App->physics->AddBody(c, mass);

	cubes.add(c);

	//return c;
}

