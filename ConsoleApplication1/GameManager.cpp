#include "GameManager.h";
#include "resourceManager.h"
#include "geometricFigures.h"
#include "compositeGeometry.h"
#include "GhostGenerator.h"
#include "Obstacles.h"
#include "Lib.h"

mat4 projection;
vector<vec3> vscales;
vector<vec3> vtranslations;
vector<float> angoli;

vector<vec3> sObs;
vector<vec3> tObs;
vector<float> aObs;

vector<vec3> fscales;
vector<vec3> ftranslations;
vector<float> fangoli;

vec3 bbox;

Obstacle* RenderBox1;
BulletGenerator* bulGen;
vector<GeometricFigure*> figures;
vector<GeometricFigure*> fantasmaComps;
vector<Obstacle*> obstacles;
vector<CompositeGeometry*> fantasmi;
CompositeGeometry *Renderer1;
CompositeGeometry* RendererFantasma;
GhostGenerator* ghostGen;
Background* background;
GeometricFigure* palla;
vec3 Dir;
float dex, dey;
int d;
Game::Game(unsigned int width, unsigned int height) :
	Width(width), Height(height), State(GAME_ACTIVE), Keys() {}

Game::~Game() {}

void Game::Init() {
	ResourceManager::LoadShader("vertexShader.glsl", "fragmentShader.glsl", nullptr, "curva");
    //ResourceManager::LoadShader("vertexShader.glsl", "fragmentShader.glsl", nullptr, "obstacle");
    projection = ortho(0.0f, float(Width), 0.0f, float(Height));
    float angolo =  0.0;
    vec3 vscale = vec3(50.0f, 50.0f, 0.0);
    vec3 vtranslation = vec3(float(Width / 2), 100, 0.0);
    Dir = vec3(0.0, 0.0, 0.0);
    vscales.push_back(vscale);
    vtranslations.push_back(vtranslation);
    angoli.push_back(angolo);

    vector<glm::vec3> cps;
    cps.push_back(vec3(0.5, 0.5, 0.0));
    cps.push_back(vec3(1.0, 0.0, 0.0));
    cps.push_back(vec3(0.50, -0.50, 0.0));
    cps.push_back(vec3(-0.50, -0.50, 0.0));
    cps.push_back(vec3(-1.00, 0.0, 0.0));
    cps.push_back(vec3(-0.50, 0.50, 0.0));
    // set render-specific controls
    GeometricFigure *Busto = new GeometricFigure(ResourceManager::GetShader("curva"), cps, vec4(0.4, 0.4, 0.4, 1.0));
    figures.push_back(Busto);
    cps.clear();

    cps.push_back(vec3(0.0, 0.0, 0.0));
    cps.push_back(vec3(0.0, 1.0, 0.0));
    cps.push_back(vec3(-0.30, 1.00, 0.0));
    cps.push_back(vec3(-0.30, 0.00, 0.0));

    vscales.push_back(vscale+vec3(0.0f, 20.0f, 0.0f));
    vtranslations.push_back(vtranslation+vec3(50.0f, 0.0f, 0.0f));
    angoli.push_back(angolo);

    GeometricFigure* Braccio = new GeometricFigure(ResourceManager::GetShader("curva"), cps, vec4(0.5, 0.5, 0.5, 1.0));
    figures.push_back(Braccio);
    cps.clear();
    cps.push_back(vec3(0.0, 0.0, 0.0));
    cps.push_back(vec3(0.0, 1.0, 0.0));
    cps.push_back(vec3(-0.30, 1.00, 0.0));
    cps.push_back(vec3(-0.30, 0.00, 0.0));

    vscales.push_back(vscale + vec3(0.0f, 20.0f, 0.0f));
    vtranslations.push_back(vtranslation + vec3(-35.0f, 0.0f, 0.0f));
    angoli.push_back(0.0f);

    GeometricFigure* Braccio1 = new GeometricFigure(ResourceManager::GetShader("curva"), cps, vec4(0.5, 0.5, 0.5, 1.0));
    figures.push_back(Braccio1);
    cps.clear();

    Renderer1 = new CompositeGeometry(figures);

    sObs.push_back(vec3(50.0f, 110.0f, 0.0f));
    tObs.push_back(vec3(300.0f, 240.0f, 0.0f));
    aObs.push_back(0.0f);
    bbox = vec3(400.0f, 370.0f, 0.0f);

    RenderBox1 = new Obstacle(ResourceManager::GetShader("curva"), 0, sObs.at(0), tObs.at(0), aObs.at(0));

    obstacles.push_back(RenderBox1);

    sObs.push_back(vec3(50.0f, 110.0f, 0.0f));
    tObs.push_back(vec3(600.0f, 600.0f, 0.0f));
    aObs.push_back(0.0f);
    bbox = vec3(400.0f, 370.0f, 0.0f);

    RenderBox1 = new Obstacle(ResourceManager::GetShader("curva"), 0, sObs.at(1), tObs.at(1), aObs.at(1));

    obstacles.push_back(RenderBox1);

    sObs.push_back(vec3(50.0f, 110.0f, 0.0f));
    tObs.push_back(vec3(200.0f, 500.0f, 0.0f));
    aObs.push_back(0.0f);
    bbox = vec3(400.0f, 370.0f, 0.0f);

    RenderBox1 = new Obstacle(ResourceManager::GetShader("curva"), 0, sObs.at(2), tObs.at(2), aObs.at(2));

    obstacles.push_back(RenderBox1);

    background = new Background(ResourceManager::GetShader("curva"), 1200, 1200);
    cps.clear();
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
    GeometricFigure* FanstasmaBusto = new GeometricFigure(ResourceManager::GetShader("curva"), cps, vec4(1.0, 1.0, 1.0, 1.0));
    fantasmaComps.push_back(FanstasmaBusto);
    fscales.push_back(vec3(50.0, 50.0, 0.0));
    ftranslations.push_back(vec3(500.0, 500.0, 0.0));
    fangoli.push_back(0.0);

    cps.clear();
    cps.push_back(vec3(0.0, 0.0, 0.0));
    cps.push_back(vec3(0.1, 0.1, 0.0));
    cps.push_back(vec3(-0.2, 0.3, 0.0));
    GeometricFigure* FanstasmaEyeDx = new GeometricFigure(ResourceManager::GetShader("curva"), cps, vec4(1.0, 0.0, 0.0, 1.0));
    fantasmaComps.push_back(FanstasmaEyeDx);
    fscales.push_back(vec3(80.0, 80.0, 0.0));
    ftranslations.push_back(vec3(490.0, 500.0, 0.0));
    fangoli.push_back(0.0);

    cps.clear();
    cps.push_back(vec3(0.0, 0.0, 0.0));
    cps.push_back(vec3(-0.1, 0.1, 0.0));
    cps.push_back(vec3(0.2, 0.3, 0.0));
    GeometricFigure* FanstasmaEyeSx = new GeometricFigure(ResourceManager::GetShader("curva"), cps, vec4(1.0, 0.0, 0.0, 1.0));
    fantasmaComps.push_back(FanstasmaEyeSx);
    fscales.push_back(vec3(80.0, 80.0, 0.0));
    ftranslations.push_back(vec3(510.0, 500.0, 0.0));
    fangoli.push_back(0.0);

    RendererFantasma = new CompositeGeometry(fantasmaComps);

    ghostGen = new GhostGenerator(ResourceManager::GetShader("curva"));

    fantasmi.push_back(RendererFantasma);
    palla = new GeometricFigure(ResourceManager::GetShader("curva"), vec2(400.0, 400.0), vec2(5.0, 5.0));
    palla->model = mat4(1.0);

    bulGen = new BulletGenerator(ResourceManager::GetShader("curva"), vec2(600.0, 400.0), vec2(20.0, 20.0), 10);


}

