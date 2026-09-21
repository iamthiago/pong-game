//
// Created by Thiago Pereira on 2026-09-18.
//

#ifndef PONG_GAME_GAME_H
#define PONG_GAME_GAME_H
#include <SDL.h>

struct Vector2 {
    float x;
    float y;
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

        Vector2 mBallPos;
        Vector2 mBallVel;
};

#endif //PONG_GAME_GAME_H