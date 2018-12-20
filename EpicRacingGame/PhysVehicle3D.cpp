#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	// CAR'S CHASSIS

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	// CAR'S CABIN

	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);

	btVector3 offset_cabin(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offset_cabin = offset_cabin.rotate(q.getAxis(), q.getAngle());

	cabin.transform.M[12] += offset_cabin.getX();
	cabin.transform.M[13] += offset_cabin.getY();
	cabin.transform.M[14] += offset_cabin.getZ();

	cabin.color = Red;

	// CAR'S BACK LEFT LIGHT

	Cube backleft_light_cube(info.back_left_light_size.x, info.back_left_light_size.y, info.back_left_light_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&backleft_light_cube.transform);

	btVector3 offset_backleft_light_cube(info.back_left_light_offset.x, info.back_left_light_offset.y, info.back_left_light_offset.z);
	offset_backleft_light_cube = offset_backleft_light_cube.rotate(q.getAxis(), q.getAngle());

	backleft_light_cube.transform.M[12] += offset_backleft_light_cube.getX();
	backleft_light_cube.transform.M[13] += offset_backleft_light_cube.getY();
	backleft_light_cube.transform.M[14] += offset_backleft_light_cube.getZ();

	backleft_light_cube.color = Red;

	// CAR'S BACK RIGHT LIGHT

	Cube backright_light_cube(info.back_right_light_size.x, info.back_right_light_size.y, info.back_right_light_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&backright_light_cube.transform);

	btVector3 offset_backright_light_cube(info.back_right_light_offset.x, info.back_right_light_offset.y, info.back_right_light_offset.z);
	offset_backright_light_cube = offset_backright_light_cube.rotate(q.getAxis(), q.getAngle());

	backright_light_cube.transform.M[12] += offset_backright_light_cube.getX();
	backright_light_cube.transform.M[13] += offset_backright_light_cube.getY();
	backright_light_cube.transform.M[14] += offset_backright_light_cube.getZ();

	backright_light_cube.color = Red;

	chassis.Render();
	cabin.Render();
	backleft_light_cube.Render();
	backright_light_cube.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}