#pragma once
#ifndef GAME_H
#define GAME_H

#include "Lib.h";
#include "geometricFigures.h"
#include "compositeGeometry.h"
#include "BulletGenerator.h"
#include"Obstacles.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GeometricFigure *Renderer;
    GameState State;
    float deltaTime;
    bool Keys[1024];
    unsigned int Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void setParameters(float dx, float d, float theta, float deltaTime);
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    bool AnyCollision();
    bool IsCollision(CompositeGeometry *f,Obstacle *o);
    bool IsCollision(BulletGenerator* f, Obstacle* o);
    bool IsCollision(BulletGenerator* b, CompositeGeometry* o);
    vec3 resolveCollision(CompositeGeometry* f, Obstacle* o);
    int getDirection();
};

#endif
