//
// Created by Thiago Pereira on 2026-09-18.
//
#include "Game.h"

constexpr int SCREEN_HEIGHT = 768;
constexpr int SCREEN_WIDTH = 1024;

constexpr int THICKNESS = 15;
constexpr int PADDLE_HEIGHT = 100;

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

    mBallPos = Vector2{SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f};
    mPaddlePos = Vector2{THICKNESS * 2, SCREEN_HEIGHT/2.0f};

    return true;
}

void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
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
}

void Game::UpdateGame() {
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

    SDL_Rect rightWall {
        SCREEN_WIDTH - THICKNESS,
        0,
        THICKNESS,
        SCREEN_HEIGHT
    };

    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_RenderFillRect(mRenderer, &bottomWall);
    SDL_RenderFillRect(mRenderer, &rightWall);

    SDL_Rect ball {
        static_cast<int>(mBallPos.x - THICKNESS/2),
        static_cast<int>(mBallPos.y - THICKNESS/2),
        THICKNESS,
        THICKNESS
    };

    SDL_Rect paddle {
        static_cast<int>(mPaddlePos.x - THICKNESS/2),
        static_cast<int>(mPaddlePos.y - PADDLE_HEIGHT/2),
        THICKNESS,
        PADDLE_HEIGHT
    };

    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_RenderPresent(mRenderer);
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

Game::Game() = default;
