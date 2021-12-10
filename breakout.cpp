/**
 * file: breakout.cpp
 * lab: 11
 * by: Ragy costa de jesus
 * org: COP2001, 202102, 10410
 * desc: main application file for a 20 breakout game
 * --------------------------------------------------------
 */

#include <iostream>   //console library for debugging


#include "fgcugl.h"   //fgcu OpenGl library

#include "breakout.h"   //main header file

#include "paddle.h"    //create the player paddle

#include "ball.h"




//function prototype
//creation
void createBricks( Block bricks[BRICK_ROWS][BRICK_COLUMNS]);

//main function
Direction processInput();
bool update(Ball& ball,Paddle& paddle, Block bricks[BRICK_ROWS][BRICK_COLUMNS], Walls walls, double lag, Direction next );
void render(Ball ball, Paddle paddle, Block bricks[BRICK_ROWS][BRICK_COLUMNS],Walls walls, double lag);


int main() {


    fgcugl::openWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, false);

    //border walls
    Walls walls;
    walls.top = Block(0 , WINDOW_HEIGHT - WALL_SIZE, WINDOW_WIDTH, WALL_SIZE);
    walls.bottom = Block(0, 0, WINDOW_WIDTH, WALL_SIZE);
    walls.left = Block(0, 0, WALL_SIZE, WINDOW_HEIGHT);
    walls.right = Block(WINDOW_WIDTH - WALL_SIZE, 0, WALL_SIZE, WINDOW_HEIGHT);


    //create the player paddle
    int padx = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2;
    int pady = walls.bottom.top() + 1 + PADDLE_HEIGHT;
    Paddle paddle = Paddle(padx, pady, PADDLE_WIDTH, PADDLE_HEIGHT, fgcugl::Green);

    //create a ball
    float ballX = WINDOW_WIDTH / 2.0;
    float ballY = paddle.top() + BALL_SIZE + 1.0;
    Ball ball = Ball(ballX, ballY , BALL_SPEED_X, BALL_SPEED_Y, BALL_SIZE, BALL_COLOR );


    //CREATE RACK OF BRICKS
    Block bricks[BRICK_ROWS][BRICK_COLUMNS];
    createBricks(bricks);


    //DIRECTIONS
    Direction currentDirection = DIR_NONE, nextDirections = DIR_NONE;


    //timing for game loop
    double startTime = fgcugl::getTime();
    double finishTime = 0.0, deltaTime = 0.0;



    //GAME OVER
    bool gameover = false;

    //main game
    while(!gameover)
    {
        finishTime = fgcugl::getTime();         //stop frame Timer
        deltaTime += finishTime - startTime;    //add current lag
        startTime = finishTime;

        //processInput();
        nextDirections = processInput();

        //update();
        while(deltaTime >= FRAME_RATE)
        {
            gameover = update( ball,paddle, bricks, walls, deltaTime,nextDirections );
            deltaTime -= FRAME_RATE;
        }

        //render();
        render( ball, paddle, bricks, walls, deltaTime);


        //get keyboard and window move/close events
        fgcugl::getEvents();

        //end the game if window is closed
        gameover = gameover || fgcugl::windowClosing();

    }// end of the game

    fgcugl::cleanup();

    return 0;
}   //end main


/**
 CREATE  RACK BRICKS

 Parameters:
    brick:      the array of bricks

 Return:
    void

*/
void createBricks( Block bricks[BRICK_ROWS][BRICK_COLUMNS])
{
    int y = BRICK_START_Y;

    for ( int row = 0; row < BRICK_ROWS; row++)
    {
        int x = BRICK_START_X;

        for( int column = 0 ; column < BRICK_COLUMNS; column++)
        {

            fgcugl:: Color blockColor;

            //assign color to block rows
            switch(row)
            {
                case 0:
                case 1:
                    blockColor = fgcugl::Yellow;
                    break;
                case 2:
                case 3:
                    blockColor = fgcugl:: Green;
                    break;
                case 4:
                case 5:
                    blockColor = fgcugl::Orange;
                    break;
                case 6:
                case 7:
                    blockColor = fgcugl::Red;
            }   //END OF SWITCH

            bricks[row][column] = Block(x, y, BRICK_WIDTH, BRICK_HEIGHT, blockColor);

            x += BRICK_WIDTH;
        }   //COLUMNS

        y += BRICK_HEIGHT;
    }   //rows

}   //end of creatBricks



