#pragma once
#ifndef GHOST_H
#define GHOST_H

#include "Lib.h"
#include "compositeGeometry.h"
#include "shader.h"

class GhostGenerator {
public:
	GhostGenerator(const Shader& shader);
	void initTransforms();
	void generateGhost();
	void simulateGhosts();
	void randomSpawn();

	vector<CompositeGeometry*> ghosts;
	vector<vec3> scales;
	vector<vec3> translates;
	vector<float> angoli;
	vector<vec3> t;
	int maxG = 5;
	int count;
	Shader shader;
};

#endif // !GHOST_H
