#include "game.h"
#include <iostream>

const int thickness             = 15;
const int PADDLE_H              = 200;
const int PADDLE_MOVE_SPEED     = 300;    // pixels/second
const int BALL_MOVE_SPEED_X     = 200;    // pixels/second
const int BALL_DIRECT_LEFT      = -1;
const int BALL_DIRECT_RIGHT     = 1;
const int BALL_MOVE_SPEED_Y     = 235;    // pixels/second
const int BALL_DIRECT_UP        = -1;
const int BALL_DIRECT_DOWN      = 1;


const int WINDOW_W             = 1024;
const int WINDOW_H             = 768;

Game::Game() : 
    m_IsRunning  (true), 
    mWindow      (nullptr), 
    mRenderer    (nullptr),
    mTicksCount  (0),
    mPaddle1Pos  ({ thickness / 2,              WINDOW_H / 2 }),
    mPaddle2Pos  ({ WINDOW_W - thickness / 2,   WINDOW_H / 2 }),
    mPaddle1Dir  (0),
    mPaddle2Dir  (0),
    mBalls       ({
                    {
                        { WINDOW_W / 2 ,                  WINDOW_H / 2 }, 
                        { BALL_DIRECT_RIGHT * BALL_MOVE_SPEED_X, BALL_DIRECT_DOWN * BALL_MOVE_SPEED_Y}
                    },
                    {
                        { WINDOW_W / 2 ,                  WINDOW_H / 2 }, 
                        { BALL_DIRECT_LEFT * (BALL_MOVE_SPEED_X + 30), BALL_DIRECT_UP * (BALL_MOVE_SPEED_Y - 30)}
                    },
                    // {
                    //     { WINDOW_W / 2 ,                  WINDOW_H / 2 }, 
                    //     { BALL_DIRECT_LEFT * (BALL_MOVE_SPEED_X - 100), BALL_DIRECT_DOWN * (BALL_MOVE_SPEED_Y + 100)}
                    // },
                    // {
                    //     { WINDOW_W / 2 ,                  WINDOW_H / 2 }, 
                    //     { BALL_DIRECT_RIGHT * (BALL_MOVE_SPEED_X - 30), BALL_DIRECT_UP * (BALL_MOVE_SPEED_Y + 30)}
                    // }
    }),
    mAttempts    (3)
{
}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)", // Window title
        100, // Top left x-coordinate of window
        100, // Top left y-coordinate of window
        WINDOW_W, // Width of window
        WINDOW_H, // Height of window
        SDL_WINDOW_RESIZABLE // Flags (0 for no flags set)
    );
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow, // Window to create renderer for
        -1, // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}

void Game::RunLoop()
{
    while (m_IsRunning)
    {
        ProcessInput(); 
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))    // returns true if it finds an event in the queue.
    {
        switch (event.type)
        {
        case SDL_QUIT:    // receives when the user tries to close the window(either by clicking on the X or using a keyboard shortcut).
            m_IsRunning = false;
            break;
        default:
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {    // when the user presses the Escape key
        m_IsRunning = false;
    }

    mPaddle1Dir = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddle1Dir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddle1Dir += 1;
    }
    mPaddle2Dir = 0;
    if (state[SDL_SCANCODE_UP])
    {
        mPaddle2Dir -= 1;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        mPaddle2Dir += 1;
    }
}

