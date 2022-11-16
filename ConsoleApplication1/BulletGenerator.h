#pragma once
#ifndef BULLET_H
#define BULLET_H

#include "Lib.h"
#include "geometricFigures.h"
#include "shader.h"

class BulletGenerator {
public:
	BulletGenerator(const Shader& shader, vec2 c, vec2 radius, int maxBullets);
	void generateBullet();
	void simulateBullets(mat4 modelGun);
	
	int maxB;
	int count;
	int maxY = 300;
	mat4 model;
	float velY = 5.0f;
	vec3 translation;
	Shader s;
	vec2 c;
	vec2 radius;
	vector<GeometricFigure*> bullets;
};

#endif // !BULLET_H