void Game::ProcessInput(float dt) {
    bulGen->generateBullet();
}

void Game::Update(float dt) {
    ResourceManager::GetShader("curva").SetFloat("time", deltaTime);
}

void Game::setParameters(float dx, float dy, float theta, float deltaTime) {
    dex = dx;
    dey = dy;
    this->deltaTime = deltaTime;
    Renderer1->setParameters(dx, dy, theta, deltaTime);
}

bool Game::IsCollision(CompositeGeometry *f,Obstacle *o) {
    bool collisionX = f->cornerUpperLeft.x < o->cornerBottomRight.x && f->cornerBottomRight.x > o->cornerUpperLeft.x;
    bool collisionY = f->cornerUpperLeft.y > o->cornerBottomRight.y && f->cornerBottomRight.y < o->cornerUpperLeft.y;
    if (collisionX && collisionY) {
        Dir = resolveCollision(f, o);
    }
    else {
        Dir = vec3(0.0);
    }
    return collisionX && collisionY;
}

bool Game::IsCollision(BulletGenerator* b, Obstacle* o) {
    for (int i = 0; i < b->bullets.size(); i++) {
        bool collisionX = b->bullets.at(i)->cornerUpperLeft.x < o->cornerBottomRight.x&& b->bullets.at(i)->cornerBottomRight.x > o->cornerUpperLeft.x;
        bool collisionY = b->bullets.at(i)->cornerUpperLeft.y > o->cornerBottomRight.y && b->bullets.at(i)->cornerBottomRight.y < o->cornerUpperLeft.y;
        if (collisionY && collisionX) {
            b->bullets.erase(b->bullets.begin() + i);
            b->count--;
            return true;
        }
    }
    return false;
}