/**
 Get user keyboard input

 Parameters:

 Return:
    Direction   next direction to move(default to  NONE)

*/
Direction processInput()
{
    Direction direction = DIR_NONE;

    //get keyboard input
    char key;
    key = fgcugl::getKey();

    switch(key) {
        case 'W':   //up
            direction = DIR_UP;
            break;
        case 'S':   //down
            direction = DIR_DOWN;
            break;
        case 'A':   //left
            direction = DIR_LEFT;
            break;
        case 'D':   //right
            direction = DIR_RIGHT;
            break;
        case 'X':
            direction = DIR_EXIT;
    }


    return direction;
}

/**
 Update game object
 parameters:
     lag:               current frame time plus lag
     paddle:            player paddle
     brick:             the rack of blocks
     walls:             border walls for collision checks
     ball:              location and speed of the ball
     next               next direction to move the ball
 returns:
 bool       true if game should end
 */
bool update(Ball& ball,Paddle& paddle, Block bricks[BRICK_ROWS][BRICK_COLUMNS] ,Walls walls, double lag, Direction next )
{
    bool quit = false;


    Paddle::PaddleDirection paddleDir = Paddle::None;
    if(next == DIR_LEFT)
    {
        paddleDir = Paddle::Left;

    }
    else if(next == DIR_RIGHT)
    {
        paddleDir = Paddle::Right;
    }

    paddle.update(paddleDir, walls.left, walls.right, lag);

    //adjust ball to new position based on speed
    ball.upate(lag);

    //see if ball hit the bottom wall
    if(ball.collisionCheck(walls.bottom))
    {
        quit = true;
    }
    else
    {
        //check if the ball hits the wall
        int collision = ball.collisionCheck( walls.top);
        collision |= ball.collisionCheck(walls.bottom);
        collision |= ball.collisionCheck( walls.left);
        collision |= ball.collisionCheck( walls.right);

        //check for collision of ball with bricks
        //do not start checking until the ball has reach
        //the lowest brick
        if ( ball.top() >= BRICK_START_Y)
        {
            //draw bricks
            for(int row = 0 ; row < BRICK_ROWS; row++)
            {
                for(int column = 0; column < BRICK_COLUMNS ; column++)
                {
                    //if block is not broken(has a y-coordinate)
                    if (!bricks[row][column].isEmpty())
                    {
                        Ball::CollisionType brickCollision = ball.collisionCheck(bricks[row][column] );
                        if (brickCollision)
                        {
                            collision |= brickCollision;
                            bricks[row][column] = Block();
                        }
                    }   //not broken

                }   //column

            }     //row
        }      //ball reached block
        else
        {
            //check for collision with the player paddle
            collision |= ball.collisionCheck(paddle.getPaddle());
        }
        ball.doCollision(collision);

    }   //collision checking


    return quit;
}   //end of update



/**
 draw game object into OpenGL window

 parameters:
     lag:               current frame time plus lag
     paddle:            player paddle
     bricks:            the rack of blocks
     walls:             border walls for collision checks
     ball:              build up of lag between frames

 returns:
 void
 */
void render(Ball ball, Paddle paddle,Block bricks[BRICK_ROWS][BRICK_COLUMNS], Walls walls, double lag )
{
    //draw paddle
    paddle.draw(lag);

    //draw ball in new position relative to current lag
    ball.draw(lag);

    //draw bricks
    for(int row = 0 ; row < BRICK_ROWS; row++)
    {
        for(int column = 0; column < BRICK_COLUMNS ; column++)
        {
            //if block is not broken(has a y-coordinate)
            if (!bricks[row][column].isEmpty() )
            {
                bricks[row][column].drawWithBorder();

            }
        }   //column

    }   //row

    //Draw walls
    walls.top.draw();
    walls.bottom.draw();
    walls.left.draw();
    walls.right.draw();

    //paint the new scene in the window
    fgcugl::windowPaint();
}   // end of render
