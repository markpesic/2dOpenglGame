#include "BulletGenerator.h"

BulletGenerator::BulletGenerator(const Shader& shader, vec2 c, vec2 radius, int maxBullets) {
	this->maxB = maxBullets;
	this->model = mat4(1.0);
	this->translation = vec3(0.0);
	this->s = shader;
	this->c = c;
	this->radius = radius;
	this->count = 0;
}

void BulletGenerator::generateBullet() {
	if (count <= this->maxB) {
		GeometricFigure* p = new GeometricFigure(this->s, this->c, this->radius);
		p->position = vec3(0.0);
		p->model = mat4(1.0);
		this->bullets.push_back(p);
		count++;
	}
}

void BulletGenerator::simulateBullets(mat4 modelGun) {
	vector<int> trash;
	for (int i = 0; i < this->bullets.size(); i++) {
		vec3 pos = vec3(this->bullets.at(i)->model * vec4(c.x + this->bullets.at(i)->position.x, c.y+this->bullets.at(i)->position.y, 0.0, 1.0));
		if (pos.y >= 1200) {
			trash.push_back(i);
		}
	}

	for (int i = 0; i < trash.size(); i++) {
		this->bullets.erase(this->bullets.begin()+trash.at(i));
		count--;
	}

	for (int i = 0; i < this->bullets.size(); i++) {
		if (this->bullets.at(i)->position.y == 0.0) {
			this->bullets.at(i)->model = modelGun;
		}
		this->bullets.at(i)->position = vec3(0.0, this->bullets.at(i)->position.y + velY, 0.0);
		this->bullets.at(i)->model = translate(this->bullets.at(i)->model, this->bullets.at(i)->position);
		this->bullets.at(i)->drawFigure();
	}
}