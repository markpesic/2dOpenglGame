#include "Obstacles.h"

void Obstacle::initVAO(int type) {
	if (type == 0) {
		float type0[] = {
			-1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f
		};
		for (int i = 0; i < 18; i++) {
			vertices[i] = type0[i];
		}
	}
	else{
		float type1[] = {
			-1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			-1.0f, 0.5f, 0.0f,

			-1.0f, 0.5f, 0.0f,
			1.0f, 0.5f, 0.0f,
			1.0f, 0.0f, 0.0f
		};
		for (int i = 0; i < 18; i++) {
			vertices[i] = type1[i];
		}
	}

	GLuint VBO_G, VBO_C;

	float colors[] = {
		0.2, 0.2, 0.2, 1.0,
		0.2, 0.2, 0.2, 1.0,
		0.2, 0.2, 0.2, 1.0,
		0.2, 0.2, 0.2, 1.0,
		0.2, 0.2, 0.2, 1.0,
		0.2, 0.2, 0.2, 1.0,
	};

	glGenVertexArrays(1, &(this->VAO));
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_G);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors) , colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(1);
}

Obstacle::Obstacle(const Shader &shader, int type, vec3 s, vec3 t, float th) {
	this->shader = shader;
	this->initVAO(type);
	this->model = mat4(1.0);
	this->model = translate(this->model, t);
	this->model = rotate(this->model, th, vec3(0.0, 0.0, 1.0));
	this->model = scale(this->model, s);

	this->UpdateBBox();
}

void Obstacle::UpdateBBox() {
	edges.clear();
	vector<vec3> verts = vector<vec3>();
	for (int i = 0; i < 18; i+= 3) {
		verts.push_back(vec3(model*vec4(vertices[i], vertices[i+1], vertices[i+2],1.0)));
	}

	float minX = 1e8;
	float maxX = -1e8;
	float minY = 1e8;
	float maxY = -1e8;

	for (int i = 0; i < verts.size(); i++) {
		if (verts.at(i).x < minX) {
			minX = verts.at(i).x;
		}
	}

	for (int i = 0; i < verts.size(); i++) {
		if (verts.at(i).x > maxX) {
			maxX = verts.at(i).x;
		}
	}

	for (int i = 0; i < verts.size(); i++) {
		if (verts.at(i).y < minY) {
			minY = verts.at(i).y;
		}
	}

	for (int i = 0; i < verts.size(); i++) {
		if (verts.at(i).y > maxY) {
			maxY = verts.at(i).y;
		}
	}

	this->cornerBottomRight = vec3(maxX, minY, 0.0);
	this->cornerUpperLeft = vec3(minX, maxY, 0.0);

	this->cornerBottomLeft = vec3(minX, minY, 0.0);
	this->cornerUpperRight = vec3(maxX, maxY, 0.0);

	tuple<vec3, vec3> e1 {cornerUpperLeft, cornerBottomLeft};
	tuple<vec3, vec3> e2 {cornerBottomLeft, cornerBottomRight};
	tuple<vec3, vec3> e3 {cornerBottomRight, cornerUpperRight};
	tuple<vec3, vec3> e4 {cornerUpperRight, cornerUpperLeft};

	this->edges.push_back(e1);
	this->edges.push_back(e2);
	this->edges.push_back(e3);
	this->edges.push_back(e4);

	//printf("Obstacles cornerBottomRight: %f %f\n", maxX, minY);
	//printf("Obstacles cornerUpperLeft: %f %f\n", minX, maxY);
}

void Obstacle::drawObstacle() {
	this->shader.Use();

	if (this->cornerBottomRight.x >= 1000) {
		this->direction = -1.0f;
	}
	if (this->cornerBottomLeft.x <= 10) {
		this->direction = 1.0f;
	}
	this->model = translate(this->model, vec3(direction*velocitX, 0.0, 0.0));
	this->UpdateBBox();
	shader.SetInteger("sceltaFS", 1);
	shader.SetMatrix4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}