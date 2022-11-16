#pragma once
#ifndef LIGHT_H
#define LIGH_H
#include "Lib.h"
#include "Obstacles.h"
#include<algorithm>

class LightGenerator {
public:
	LightGenerator(const Shader &shader, vector<Obstacle*> obs, float width, float height);
	void calculateLight(vec2 c, float radius);
	void initVao();
	void renderLight(mat4 model);

	vector<tuple<vec3, vec3>> edges;
	vector<tuple<float, float, float>> rays;
	vector<vec3> vertices;
	vector<vec4> colors;
	GLuint VAO;
	Shader shader;
	mat4 model;
};

#endif // !LIGHT_H

