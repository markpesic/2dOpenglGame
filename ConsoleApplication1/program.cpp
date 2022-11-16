#include "Lib.h"
#include<iostream>
#include<math.h>

#include"GameManager.h"
#include"resourceManager.h"

const unsigned int width = 1200;
const unsigned int height = 800;

Game game(width, height);

void resize(GLsizei w, GLsizei h);
float dx = 0.0, dy = 0.0;
int precD = -1.0;
bool iscol = false;
int direc = 0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		if (!iscol) {
			dx += -1.0;
			precD = 1;
		}
		else
			dx += 1.0;
		break;
	case 'd':
		if (!iscol) {
			dx += 1.0;
			precD = 3;
		}
		else
			dx += -1.0;
		break;
	case ' ':
		game.ProcessInput(0.0);
		printf("Creazione di un bullet\n");
	}

	glutPostRedisplay();
}
float psi = 0.0;
void mouseMotion(int x, int y) {
	printf("%d %d\n",x,y);
	vec3 mousevec = vec3(x, y, 0.0);
	mousevec = normalize(mousevec);
	float r = dot(vec3(0.0, 1.0, 0.0), mousevec);
	r = acos(r);
	psi = (r * 90.0f) / (3.14f / 2.0f);
	printf("%f\n", psi);
	glutPostRedisplay();
}

void updateTime(int value) {
	game.Update(lastFrame*1000.0f);
}
float offset = 0.0;
void drawScene() {
	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentFrame - lastFrame)*0.001;
	lastFrame = currentFrame;
	//printf("deltaTime %f\n",deltaTime);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (currentFrame - offset >= 100) {
		//glutTimerFunc(1000, updateTime, 0);
		game.Update((float)currentFrame*1000.0f);
		offset = currentFrame;
	}
	game.setParameters(dx, dy, psi, lastFrame);
	game.Render();
	iscol = game.AnyCollision();
	direc = game.getDirection();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_RIGHT_BUTTON);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Prova OpenGL");
	glutReshapeFunc(resize);
	//glutMouseFunc();
	glutKeyboardFunc(myKeyboard);
	glutMotionFunc(mouseMotion);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int deltaTime = 0;
	int lastFrame = 0;
	game.Init();
	//while (1) {
	//	glutMainLoopEvent();
	//	int currentFrame = glutGet(GLUT_ELAPSED_TIME);
	//	deltaTime = currentFrame - lastFrame;
	//	lastFrame = currentFrame;
	//	game.ProcessInput((float)deltaTime);

	//	game.Update((float)deltaTime);

	//	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	game.Render();
	//	glutSwapBuffers();
	//}
	//glutLeaveMainLoop();
	glutDisplayFunc(drawScene);
	glutMainLoop();
	ResourceManager::Clear();
	return 0;
}

void resize(GLsizei w, GLsizei h) {
	float AspectRatio_mondo = (float)(width) / (float)(height);

	if (AspectRatio_mondo > w / h) //Se l'aspect ratio del mondo è diversa da quella della finestra devo mappare in modo diverso per evitare distorsioni del disegno
	{
		glViewport(0, 0, w, w / AspectRatio_mondo);
	}
	else {
		glViewport(0, 0, h * AspectRatio_mondo, h);
	}


	glutPostRedisplay();
}