#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Lib.h"
#include "shader.h"
#include<tuple>

class Obstacle {
public:
	Obstacle(const Shader &shader, int type, vec3 s, vec3 t, float th);
	void initVAO(int type);
	void drawObstacle();
	void UpdateBBox();
	Shader shader;
	float vertices[18];
	mat4 model;
	GLuint VAO;
	vec3 cornerBottomRight;
	vec3 cornerUpperLeft;
	vec3 cornerUpperRight;
	vec3 cornerBottomLeft;
	float velocitX = 0.05f;
	float direction = 1.0f;
	vector<tuple<vec3, vec3>> edges;
};

#endif