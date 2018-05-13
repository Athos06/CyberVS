#ifndef CONSTANTS_H 
#define CONSTANTS_H

//para filtrado de colisiones en bullet
#define BIT(x) (1<<(x))

enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_PROYECTILE = BIT(0), //<Collide with ships
	COL_OBSTACLES = BIT(1), //<Collide with walls
	COL_PLAYER = BIT(2), //<Collide with walls
};

const int proyectileCollidesWith = COL_OBSTACLES | COL_PLAYER;
const int playerCollidesWith = COL_PROYECTILE;
const int obstaclesCollidesWith = COL_PROYECTILE;
//|||||||||||||||||||||||||||||||||||||||||||||||||||||

const float PI = 3.14159265;
//|||||||||||||||||||||||||||||||||||||||||||||||||||||

//para la dificultad de la IA
const int AI_EASY = 0;
const int AI_NORMAL = 1;
const int AI_HARD = 2;
//|||||||||||||||||||||||||||||||||||||||||||||||||||||

const int CANNON_ROT_SPEED = 30;
const int BASE_AI_ERROR = 4;
//|||||||||||||||||||||||||||||||||||||||||||||||||||||

#endif