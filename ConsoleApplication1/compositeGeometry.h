#pragma once
#ifndef COMPOSITEGEOMETRY_H
#define COMPOSITEGEOMETRY_H

#include "Lib.h"
#include "geometricFigures.h"

class CompositeGeometry {
public:
	CompositeGeometry(vector<GeometricFigure*> figures);
	//void calculateBoundingBox();
	void drawFigures(vector<vec3> fscales, vector<vec3> ftranslations, vector<float> angoli, vec3 t);
	void setParameters(float dx, float dy, float theta, float deltaTime);
	void setCorners(vec3 cul, vec3 cbr);
	void UpdateBBox();
	void selfDestroy();
	vector<GeometricFigure*> figures;
	vector<vec3> vertices;
	vec3 cornerUpperLeft;//
	vec3 cornerBottomRight;
	float dx;
	float dy;
	float theta;
	float deltaTime;
	mat4 model;
};

#endif