#ifndef PLAYER_H
#define PLAYER_H


#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

class Player {
public:

	Camera camera;
	float foot;

	Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f)) {
		camera.setPlayerPos(position);
		foot = position.y - 1.5;

	}





};



#endif
#pragma once