bool Game::IsCollision(BulletGenerator* b, CompositeGeometry* o) {
    for (int i = 0; i < b->bullets.size(); i++) {
        bool collisionX = b->bullets.at(i)->cornerUpperLeft.x < o->cornerBottomRight.x&& b->bullets.at(i)->cornerBottomRight.x > o->cornerUpperLeft.x;
        bool collisionY = b->bullets.at(i)->cornerUpperLeft.y > o->cornerBottomRight.y && b->bullets.at(i)->cornerBottomRight.y < o->cornerUpperLeft.y;
        if (collisionY && collisionX) {
            b->bullets.erase(b->bullets.begin() + i);
            b->count--;
            o->selfDestroy();
            ghostGen->count--;
            ghostGen->generateGhost();
            return true;
        }
    }
    return false;
}

vec3 Game::resolveCollision(CompositeGeometry* f, Obstacle* o) {
    float minDiff = 1e8;
    int index = 0;
    vec3 resolveDir = vec3(0.0);
    float diffVec[4] = {
        o->cornerBottomRight.x - f->cornerUpperLeft.x,
        o->cornerBottomRight.y - f->cornerUpperLeft.y,
        o->cornerUpperLeft.x - f->cornerBottomRight.x,
        o->cornerUpperLeft.y - f->cornerBottomRight.y };

    for (int i = 0; i < 4; i++) {
        if (abs(diffVec[i]) < minDiff) {
            index = i;
            minDiff = diffVec[i];
        }
    }
    //for (int i = 0; i < 4; i++) {
    //    printf("DiffVec[%d]: %f\n", i,abs(diffVec[i]));
    //}
    //printf("DiffVec: %f\n", minDiff);

    if (index == 0 ) {
        //resolveDir = vec3(abs(diffVec[index]), 0.0, 0.0);
        d = 0;
    }
    else if(index == 1){
        //resolveDir = vec3(0.0,-abs(diffVec[index]), 0.0);
        d = 1;
        //printf("direction game: %d\n", d);
    }
    else if (index == 2) {
        //resolveDir = vec3(-abs(diffVec[index]), 0.0, 0.0);
        d = 2;
    }
    else if(index == 3){
        //resolveDir = vec3(0.0, abs(diffVec[index]), 0.0);
        d = 3;
    }

    return resolveDir;
}

bool Game::AnyCollision() {
    bool col = false;
    for (int i = 0; i < obstacles.size() && !col; i++) {
        col = IsCollision(Renderer1, obstacles.at(i));
    }

    for (int i = 0; i < obstacles.size() && !col; i++) {
        col = IsCollision(bulGen, obstacles.at(i));
    }

    for (int i = 0; i < ghostGen->ghosts.size(); i++) {
        IsCollision(bulGen, ghostGen->ghosts.at(i));
    }
    IsCollision(bulGen, RendererFantasma);
    return col;
}

int Game::getDirection() {
    //printf("direction game: %d\n", d);
    return d;
}

void Game::Render() {
    ResourceManager::GetShader("curva").SetMatrix4("projection", projection);
    background->renderBackgrond();
    for (int i = 0; i < obstacles.size(); i++) {
        obstacles.at(i)->drawObstacle();
    }
    Renderer1->drawFigures(vscales, vtranslations, angoli, Dir);
    ghostGen->simulateGhosts();
    bulGen->simulateBullets(Renderer1->model);
}
