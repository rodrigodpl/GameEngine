#pragma once

#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet\include\LinearMath\btVector3.h"

struct PhysBody3D;

struct VehicleInfo
{
	~VehicleInfo();
	
	vec3 chassis_size;
	vec3 chassis_offset;
	float mass;
	float linear_dampening; // defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, const VehicleInfo& info);
	~PhysVehicle3D();

	void Update(bool render = true);
	void applyForwardImpulse(float force);
	void applyLateralImpulse(float force);
	void applyUpwardImpulse(float force);
	void doBarrelRoll(bool right);
	void Turn(float degrees);
	float GetKmh() const;
public:

	VehicleInfo info;
	btRigidBody* body;

private:
	float currentSpeedKmHour;
	btVector3 forward;
	btVector3 right;
	btVector3 up;
	btScalar lateral_angle;
	bool barrel_left;
	bool barrel_right;

};