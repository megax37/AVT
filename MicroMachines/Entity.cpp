#include <stdio.h>
#include <stdlib.h>

#include "Entity.h"

Entity::Entity() {
	current_position[0] = 0.0f;
	current_position[1] = 0.0f;
	current_position[2] = 0.0f;
	current_rotation[0] = 0.0f;
	current_rotation[1] = 0.0f;
	current_rotation[2] = 0.0f;
	current_scale[0] = 0.0f;
	current_scale[1] = 0.0f;
	current_scale[2] = 0.0f;
	initial_velocity[0] = 0.0f;
	initial_velocity[1] = 0.0f;
	initial_velocity[2] = 0.0f;
}

Entity::Entity(int meshCapacity) {
	mesh = (MyMesh*)malloc(meshCapacity * sizeof(*mesh));
	meshLength = meshCapacity;
	current_position[0] = 0.0f;
	current_position[1] = 0.0f;
	current_position[2] = 0.0f;
	current_rotation[0] = 0.0f;
	current_rotation[1] = 0.0f;
	current_rotation[2] = 0.0f;
	current_scale[0] = 0.0f;
	current_scale[1] = 0.0f;
	current_scale[2] = 0.0f;
	initial_velocity[0] = 0.0f;
	initial_velocity[1] = 0.0f;
	initial_velocity[2] = 0.0f;
}