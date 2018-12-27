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
	minutes = 1;
	seconds = 60.0f;
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
    
	LoadCheckPoints();
	LoadCircuit();
	LoadCoins();

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
	
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && indexMusic != 3)
		indexMusic += 1;
	else if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && indexMusic == 3)
		indexMusic = 0;

	Radio();
	Timer(dt);
	PrintCircuit();

	return UPDATE_CONTINUE;
}


update_status ModuleSceneIntro::PostUpdate(float dt)
{
	p2List_item<p2List_item<PhysBody3D*>*>* iterator = coins_to_delete.getFirst();
	while (iterator != nullptr)
	{
		p2List_item<p2List_item<PhysBody3D*>*>* next = iterator->next;
		b_coins.del(iterator->data);
		coins_to_delete.del(iterator);
		iterator = next;
	}
	coins_to_delete.clear();

	return UPDATE_CONTINUE;
}


void ModuleSceneIntro::LoadCheckPoints() {

	check_points[0] = CreateCheckPoints(SENSOR_DIM, vec3(vec3_zero.x, vec3_zero.y, vec3_zero.z + ROAD_DIM.z*6), true);
	check_points[0]->collision_listeners.add(this);
	
}

void ModuleSceneIntro::LoadCoins() {

	CreateCoin(1.0f, vec3(vec3_zero.x, vec3_zero.y + 1, vec3_zero.z + ROAD_DIM.z * 6), Yellow, true);
	//coins[0]->SetAsSensor(true);
	//coins[0]->collision_listeners.add(this);

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

	Sphere coin(1.0);
	coin.color = Yellow;

	for (p2List_item<PhysBody3D*>* iterator = b_coins.getFirst(); iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->GetTransform(&coin.transform);
		coin.Render();
	}

	/*p2List_item<Sphere>* sphere_item = s_coins.getFirst();            // previous method to render sphere coins (primitive)
	
	while (sphere_item != nullptr) {
		if(sphere_item->data.to_delete == false)
		       sphere_item->data.Render();

		sphere_item = sphere_item->next;
	}*/
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

PhysBody3D* ModuleSceneIntro::CreateCoin(float radius, vec3 pos, Color color, bool sensor, float angle, vec3 u, float mass)
{
	Sphere s(radius);
	s.SetPos(pos.x, pos.y, pos.z);
	s.color = color;

	if (angle != 0)
		s.SetRotation(angle, vec3(u.x, u.y, u.z));


	PhysBody3D* coin = App->physics->AddBody(s, 0.0f);
	coin->SetAsSensor(sensor);
	coin->collision_listeners.add(this);

	b_coins.add(coin);
	//s_coins.add(s);
	return coin;
}

PhysBody3D* ModuleSceneIntro::CreateCheckPoints(vec3 dim, vec3 pos, bool sensor) {

	Cube c(dim.x, dim.y, dim.z);
	c.SetPos(pos.x, pos.y, pos.z);

	PhysBody3D* tmp;
	tmp = App->physics->AddBody(c, 0.0f);
	tmp->SetAsSensor(sensor);
	

	return tmp;
}
void ModuleSceneIntro::Radio()
{
	if (indexMusic == 0 && !song1_active)
	{
		song = "Naruto";
		song1_active = true;
		song4_active = false;
	}
	else if (indexMusic == 1 && !song2_active)
	{
		song = "Dragon Ball";
		song1_active = false;
		song2_active = true;
	}
	else if (indexMusic == 2 && !song3_active)
	{
		
		song = "One Piece";
		song2_active = false;
		song3_active = true;
	}
	else if (indexMusic == 3 && !song4_active)
	{
		song = "Willyrex";
		song3_active = false;
		song4_active = true;
	}
}

void ModuleSceneIntro::Timer(float dt)
{

	if (minutes >= 0 && finishedTime == false) {
			seconds -= 1.0f * dt;
			
			if (seconds <= 0)
			{
				seconds = 60.0f;
				minutes -= 1;

				if (minutes == -1) {
					finishedTime = true;
				}
			}
	}
	
	else if (finishedTime == true) {
		seconds = 0.0f;
		minutes = 0;
	}
}


void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

	if (body1 == check_points[0] && body2 == (PhysBody3D*)App->player->vehicle) {
		current_checkpoint = 1;
		LOG("LA PARASTE DE PECHO COLORAAAAAAAAO!!!!!!!!! &d", current_checkpoint);
	}


	if (body2 == (PhysBody3D*)App->player->vehicle) {

		for (p2List_item<PhysBody3D*>* iterator = b_coins.getFirst(); iterator != nullptr; iterator = iterator->next)
		{
			if (body1 == iterator->data) {
				current_coins += 1;
				coins_to_delete.add(iterator);

			}
		}

	}

	/*if (body1 == coins[0] && body2 == (PhysBody3D*)App->player->vehicle) {            // First method to delete coin phys bodies
				App->physics->DeleteBody(coins[0]);
		}*/

}
