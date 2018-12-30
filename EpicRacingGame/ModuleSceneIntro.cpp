#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"
#include "ModuleAudio.h"

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
	
	//Insert PlayMusic();
	//Insert 
	App->audio->LoadFx("fx/coin_fx.wav");
	

	minutes = 1;
	seconds = 60.0f;
	
	App->camera->Move(vec3(0.0f, 25.0f, -15.0f));
	App->camera->LookAt(vec3(App->player->initial_position.x, App->player->initial_position.y, App->player->initial_position.z));
	
	LoadCheckPoints();
	LoadCircuit();
	LoadCoins();
	LoadObstacles();
	App->audio->PlayMusic("music/Naruto_op_16.ogg");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (p2List_item<PhysBody3D*>* iterator = b_coins.getFirst(); iterator != nullptr; iterator = iterator->next)
			coins_to_delete.add(iterator);
	
	p2List_item<p2List_item<PhysBody3D*>*>* iterator = coins_to_delete.getFirst();
	
	while (iterator != nullptr)
	{
		p2List_item<p2List_item<PhysBody3D*>*>* next = iterator->next;
		b_coins.del(iterator->data);
		coins_to_delete.del(iterator);
		iterator = next;
	}
	coins_to_delete.clear();

	return true;
}
update_status ModuleSceneIntro::PreUpdate(float dt) {

if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && indexMusic != 3)
		indexMusic += 1;
	else if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN && indexMusic == 3)
		indexMusic = 0;

return UPDATE_CONTINUE;
}


// Update
update_status ModuleSceneIntro::Update(float dt)
{
	
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	
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

	
	check_points[1] = CreateCheckPoints(RESPAWN_DIM, vec3(vec3_zero.x, vec3_zero.y + 5, vec3_zero.z + ROAD_DIM.z * 4), true); // DEATH SENSOR
	check_points[1]->collision_listeners.add(this);

	check_points[0] = CreateCheckPoints(SENSOR_DIM, vec3(vec3_zero.x, vec3_zero.y + 23, vec3_zero.z + ROAD_DIM.z * 9), true);
	check_points[0]->collision_listeners.add(this);

	check_points[2] = CreateCheckPoints(SENSOR_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 23, vec3_zero.z + ROAD_DIM.z * 12), true);
	check_points[2]->collision_listeners.add(this);

	check_points[3] = CreateCheckPoints(SENSOR_DIM2, vec3(vec3_zero.x + ROAD_DIM.x * -2, vec3_zero.y + 23, vec3_zero.z + ROAD_DIM.z * 18), true);
	check_points[3]->collision_listeners.add(this);

	check_points[4] = CreateCheckPoints(SENSOR_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 23, vec3_zero.z + ROAD_DIM.z * 9), true);
	check_points[4]->collision_listeners.add(this);

	check_points[5] = CreateCheckPoints(SENSOR_DIM2, vec3(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 33, vec3_zero.z + ROAD_DIM.z * -4), true);
	check_points[5]->collision_listeners.add(this);

	deathLine = CreateCheckPoints(SENSOR_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 33, vec3_zero.z + ROAD_DIM.z + 6), true);
	deathLine->collision_listeners.add(this);
	
}

void ModuleSceneIntro::LoadCoins() {

	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 22, vec3_zero.x + ROAD_DIM.z * 7), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 22, vec3_zero.x + ROAD_DIM.z * 10), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 22, vec3_zero.x + ROAD_DIM.z * 26), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * -13, vec3_zero.y + 22, vec3_zero.x + ROAD_DIM.z * 18), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 22, vec3_zero.x + ROAD_DIM.z * 18), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * -14, vec3_zero.y + 22, vec3_zero.x + ROAD_DIM.z * 13), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 22, vec3_zero.x + ROAD_DIM.z * 7), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 33, vec3_zero.x + ROAD_DIM.z * -4), Yellow, true);
	CreateCoin(1.0f, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 33, vec3_zero.x + ROAD_DIM.z * -4), Yellow, true);

	//coins[0]->SetAsSensor(true);
	//coins[0]->collision_listeners.add(this);

}


