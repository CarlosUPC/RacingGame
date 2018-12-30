#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2SString.h"

#define MAX_SNAKE 2
#define ROAD_DIM vec3(10,0.5,10)
#define OBS_DIM_WIDE vec3(4, 5, 10)
#define OBS_DIM_THIN vec3(2, 5, 10)
#define AXIS_DIM vec3(10, 10, 1)
#define OBS_ROT_DIM vec3(3, 3, 3)
#define SENSOR_DIM vec3(10,2,2)
#define SENSOR_DIM2 vec3(2,2,10)
#define RESPAWN_DIM vec3(500,5,500)

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	
	void LoadCircuit();
	void PrintCircuit();
	void LoadObstacles();
	void CreateCube(vec3 dimension = { 1.0f,1.0f,1.0f }, vec3 position = { 0.0f, 0.0f, 0.0f }, Color color = White, float angle = 0.0f, vec3 u = (0.0f, 0.0f, 0.0f), float mass = 0.0f);
	
	void LoadCoins();
	PhysBody3D* CreateCoin(float radius = 0.0f, vec3 position = { 0.0f, 0.0f, 0.0f }, Color color = White, bool sensor = false,float angle = 0.0f, vec3 u = (0.0f, 0.0f, 0.0f), float mass = 0.0f);
	
	void LoadCheckPoints();
	PhysBody3D* CreateCheckPoints(vec3 dimension = { 1.0f,1.0f,1.0f }, vec3 position = { 0.0f, 0.0f, 0.0f }, bool sensor = false);
	
	void Timer(float dt);
	void Radio();

public:
	
	// Vehicle
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	// Primitives
	p2List<Cube> cubes;
	p2List<Sphere> s_coins;
	
	// Coins
	p2List<p2List_item<PhysBody3D*>*> coins_to_delete;
	p2List<PhysBody3D*> b_coins;

	//Circuit Paramenters
	vec3 vec3_zero = { 0.0f,0.0f,0.0f };
	vec3 axis_x = { 1.0f,0.0f,0.0f }, axis_y = { 0.0f,1.0f,0.0f }, axis_z = { 0.0f,0.0f,1.0f };

	//Constraints
	Cube axis1, obstacle1, axis2, obstacle2, axis3, obstacle3;
	PhysBody3D *axis1_pb = nullptr, *obstacle1_pb = nullptr, *axis2_pb = nullptr, *obstacle2_pb = nullptr, *axis3_pb = nullptr, *obstacle3_pb = nullptr;
	
	//Sensors
	PhysBody3D* check_points[9] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr , nullptr };
	PhysBody3D* coins[9] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr , nullptr };
	uint current_checkpoint = 5;
	uint current_coins = 0;

	//Timer
	int minutes = 0;
	float seconds = 0;
	bool finishedTime = false;

	// Music
	uint indexMusic = 0;
	bool song1_active = true, song2_active = false, song3_active = false, song4_active = false;
	p2SString song = "Pokemon";

	//Win - Lose Condition
	PhysBody3D* deathLine = nullptr;
    bool win = false;
	bool lose = false;
};
