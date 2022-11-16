#include "compositeGeometry.h"

CompositeGeometry::CompositeGeometry(vector<GeometricFigure*> figures) {
	this->figures = figures;
}

void CompositeGeometry::drawFigures(vector<vec3> fscales, vector<vec3> ftranslations, vector<float> angoli, vec3 col) {
	this->model = mat4(1.0);
	this->model = translate(this->model, vec3(dx*10.0f+col.x, dy*5.0f+col.y, 0.0f));

	for (int i = 0; i < this->figures.size(); i++) {
		this->figures.at(i)->model = this->model;
		this->figures.at(i)->model = translate(this->figures.at(i)->model, ftranslations.at(i));
		this->figures.at(i)->model = rotate(this->figures.at(i)->model, angoli.at(i), vec3(0.0, 0.0, 1.0));
		this->figures.at(i)->model = scale(this->figures.at(i)->model, fscales.at(i));

		this->figures.at(i)->drawFigure();
	}
	this->UpdateBBox();
}

void CompositeGeometry::selfDestroy() {
	for (int i = 0; i < this->figures.size(); i++) {
		this->figures.at(i)->vertici.clear();
		this->figures.at(i)->colors.clear();
		this->figures.at(i)->derivate.clear();
		glDeleteVertexArrays(1, &this->figures.at(i)->VAO);
	}

	this->figures.clear();
}

void CompositeGeometry::setParameters(float dx, float dy, float theta, float deltaTime) {
	for (int i = 0; i < this->figures.size(); i++) {
		this->figures.at(i)->setParameters(dx, dy, theta);
	}
	this->dx = dx;
	this->dy = dy;
	this->theta = theta;
}

void CompositeGeometry::UpdateBBox() {
	this->vertices.clear();
	for (int i = 0; i < this->figures.size(); i++) {
		mat4 tempModel = this->figures.at(i)->model;
		for (int j = 0; j < this->figures.at(i)->vertici.size(); j++) {
			this->vertices.push_back(vec3(tempModel * vec4(this->figures.at(i)->vertici.at(j), 1.0)));
		}
	}

	float minX = 1e8;
	float maxX = -1e8;
	float minY = 1e8;
	float maxY = -1e8;

	for (int i = 0; i < this->vertices.size(); i++) {
		if (this->vertices.at(i).x < minX) {
			minX = this->vertices.at(i).x;
		}
	}

	for (int i = 0; i < this->vertices.size(); i++) {
		if (this->vertices.at(i).x > maxX) {
			maxX = this->vertices.at(i).x;
		}
	}

	for (int i = 0; i < this->vertices.size(); i++) {
		if (this->vertices.at(i).y < minY) {
			minY = this->vertices.at(i).y;
		}
	}

	for (int i = 0; i < this->vertices.size(); i++) {
		if (this->vertices.at(i).y > maxY) {
			maxY = this->vertices.at(i).y;
		}
	}

	this->cornerBottomRight = vec3(maxX,minY, 0.0);
	this->cornerUpperLeft = vec3(minX, maxY, 0.0);


	//printf("CompositeGeometry cornerBottomRight: %f %f\n", maxX, minY);
	//printf("CompositeGeometry cornerUpperLeft: %f %f\n", minX, maxY);
}

void CompositeGeometry::setCorners(vec3 cul, vec3 cbr) {
	this->cornerUpperLeft = cul;
	this->cornerBottomRight = cbr;
}