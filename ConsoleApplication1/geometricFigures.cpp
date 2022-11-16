#include "geometricFigures.h"
#include "resourceManager.h"

float deltax, deltay, theta;
float velocityx = 5.0f;
float velocityy = 5.0f;
GeometricFigure::GeometricFigure(const Shader &shader, vector<vec3> cp, vec4 colcp) {
	this->shader = shader;
	this->CP = cp;
	this->color = colcp;
	this->initFigure();
}

GeometricFigure::GeometricFigure(const Shader& shader, vec2 c, vec2 radius) {
	this->shader = shader;
	this->costruisci_cerchio(c.x, c.y, radius.x, radius.y);
	initVao();
}
void GeometricFigure::initFigure() {
	for (int i = 0; i < this->CP.size(); i++) {
		this->derivate.push_back(vec3(0.0, 0.0, 0.0));
	}
	costruisci_formaHermite(vec4(1.0, 0.0, 0.0, 1.0));
	initVao();
}

void GeometricFigure::initVao() {
	glGenVertexArrays(1, &(VAO));
	glBindVertexArray(VAO);

	glGenBuffers(1, &(VBO_G));
	glBindBuffer(GL_ARRAY_BUFFER, VBO_G);
	glBufferData(GL_ARRAY_BUFFER, vertici.size() * sizeof(vec3), this->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &(VBO_C));
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

float GeometricFigure::phi0(float t) {
	return 2.0 * t * t * t - 3.0 * t * t + 1;
}

float GeometricFigure::phi1(float t) {
	return t * t * t - 2.0 * t * t + t;
}

float GeometricFigure::psi0(float t) {
	return -2.0 * t * t * t + 3.0 * t * t;
}

float GeometricFigure::psi1(float t) {
	return t * t * t - t * t;
}

float GeometricFigure::dx(int i, float* t, float Tens, float Bias, float Cont)
{
	if (i == 0)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (this->CP[i + 1].x - this->CP[i].x) / (t[i + 1] - t[i]);
	if (i == this->CP.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (this->CP[i].x - this->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (this->CP.at(i).x - this->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (this->CP.at(i + 1).x - this->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (this->CP.at(i).x - this->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (this->CP.at(i + 1).x - this->CP.at(i).x) / (t[i + 1] - t[i]);
}
float GeometricFigure::dy(int i, float* t, float Tens, float Bias, float Cont)
{
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (this->CP.at(i + 1).y - this->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == this->CP.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (this->CP.at(i).y - this->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (this->CP.at(i).y - this->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (this->CP.at(i + 1).y - this->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (this->CP.at(i).y - this->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (this->CP.at(i + 1).y - this->CP.at(i).y) / (t[i + 1] - t[i]);
}

float GeometricFigure::DX(int i, float* t)
{
	//Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente x della curva è quello originale, altrimenti è quello che è stato modificato nella funzione 
	//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

	if (derivate.at(i).x == 0)
		return dx(i, t, 0.0, 0.0, 0.0);

	if (derivate.at(i).x != 0)
		return derivate.at(i).x;

}
	
float GeometricFigure::DY(int i, float* t)
{
	// Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente y della curva è quello originale, altrimenti è quello che è stato modificato nella funzione
		//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

	if (derivate.at(i).y == 0)
		return dy(i, t, 0.0, 0.0, 0.0);

	if (derivate.at(i).y != 0)
		return derivate.at(i).y;

}

void GeometricFigure::InterpolazioneHermite(float* t, vec4 color, int pval)
{
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);

	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0; //indice dell'estremo sinistro dell'intervallo [t(i),t(i+1)] a cui il punto tg
				//appartiene


	this->vertici.clear();
	this->colors.clear();
	for (tg = 0; tg <= 1; tg += passotg)
	{
		if (tg > t[is + 1])is++;

		ampiezza = t[is + 1] - t[is];
		tgmapp = (tg - t[is]) / ampiezza;

		x = this->CP[is].x * phi0(tgmapp) + DX(is, t) * phi1(tgmapp) * ampiezza + this->CP[is + 1].x * psi0(tgmapp) + DX(is + 1, t) * psi1(tgmapp) * ampiezza;
		y = this->CP[is].y * phi0(tgmapp) + DY(is, t) * phi1(tgmapp) * ampiezza + this->CP[is + 1].y * psi0(tgmapp) + DY(is + 1, t) * psi1(tgmapp) * ampiezza;
		this->vertici.push_back(vec3(x, y, 0.0));
		this->colors.push_back(this->color);
	}

}

void GeometricFigure::costruisci_formaHermite(vec4 color)
{

	if (this->CP.size() > 1)
	{

		float *t = new float[this->CP.size()];
		int i;
		float step = 1.0 / (float)(this->CP.size() - 1);

		for (i = 0; i < this->CP.size(); i++)
			t[i] = (float)i * step;

		InterpolazioneHermite(t, color, 140);
	}

}

void GeometricFigure::costruisci_cerchio(float cx, float cy, float raggiox, float raggioy) {
	float steps = 3.14159 / 100.0;

	this->vertici.push_back(vec3(cx, cy, 0.0));
	this->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

	for (int i = 0; i <= 100.0; i++) {
		float t = (float)i * steps * 3.14159;
		this->vertici.push_back(vec3(cx + raggiox * cos(t), cy + raggioy * sin(t), 0.0));
		this->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	}
}

void GeometricFigure::setParameters(float dx, float dy, float t) {
		deltax = dx;
		deltay = dy;
		theta = t;
}

void GeometricFigure::updateBBox() {
	vector<vec3> realVertices;
	for (int i = 0; i < this->vertici.size(); i++) {
		realVertices.push_back(vec3(this->model * vec4(this->vertici.at(i), 1.0)));
	}

	float minX = 1e8;
	float maxX = -1e8;
	float minY = 1e8;
	float maxY = -1e8;

	for (int i = 0; i < realVertices.size(); i++) {
		if (realVertices.at(i).x < minX) {
			minX = realVertices.at(i).x;
		}
	}

	for (int i = 0; i < realVertices.size(); i++) {
		if (realVertices.at(i).x > maxX) {
			maxX = realVertices.at(i).x;
		}
	}

	for (int i = 0; i < realVertices.size(); i++) {
		if (realVertices.at(i).y < minY) {
			minY = realVertices.at(i).y;
		}
	}

	for (int i = 0; i < realVertices.size(); i++) {
		if (realVertices.at(i).y > maxY) {
			maxY = realVertices.at(i).y;
		}
	}

	this->cornerBottomRight = vec3(maxX, minY, 0.0);
	this->cornerUpperLeft = vec3(minX, maxY, 0.0);
}

void GeometricFigure::drawFigure() {
	(this->shader).Use();
	
	this->updateBBox();
	shader.SetInteger("sceltaFS", 0);
	this->shader.SetMatrix4("model", model);

	glBindVertexArray(VAO);
	//printf("%d\n", this->vertici.size());
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertici.size());
	glBindVertexArray(0);
}

Background::Background(const Shader &shader, float width, float height){
	this->width = width;
	this->height = height;
	this->shader = shader;
	this->initVao();
}

void Background::initVao() {
	float back[] = {
			-1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f
	};

	float colors[] = {
	0.2, 0.2, 0.2, 1.0,
	0.2, 0.2, 0.2, 1.0,
	0.2, 0.2, 0.2, 1.0,
	0.2, 0.2, 0.2, 1.0,
	0.2, 0.2, 0.2, 1.0,
	0.2, 0.2, 0.2, 1.0,
	};

	GLuint VBO_G, VBO_C;
	glGenVertexArrays(1, &(this->VAO));
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_G);
	glBufferData(GL_ARRAY_BUFFER, sizeof(back), back, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(1);
}

void Background::renderBackgrond() {
	this->shader.Use();

	mat4 model = mat4(1.0);
	model = translate(model, vec3(200, 200, 0.0));
	model = scale(model, vec3(this->width,this->height-300, 0.0));

	this->shader.SetInteger("sceltaFS", 2);
	this->shader.SetMatrix4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}