void Game::UpdateGame()
{
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();

    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    if (mPaddle1Dir != 0)
    {
        mPaddle1Pos.y += mPaddle1Dir * PADDLE_MOVE_SPEED * deltaTime;
        // Make sure paddle doesn't move off screen!
        if (mPaddle1Pos.y < (PADDLE_H/2.0f + thickness))
        {
            mPaddle1Pos.y = PADDLE_H/2.0f + thickness;
        }
        else if (mPaddle1Pos.y > (WINDOW_H - PADDLE_H/2.0f - thickness))
        {
            mPaddle1Pos.y = WINDOW_H - PADDLE_H/2.0f - thickness;
        }
    }
    if (mPaddle2Dir != 0)
    {
        mPaddle2Pos.y += mPaddle2Dir * PADDLE_MOVE_SPEED * deltaTime;
        // Make sure paddle doesn't move off screen!
        if (mPaddle2Pos.y < (PADDLE_H/2.0f + thickness))
        {
            mPaddle2Pos.y = PADDLE_H/2.0f + thickness;
        }
        else if (mPaddle2Pos.y > (WINDOW_H - PADDLE_H/2.0f - thickness))
        {
            mPaddle2Pos.y = WINDOW_H - PADDLE_H/2.0f - thickness;
        }
    }

    for ( auto &ball : mBalls )
    {
        auto x = ball.pos.x;
        auto y = ball.pos.y;
        x += ball.vel.x * deltaTime;
        y += ball.vel.y * deltaTime;
        //  perform collision detection to determine if the ball collides with a wall.
        if (y < (thickness + thickness / 2))
        {
            y = 2*(thickness + thickness / 2) - y;
            ball.vel.y *= (-1);
        }

        if (y > (WINDOW_H - thickness - thickness / 2))
        {
            y = 2*(WINDOW_H - thickness - thickness / 2) - y;
            ball.vel.y *= (-1);
        }

        if (x < (thickness + thickness / 2))
        {
            if ((y + thickness / 2) > (mPaddle1Pos.y - PADDLE_H /2)
            && (y - thickness / 2) < (mPaddle1Pos.y + PADDLE_H /2)
            ){
                x = 2*(thickness + thickness / 2) - x;
                ball.vel.x *= (-1);
            }
            else
            {
                if (--mAttempts > 0)
                { 
                    ball.pos.x = WINDOW_W / 2;
                    ball.pos.y = WINDOW_H / 2;
                }
                return;
            }
        }
        if (x > (WINDOW_W - thickness - thickness / 2))
        {
            if ((y + thickness / 2) > (mPaddle2Pos.y - PADDLE_H /2) 
            && (y - thickness / 2) < (mPaddle2Pos.y + PADDLE_H /2)
            ){
                x = 2*(WINDOW_W - thickness - thickness / 2) - x;
                ball.vel.x *= (-1);
            }
            else
            {
                if (--mAttempts > 0)
                { 
                    ball.pos.x = WINDOW_W / 2;
                    ball.pos.y = WINDOW_H / 2;
                }
                return;
            }
        }
        
        ball.pos.x = x;
        ball.pos.y = y;
    }
}

void Game::GenerateOutput()
{
    if (mRenderer == nullptr) return;

    SDL_SetRenderDrawColor(        // set the color as blue with 100 % opacity
        mRenderer,
        0, // R
        0, // G
        255, // B
        255 // A
    );
    SDL_RenderClear(mRenderer);    //To clear the back buffer

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);    // change the draw color to white
    
    // draw top wall
    SDL_Rect wall{
        0, // Top left x
        0, // Top left y
        WINDOW_W, // Width
        thickness // Height
    };
    SDL_RenderFillRect(mRenderer, &wall);

    // draw bottom wall
    wall.y = WINDOW_H - thickness;        // Hight of window - thickless
    SDL_RenderFillRect(mRenderer, &wall);

    SDL_Rect ball;
    for ( const auto &iter_ball : mBalls )
    {
        ball.x = static_cast<int>(iter_ball.pos.x - thickness / 2);
        ball.y = static_cast<int>(iter_ball.pos.y - thickness / 2);
        ball.h = thickness;
        ball.w = thickness;
        SDL_RenderFillRect(mRenderer, &ball);
    }

    // draw left paddle
    SDL_Rect paddle{
        static_cast<int>(mPaddle1Pos.x - thickness / 2),
        static_cast<int>(mPaddle1Pos.y - PADDLE_H / 2),
        thickness,
        PADDLE_H
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    // draw right paddle
    paddle.x = static_cast<int>(mPaddle2Pos.x - thickness / 2);
    paddle.y = static_cast<int>(mPaddle2Pos.y - PADDLE_H / 2);
    paddle.h = PADDLE_H;
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_RenderPresent(mRenderer);    //swap the front and back buffers


}
