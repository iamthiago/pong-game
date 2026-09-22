//
// Created by Thiago Pereira on 2026-09-18.
//
#include "Game.h"
#include <numbers>
#include <random>

constexpr int SCREEN_HEIGHT = 768;
constexpr int SCREEN_WIDTH = 1024;

constexpr int THICKNESS = 15;
constexpr int PADDLE_HEIGHT = 100;

constexpr int NUM_BALLS = 5;
constexpr float BALL_SPEED = 250.0f;

Game::Game()
    : mWindow(nullptr)
      , mRenderer(nullptr)
      , mIsRunning(true)
      , mTicksCount(0)
      , mLeftPaddleDir(0)
      , mLeftPaddlePos()
      , mRightPaddleDir(0)
      , mRightPaddlePos()
      , mGen(std::random_device{}()){
}

bool Game::Initialize() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)",
        100,
        100,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1, // default
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    mLeftPaddlePos = Vector2{THICKNESS * 2, SCREEN_HEIGHT/2.0f};
    mRightPaddlePos = Vector2{SCREEN_WIDTH - (THICKNESS * 2), SCREEN_HEIGHT/2.0f};

    std::uniform_real_distribution<float> coneDist(-std::numbers::pi / 4.0f, std::numbers::pi / 4.0f);
    std::bernoulli_distribution goLeft(0.5);

    mBalls.reserve(NUM_BALLS);

    for (int i = 0; i < NUM_BALLS; i++) {
        float angle = coneDist(mGen);
        float dir = goLeft(mGen) ? -1.0f : 1.0f;
        auto b = Ball{
            Vector2{SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f},
            Vector2{dir * BALL_SPEED * std::cos(angle), BALL_SPEED * std::sin(angle)}
        };
        mBalls.push_back(b);
    }

    return true;
}

void Game::ProcessInput() {
    SDL_Event event;
    // while there are still events in the queue
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    // Get state of the keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    mLeftPaddleDir = 0;
    if (state[SDL_SCANCODE_W]) {
        mLeftPaddleDir -= 1;
    }

    if (state[SDL_SCANCODE_S]) {
        mLeftPaddleDir += 1;
    }

    mRightPaddleDir = 0;
    if (state[SDL_SCANCODE_I]) {
        mRightPaddleDir -= 1;
    }

    if (state[SDL_SCANCODE_K]) {
        mRightPaddleDir += 1;
    }
}

void Game::UpdateGame() {
    // wait until 16ms has elapse since the last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // Limit maximum delta time value
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();

    // if paddle has moved
    if (mLeftPaddleDir != 0) {
        mLeftPaddlePos.y += mLeftPaddleDir * 300.0f * deltaTime;

        // Make sure it does not move off screen
        if (mLeftPaddlePos.y < (PADDLE_HEIGHT/2.0f + THICKNESS)) {
            mLeftPaddlePos.y = PADDLE_HEIGHT/2.0f + THICKNESS;
        }
        else if (mLeftPaddlePos.y > (SCREEN_HEIGHT - PADDLE_HEIGHT/2.0f - THICKNESS)) {
            mLeftPaddlePos.y = SCREEN_HEIGHT - PADDLE_HEIGHT/2.0f - THICKNESS;
        }
    }

    // if right paddle has moved
    if (mRightPaddleDir != 0) {
        mRightPaddlePos.y += mRightPaddleDir * 300.0f * deltaTime;

        // Make sure it does not move off screen
        if (mRightPaddlePos.y < (PADDLE_HEIGHT/2.0f + THICKNESS)) {
            mRightPaddlePos.y = PADDLE_HEIGHT/2.0f + THICKNESS;
        }
        else if (mRightPaddlePos.y > (SCREEN_HEIGHT - PADDLE_HEIGHT/2.0f - THICKNESS)) {
            mRightPaddlePos.y = SCREEN_HEIGHT - PADDLE_HEIGHT/2.0f - THICKNESS;
        }
    }

    for (auto& b : mBalls) {
        // Update ball positioning based on bal velocity
        b.pos.x += b.vel.x * deltaTime;
        b.pos.y += b.vel.y * deltaTime;

        // Collision with top wall
        // less than 0 means the ball is moving upwards
        if (b.pos.y <= THICKNESS && b.vel.y < 0.0f) {
            b.vel.y *= -1.0f;
        }

        // Collision with bottom wall
        if (b.pos.y >= SCREEN_HEIGHT - THICKNESS && b.vel.y > 0.0f) {
            b.vel.y *= -1.0f;
        }

        // left and right paddle collision
        float leftDiff = std::fabs(b.pos.y - mLeftPaddlePos.y);
        float rightDiff = std::fabs(b.pos.y - mRightPaddlePos.y);

        if (
            // Our y-difference is small enough
            leftDiff <= PADDLE_HEIGHT / 2.0f &&
            // Ball is at the correct x-position (left of the screen)
            b.pos.x <= 45.0f && b.pos.x >= 40.0f &&
            // Ball is moving to the left
            b.vel.x < 0.0f
        ) {
            b.vel.x *= -1.0f;
        }
        else if (
            rightDiff <= PADDLE_HEIGHT / 2.0f &&
            (b.pos.x <= SCREEN_WIDTH - 40.0f) && (b.pos.x >= SCREEN_WIDTH - 45.0f) &&
            b.vel.x > 0.0f
        ) {
            b.vel.x *= -1.0f;
        }
    }

    // remove the ball from vector if it goes off screen
    std::erase_if(mBalls, [](const Ball& b) {
       return b.pos.x < 0.0f || b.pos.x > SCREEN_WIDTH;
    });

    if (mBalls.empty()) {
        mIsRunning = false;
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(
        mRenderer,
        0,      // Red
        0,      // Green
        255,    // Blue
        255     // Alpha (transparency)
    );

    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(
        mRenderer,
        255,
        255,
        255,
        255
    );

    SDL_Rect topWall {
        0,
        0,
        SCREEN_WIDTH,
        THICKNESS
    };

    SDL_Rect bottomWall {
        0,
        SCREEN_HEIGHT - THICKNESS,
        SCREEN_WIDTH,
        THICKNESS
    };

    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_RenderFillRect(mRenderer, &bottomWall);

    for (const auto& b : mBalls) {
        SDL_Rect ball {
            static_cast<int>(b.pos.x - THICKNESS/2.0f),
            static_cast<int>(b.pos.y - THICKNESS/2.0f),
            THICKNESS,
            THICKNESS
        };

        SDL_RenderFillRect(mRenderer, &ball);
    }

    SDL_Rect leftPaddle {
        static_cast<int>(mLeftPaddlePos.x - THICKNESS/2.0f),
        static_cast<int>(mLeftPaddlePos.y - PADDLE_HEIGHT/2.0f),
        THICKNESS,
        PADDLE_HEIGHT
    };

    SDL_Rect rightPaddle {
        static_cast<int>(mRightPaddlePos.x - THICKNESS/2.0f),
        static_cast<int>(mRightPaddlePos.y - PADDLE_HEIGHT/2.0f),
        THICKNESS,
        PADDLE_HEIGHT
    };

    SDL_RenderFillRect(mRenderer, &leftPaddle);
    SDL_RenderFillRect(mRenderer, &rightPaddle);

    SDL_RenderPresent(mRenderer);
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
