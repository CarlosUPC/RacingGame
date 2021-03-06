#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleAudio.h"



ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 4);
	car.chassis_offset.Set(0, 1, 0);

	car.cabin_size.Set(1.98, 1.25, 2);
	car.cabin_offset.Set(-0.001, 2, -0.98);

	car.back_left_light_size.Set(0.25, 0.5, 0.5);
	car.back_left_light_offset.Set(0.75, 1, -1.8);

	car.back_right_light_size.Set(0.25, 0.5, 0.5);
	car.back_right_light_offset.Set(-0.75, 1, -1.8);


	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	position = initial_position;
	vehicle->SetPos(position.x, position.y, position.z);
	
	//Insert LoadFx();
	 App->audio->LoadFx("fx/game_over_fx.wav");
	 App->audio->LoadFx("fx/win_fx.wav");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = 1.5f *MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn += 2* TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= 2*TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		//brake = BRAKE_POWER;
		acceleration = -2.5f *MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !is_jumping)
		is_jumping = true;

	if (is_jumping)
	{
		jumping_time += 0.1f;

		if (jumping_time <= 5.0f)
			vehicle->Push(0, 200, 0);
		
		else
			vehicle->Push(0, -130, 0);

		if (jumping_time >= 12.0f)
		{
			is_jumping = false;
			jumping_time = 0.0f;
		}

	}
	//--------------------CAMERA DEBUG ON----------------------//
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		App->camera->cam_on = !App->camera->cam_on;
	}

	//--------------------WIN CONDITION----------------------//
	if (App->scene_intro->win && !fx_done || App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		over_cam = true;
		App->audio->PlayFx(3);
		App->scene_intro->win = true;
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		IdentityMatrix = I_MAT;
		vehicle->SetTransform(IdentityMatrix.M);
		vehicle->SetPos(initial_position.x, initial_position.y - 20, initial_position.z);
		fx_done = true;
	}

	//--------------------LOSE CONDITION----------------------//
	if (App->scene_intro->lose && !fx_done || App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		over_cam = true;
		App->audio->PlayFx(2);
		App->scene_intro->lose = true;
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		IdentityMatrix = I_MAT;
		vehicle->SetTransform(IdentityMatrix.M);
		vehicle->SetPos(initial_position.x, initial_position.y - 20, initial_position.z);
		fx_done = true;
	}




	//----------------------RESTART-----------------------//
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		
		over_cam = false;

		App->scene_intro->lose = false;
		App->scene_intro->win = false;

		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		
		IdentityMatrix = I_MAT;
		vehicle->SetTransform(IdentityMatrix.M);
		vehicle->SetPos(initial_position.x, initial_position.y, initial_position.z);
		
		App->scene_intro->finishedTime = false;
		App->scene_intro->CleanUp();
		App->scene_intro->LoadCoins();
		App->scene_intro->current_coins = 0;
		App->scene_intro->current_checkpoint = 0;
		App->scene_intro->minutes = 2;
		App->scene_intro->seconds = 60.0f;
	}

	//-------------SPAWN TO CURRENT CHECKPOINT--------------//
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {

		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));

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
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * 8, App->scene_intro->vec3_zero.y + 22, App->scene_intro->vec3_zero.z + ROAD_DIM.z * 14);
			break;
		case 3:
			App->player->vehicle->SetTransform(IdentityMatrix.rotate(-90.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(90.0f, vec3(0.0f, 1.0f, 0.0f));
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * -1, App->scene_intro->vec3_zero.y + 22, App->scene_intro->vec3_zero.z + ROAD_DIM.z * 18);
			break;
		case 4:
			App->player->vehicle->SetTransform(IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(180.0f, vec3(0.0f, 1.0f, 0.0f));
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * -4, App->scene_intro->vec3_zero.y + 22, App->scene_intro->vec3_zero.z + ROAD_DIM.z * 8);
			break;
		case 5:
			App->player->vehicle->SetTransform(IdentityMatrix.rotate(90.0f, vec3(0.0f, 1.0f, 0.0f)).M);
			IdentityMatrix.rotate(-90.0f, vec3(0.0f, 1.0f, 0.0f));
			App->player->vehicle->SetPos(App->scene_intro->vec3_zero.x + ROAD_DIM.x * -8, App->scene_intro->vec3_zero.y + 32, App->scene_intro->vec3_zero.z + ROAD_DIM.z * -4);
			break;
		}



	}


	
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();
	//vehicle->GetPos(position.x, position.y, position.z);
	char title[80];
	
	if (!App->scene_intro->win && !App->scene_intro->lose)
		sprintf_s(title, "%.1f Km/h,CheckPoint: %i, Time: %i:%.1f, Coins: %i, Radio: %s",vehicle->GetKmh(), App->scene_intro->current_checkpoint, App->scene_intro->minutes, App->scene_intro->seconds, App->scene_intro->current_coins, App->scene_intro->song.GetString());
	else if (App->scene_intro->win)
		sprintf_s(title, "CONGRATS! YOU WON!!! PRESS ""R"" TO START AGAIN");
	else if (App->scene_intro->lose)
		sprintf_s(title, "WHAT A PITY! YOU LOST!!! PRESS ""R"" TO START AGAIN");
	
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}