void ModuleSceneIntro::LoadCircuit() {

	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 20, vec3_zero.x), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z *2), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 1.6f + 20, vec3_zero.x + ROAD_DIM.z * 3 - 0.25f), White, -20.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 1.6f + 20, vec3_zero.x + ROAD_DIM.z * 7 - 0.25f), White, 20.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 8), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 9), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 2, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 3, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 5, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 6, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 7, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 11), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 12), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 14), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 1.6f + 20, vec3_zero.x + ROAD_DIM.z * 15), White, -20.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 1.6f + 20, vec3_zero.x + ROAD_DIM.z * 18), White, 20.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 19), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 20), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 21), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 22), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 24), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 25), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 26), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 7, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 26), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 6, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 26), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 5, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 26), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 26), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 25), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 24), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 22), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 21), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 20), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 19), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 3, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 2, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 1, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -1, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -2, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -5, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -6, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -7, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -10, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -12, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -13, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -14, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 18), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -14, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 17), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -14, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 16), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -14, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 14), White); 
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -14, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -13, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -12, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -8, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -7, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -6, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -5, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 13), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 12), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 11), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 10), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 9), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 8), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 7), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -5, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 7), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -6, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 7), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 7), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -9, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 7), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -10, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 7), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 7), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 6), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 20, vec3_zero.x + ROAD_DIM.z * 5), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 1.6f + 20, vec3_zero.x + ROAD_DIM.z * 4), White, 15.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 1.3f + 23, vec3_zero.x + ROAD_DIM.z * 3), White, 15.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 1.0f + 26, vec3_zero.x + ROAD_DIM.z * 2), White, 15.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 0.7f + 29, vec3_zero.x + ROAD_DIM.z * 1), White, 15.0f, axis_x);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z*0), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -1), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -2), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -3), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -11, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -10, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -9, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -8, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -7, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -2, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * -1, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 1, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 2, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 3, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -4), White); // Coin
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -3), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -2), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * -1), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * 1), White);
	CreateCube(ROAD_DIM, vec3(vec3_zero.x + ROAD_DIM.x * 4, vec3_zero.y + 31, vec3_zero.x + ROAD_DIM.z * 2), Red); // FINISH
}

void ModuleSceneIntro::LoadObstacles()
{
	CreateCube(OBS_DIM_WIDE, vec3(vec3_zero.x + ROAD_DIM.x, vec3_zero.y + 22.75f, vec3_zero.x + ROAD_DIM.z * 9.7f), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * 8, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 12), White);
	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * 6, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 26), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_WIDE, vec3(vec3_zero.x + ROAD_DIM.x * 3.7, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 25), White);
	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 18), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 18), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_WIDE, vec3(vec3_zero.x + ROAD_DIM.x * -6, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 18.3f), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * -6, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 17.6f), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_WIDE, vec3(vec3_zero.x + ROAD_DIM.x * -7, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 12.7), White, 90.0f, axis_y);

	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 11), White);
	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * -4, vec3_zero.y + 22.75, vec3_zero.x + ROAD_DIM.z * 10), White);

	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 33.75, vec3_zero.x + ROAD_DIM.z * -4), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_THIN, vec3(vec3_zero.x + ROAD_DIM.x * -2, vec3_zero.y + 33.75, vec3_zero.x + ROAD_DIM.z * -4), White, 90.0f, axis_y);
	CreateCube(OBS_DIM_WIDE, vec3(vec3_zero.x + ROAD_DIM.x * 4.3, vec3_zero.y + 33.75, vec3_zero.x + ROAD_DIM.z * -2), White);


	//------------------------------ CONSTRAINTS----------------------------------//

	//Rotation axis
	axis1.size = (AXIS_DIM);
	axis1.SetPos(vec3_zero.x + ROAD_DIM.x * 3, vec3_zero.y + 25, vec3_zero.x + ROAD_DIM.z * 2);
	axis1_pb = App->physics->AddBody(axis1, 0.0f);
	
	axis2.size = (AXIS_DIM);
	axis2.SetPos(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 25, vec3_zero.x + ROAD_DIM.z * 2);
	axis2_pb = App->physics->AddBody(axis2, 0.0f);

	axis3.size = (AXIS_DIM);
	axis3.SetPos(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 25, vec3_zero.x + ROAD_DIM.z * 6);
	axis3_pb = App->physics->AddBody(axis3, 0.0f);

	//Rotator obstacle
	obstacle1.size = (OBS_ROT_DIM);
	obstacle1.SetPos(vec3_zero.x + ROAD_DIM.x * 3, vec3_zero.y + 25, vec3_zero.x + ROAD_DIM.z * 2);
	obstacle1.color = Green;
	obstacle1_pb = App->physics->AddBody(obstacle1, 30.0f);
	
	obstacle2.size = (OBS_ROT_DIM);
	obstacle2.SetPos(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 25, vec3_zero.x + ROAD_DIM.z * 2);
	obstacle2.color = Green;
	obstacle2_pb = App->physics->AddBody(obstacle2, 30.0f);

	obstacle3.size = (OBS_ROT_DIM);
	obstacle3.SetPos(vec3_zero.x + ROAD_DIM.x * -3, vec3_zero.y + 25, vec3_zero.x + ROAD_DIM.z * 6);
	obstacle3.color = Green;
	obstacle3_pb = App->physics->AddBody(obstacle3, 30.0f);
	
	//Constraint
	App->physics->AddConstraintHinge(*axis1_pb, *obstacle1_pb, vec3(0, 0, 0), vec3(0, 0, 12), vec3(0, 1, 0), vec3(0, 1, 0), true,true);

	App->physics->AddConstraintHinge(*axis2_pb, *obstacle2_pb, vec3(0, 0, 0), vec3(0, 0, 12), vec3(0, 1, 0), vec3(0, 1, 0), true, true);

	App->physics->AddConstraintHinge(*axis3_pb, *obstacle3_pb, vec3(0, 0, 0), vec3(0, 0, 12), vec3(0, 1, 0), vec3(0, 1, 0), true, true);


}

