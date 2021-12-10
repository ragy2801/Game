/**
 * file: breakout.cpp
 * lab: 11
 * by: Ragy costa de jesus
 * org: COP2001, 202102, 10410
 * desc: main application header file for a 20 breakout game
 * --------------------------------------------------------
 */

#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <string> //window Title

#include "fgcugl.h" // fgcu openGl library

#include "block.h"


//Global constants
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 604;
const std::string WINDOW_TITLE = "Breakout";

//game settings
const double FRAME_RATE = 1.0 / 60.0;



//flag values
//---------------

//----------------
//enums

enum Direction {
    DIR_EXIT = -1,
    DIR_NONE,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

//----------------------------------
//global constants


//game components

//Player paddle properties
int PADDLE_WIDTH = 96;
int PADDLE_HEIGHT = 20;


//ball properties
const int BALL_SIZE = 8; //BALL RADIUS
const fgcugl::Color BALL_COLOR = fgcugl::Blue;
const float BALL_SPEED_X = 40.0;     //Incremental speed horizontally in pixels
const float BALL_SPEED_Y = 70.0;     //Incremental speed vertically in pixels


//border walLS
const int WALL_SIZE = 2;

//bricks
const int BRICK_WIDTH = 64;
const int BRICK_HEIGHT = 20;
const int BRICK_COLUMNS = 14;       //Number of bricks across
const int BRICK_ROWS = 8;           //Number of bricks down
const int BRICK_START_X  = WALL_SIZE;
const int BRICK_START_Y  = (WINDOW_HEIGHT - 2 * WALL_SIZE) / 2  ;

//----------------------------
//structures

//border walls
struct Walls {
    Block top;
    Block bottom;
    Block left;
    Block right;
};


#endif //BREAKOUT_H
