#include "LightGenerator.h"

LightGenerator::LightGenerator(const Shader &shader, vector<Obstacle*> obs, float width, float height) {
	for (int i = 0; i < obs.size(); i++) {
		for (int j = 0; j < obs.at(i)->edges.size(); j++) {
			this->edges.push_back(obs.at(i)->edges.at(j));
		}
	}
	printf("%d\n", this->edges.size());
	this->shader = shader;

	////calculate edges of the screen
	//tuple<vec3, vec3> e1{ vec3(0.0, 0.0, 0.0), vec3(width, 0.0, 0.0) };
	//tuple<vec3, vec3> e2{ vec3(width, 0.0, 0.0), vec3(width, height, 0.0) };
	//tuple<vec3, vec3> e3{ vec3(width, height, 0.0) , vec3(0.0, height, 0.0) };
	//tuple<vec3, vec3> e4{ vec3(0.0, height, 0.0), vec3(0.0, 0.0, 0.0) };

	//this->edges.push_back(e1);
	//this->edges.push_back(e2);
	//this->edges.push_back(e3);
	//this->edges.push_back(e4);

}

void LightGenerator::calculateLight(vec2 c, float radius) {
	rays.clear();

	for (auto& e1 : this->edges) {
		for (int i = 0; i < 2; i++) {
			float rdx;
			float rdy;

			rdx = (i == 0 ? std::get<0>(e1).x : std::get<1>(e1).x) - c.x;
			rdy = (i == 0 ? std::get<0>(e1).y : std::get<1>(e1).y) - c.y;

			float theta = atan2f(rdy, rdx);
			float alpha = 0;

			for (int j = 0; j < 3; j++) {
				if (j == 0) alpha = theta + 0.0001f;
				if (j == 1) alpha = theta ;
				if (j == 2) alpha = theta - 0.0001f;

				rdx = radius * cosf(alpha);
				rdy = radius * sinf(alpha);

				float minT1 = INFINITY;
				float minPx = 0.0f, minPy = 0.0f, minTheta = 0.0f;
				bool bValid = false;

				for (auto& e2 : this->edges) {
					
					float sdx = std::get<1>(e2).x - std::get<0>(e2).x;
					float sdy = std::get<1>(e2).y - std::get<0>(e2).y;

					if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f) {
						float t2 = (rdx * (std::get<0>(e2).y - c.y) + (rdy * (c.x - std::get<0>(e2).x))) / (sdx * rdy - sdy * rdx);

						float t1 = (std::get<0>(e2).x + sdx * t2 - c.x) / rdx;

						if (t1 > 0.0f && (t2 >= 0.0f && t2 <= 1.0f)) {
							if (t1 < minT1) {
								minT1 = t1;
								minPx = c.x + rdx * t1;
								minPy = c.y + rdy * t1;
								minTheta = atan2f(minPy - c.y, minPx - c.x);
								bValid = true;
							}
						}
					}

				}
				if (bValid)
					rays.push_back({ minTheta, minPx, minPy });
			}
		}
	}

	sort(rays.begin(), rays.end(), 
		[&](const tuple <float, float, float>& t1, const tuple <float, float, float>& t2) {
			return std::get<0>(t1) < std::get<0>(t2);
		});
}

void LightGenerator::initVao() {
	this->vertices.clear();
	this->colors.clear();
	for (auto& r : this->rays) {
		this->vertices.push_back(vec3(get<1>(r), get<2>(r), 0.0));
		this->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
	}

	//this->vertices.clear();

	//this->vertices.push_back(vec3(100.0, 100.0, 0.0));
	//this->vertices.push_back(vec3(300.0, 100.0, 0.0));
	//this->vertices.push_back(vec3(200.0, 300.0, 0.0));

	//this->colors.clear();

	//this->colors.push_back(vec4(1.0, 0.0, 1.0, 1.0));
	//this->colors.push_back(vec4(1.0, 0.0, 1.0, 1.0));
	//this->colors.push_back(vec4(1.0, 0.0, 1.0, 1.0));

	GLuint VBO_G, VBO_C;

	glGenVertexArrays(1, &(this->VAO));
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_G);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3), this->vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void LightGenerator::renderLight(mat4 model) {
	this->shader.Use();

	this->initVao();

	this->model = model;
	//model = translate(model, vec3(500.0, 500.0, 0.0));
	//model = scale(model, vec3(10000.0, 10000.0, 0.0));
	this->shader.SetInteger("sceltaFS", 0);
	this->shader.SetMatrix4("model", model);

	glBindVertexArray(VAO);
	printf("%f %f\n", this->vertices.at(5).x, this->vertices.at(5).y);
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size());
	glBindVertexArray(0);
}
