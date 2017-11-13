#pragma once

class GameObject;

struct RayHit {

	GameObject* object = nullptr;
	float hit_distance = 0;

};