void ModuleSceneIntro::PrintCircuit() {
	

	//------- Roads ---------//
	p2List_item<Cube>* cubes_item = cubes.getFirst();

	while (cubes_item != nullptr) {
		cubes_item->data.Render();

		cubes_item = cubes_item->next;
	}


	//------- Coins ---------//
	Sphere coin(1.0);
	coin.color = Yellow;

	for (p2List_item<PhysBody3D*>* iterator = b_coins.getFirst(); iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->GetTransform(&coin.transform);
		coin.Render();
	}

	//------- Constraints ---------//
	obstacle1_pb->GetTransform(&obstacle1.transform);
	obstacle1.Render();

	obstacle2_pb->GetTransform(&obstacle2.transform);
	obstacle2.Render();
	
	obstacle3_pb->GetTransform(&obstacle3.transform);
	obstacle3.Render();
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
		App->audio->PlayMusic("music/Naruto_op_16.ogg");
		song = "Naruto";
		song1_active = true;
		song4_active = false;
	}
	else if (indexMusic == 1 && !song2_active)
	{
		App->audio->PlayMusic("music/DBZ_op_1.ogg");
		song = "Dragon Ball";
		song1_active = false;
		song2_active = true;
	}
	else if (indexMusic == 2 && !song3_active)
	{
		App->audio->PlayMusic("music/One_Piece_op_1.ogg");
		song = "One Piece";
		song2_active = false;
		song3_active = true;
	}
	else if (indexMusic == 3 && !song4_active)
	{
		App->audio->PlayMusic("music/Willyrex_Paradise.ogg");
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
		lose = true;
	}
}


void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

	if (body1 == check_points[0] && body2 == (PhysBody3D*)App->player->vehicle)  // FIRST CHECKPOINT
		current_checkpoint = 1;
		

	if (body1 == check_points[2] && body2 == (PhysBody3D*)App->player->vehicle)  // SECOND CHECKPOINT
		current_checkpoint = 2;
		

	if (body1 == check_points[3] && body2 == (PhysBody3D*)App->player->vehicle)  // THIRD CHECKPOINT
		current_checkpoint = 3;
		
	
	if (body1 == check_points[4] && body2 == (PhysBody3D*)App->player->vehicle)  // FOURTH CHECKPOINT
		current_checkpoint = 4;


	if (body1 == check_points[5] && body2 == (PhysBody3D*)App->player->vehicle)  // FIFTH CHECKPOINT
		current_checkpoint = 5;

	
	if (body1 == check_points[1] && body2 == (PhysBody3D*)App->player->vehicle) { // DEATH SENSOR
		
		App->player->vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		App->player->vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		IdentityMatrix = I_MAT;

		switch (App->scene_intro->current_checkpoint) {

		case 0:
			App->player->vehicle->SetTransform(IdentityMatrix.M);
			App->player->vehicle->SetPos(App->player->initial_position.x, App->player->initial_position.y, App->player->initial_position.z);
			break;
		case 1:
			App->player->vehicle->SetTransform(IdentityMatrix.M);
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x, App->scene_intro->vec3_zero.y + 20, App->scene_intro->vec3_zero.z + ROAD_DIM.z * 9);
			break;
		case 2:
			App->player->vehicle->SetTransform(IdentityMatrix.M);
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * 8, App->scene_intro->vec3_zero.y + 20, App->scene_intro->vec3_zero.z + ROAD_DIM.z * 12);
			break;
		case 3:
			App->player->vehicle->SetTransform(IdentityMatrix.rotate(-90.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(90.0f, vec3(0.0f, 1.0f, 0.0f));
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * -2, App->scene_intro->vec3_zero.y + 20, App->scene_intro->vec3_zero.z + ROAD_DIM.z * 18);
			break;
		case 4:
			App->player->vehicle->SetTransform(IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f));
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * -4, App->scene_intro->vec3_zero.y + 20, App->scene_intro->vec3_zero.z + ROAD_DIM.z * 9);
			break;
		case 5:
			App->player->vehicle->SetTransform(IdentityMatrix.rotate(90.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(-90.0f, vec3(0.0f, 1.0f, 0.0f));
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * -3, App->scene_intro->vec3_zero.y + 32, App->scene_intro->vec3_zero.z + ROAD_DIM.z * -4);
			break;
		}

	}

	if (body1 == deathLine && body2 == (PhysBody3D*)App->player->vehicle) { // FINAL LINE
		if (current_checkpoint == 5)
			win = true;
	}

	if (body2 == (PhysBody3D*)App->player->vehicle) {

		for (p2List_item<PhysBody3D*>* iterator = b_coins.getFirst(); iterator != nullptr; iterator = iterator->next)
		{
			if (body1 == iterator->data) {
				App->audio->PlayFx(1);
				App->player->vehicle->Push(0.0f, 0.0f, 200.0f);
				current_coins += 1;
				coins_to_delete.add(iterator);

			}
		}

	}

	/*if (body1 == coins[0] && body2 == (PhysBody3D*)App->player->vehicle) {            // First method to delete coin phys bodies
				App->physics->DeleteBody(coins[0]);
		}*/

}
