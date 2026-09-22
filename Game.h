//
// Created by Thiago Pereira on 2026-09-18.
//

#ifndef PONG_GAME_GAME_H
#define PONG_GAME_GAME_H
#include <random>
#include <SDL.h>
#include <vector>

struct Vector2 {
    float x;
    float y;
};

struct Ball {
    Vector2 pos;
    Vector2 vel;
};

class Game {
    public:
        Game();
        // Initialize the game
        bool Initialize();
        // Runs the game loop until the game is over
        void RunLoop();
        // Shutdown the game
        void Shutdown();
    private:
        // Helper functions for the game loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();

        // Window created by SDL
        SDL_Window* mWindow;
        // Renderer to draw graphics
        SDL_Renderer* mRenderer;
        // Game should continue to run
        bool mIsRunning;
        // Number of ticks since start of the game
        Uint32 mTicksCount;

        int mLeftPaddleDir;
        Vector2 mLeftPaddlePos;

        int mRightPaddleDir;
        Vector2 mRightPaddlePos;

        std::vector<Ball> mBalls;

        std::mt19937 mGen;
};

#endif //PONG_GAME_GAME_H