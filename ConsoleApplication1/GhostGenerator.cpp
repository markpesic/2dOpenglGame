#include "GhostGenerator.h"
#include <stdio.h>     
#include <stdlib.h>     
#include <time.h>

GhostGenerator::GhostGenerator(const Shader& shader) {
	this->count = 0;
    this->shader = shader;
	this->initTransforms();
	srand(time(NULL));
}

void GhostGenerator::randomSpawn() {
    float px = ((float)rand() / RAND_MAX);
    float py = ((float)rand() / RAND_MAX);
    this->t.push_back(vec3(px * 800.0f, py * 200.0f, 0.0));
}

void GhostGenerator::initTransforms() {

	for (int i = 0; i < 5;i++) {
		generateGhost();
	}
	for (int i = 0; i < ghosts.size(); i++) {
        randomSpawn();
	}
}

void GhostGenerator::generateGhost() {
    vector<GeometricFigure*> fantasmaComps;
    vector<vec3> cps;
    cps.push_back(vec3(-1.0, -1.0, 0.0));
    cps.push_back(vec3(0.0, -1.0, 0.0));
    cps.push_back(vec3(1.0, -1.0, 0.0));
    cps.push_back(vec3(0.90, 0.0, 0.0));
    cps.push_back(vec3(0.40, 0.90, 0.0));
    cps.push_back(vec3(0.0, 1.0, 0.0));
    cps.push_back(vec3(-0.40, 0.90, 0.0));
    cps.push_back(vec3(-0.90, 0.0, 0.0));
    cps.push_back(vec3(-1.0, -1.0, 0.0));
    cps.push_back(vec3(0.0, -1.0, 0.0));
    // set render-specific controls
    GeometricFigure* FanstasmaBusto = new GeometricFigure(this->shader, cps, vec4(1.0, 1.0, 1.0, 1.0));
    fantasmaComps.push_back(FanstasmaBusto);
    scales.push_back(vec3(50.0, 50.0, 0.0));
    translates.push_back(vec3(500.0, 500.0, 0.0));
    angoli.push_back(0.0);

    cps.clear();
    cps.push_back(vec3(0.0, 0.0, 0.0));
    cps.push_back(vec3(0.1, 0.1, 0.0));
    cps.push_back(vec3(-0.2, 0.3, 0.0));
    GeometricFigure* FanstasmaEyeDx = new GeometricFigure(this->shader, cps, vec4(1.0, 0.0, 0.0, 1.0));
    fantasmaComps.push_back(FanstasmaEyeDx);
    scales.push_back(vec3(80.0, 80.0, 0.0));
    translates.push_back(vec3(490.0, 500.0, 0.0));
    angoli.push_back(0.0);

    cps.clear();
    cps.push_back(vec3(0.0, 0.0, 0.0));
    cps.push_back(vec3(-0.1, 0.1, 0.0));
    cps.push_back(vec3(0.2, 0.3, 0.0));
    GeometricFigure* FanstasmaEyeSx = new GeometricFigure(this->shader, cps, vec4(1.0, 0.0, 0.0, 1.0));
    fantasmaComps.push_back(FanstasmaEyeSx);
    scales.push_back(vec3(80.0, 80.0, 0.0));
    translates.push_back(vec3(510.0, 500.0, 0.0));
    angoli.push_back(0.0);

	if (count < maxG) {
		CompositeGeometry* g = new CompositeGeometry(fantasmaComps);
		this->ghosts.push_back(g);
        randomSpawn();
		count++;
	}
}

void GhostGenerator::simulateGhosts() {
	for (int i = 0; i < this->ghosts.size(); i++) {
		this->ghosts.at(i)->drawFigures(this->scales, this->translates, this->angoli, this->t.at(i));
	}
}