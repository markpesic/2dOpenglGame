#pragma once
#ifndef GEOMETRICFIGURES_H
#define GEOMETRICFIGURES_H

#include "Lib.h"
#include "shader.h"
// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class GeometricFigure
{
public:
	GeometricFigure(const Shader &shader, vector<vec3> cp, vec4 colcp);
	GeometricFigure(const Shader& shader, vec2 c, vec2 radius);
	void initFigure();
	void initVao();
	float dx(int i, float* t, float Tens, float Bias, float Cont);
	float dy(int i, float* t, float Tens, float Bias, float Cont);
	float DX(int i, float* t);
	float DY(int i, float* t);
	void InterpolazioneHermite(float* t, vec4 color, int pval);
	void costruisci_formaHermite(vec4 color);
	void costruisci_cerchio(float cx, float cy, float raggiox, float raggioy);
	float phi0(float t);
	float phi1(float t);
	float psi0(float t);
	float psi1(float t);
	void updateBBox();
	void setParameters(float dx, float dy, float theta);
	void drawFigure();
	
	GLuint VAO;
	GLuint VBO_C;
	GLuint VBO_G;

	vector<vec3> vertici;
	vector<vec3> CP;
	vector<vec4> colors;
	vector<vec3> derivate;
	vec4 color;
	Shader shader;

	vec3 position;
	
	vec3 cornerUpperLeft;
	vec3 cornerBottomRight;
	mat4 model;

};

class Background {
public:
	Background(const Shader &shader, float width, float height);
	void initVao();
	void renderBackgrond();

	float width;
	float height;
	Shader shader;
	GLuint VAO;
};

#endif