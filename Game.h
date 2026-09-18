//
// Created by Thiago Pereira on 2026-09-18.
//

#ifndef SIMPLE_GAME_GAME_H
#define SIMPLE_GAME_GAME_H
#include <SDL.h>

#endif //SIMPLE_GAME_GAME_H

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
        SDL_Window* mWindow = nullptr;
        // Game should continue to run
        bool mIsRunning = true;
        // Renderer to draw graphics
        SDL_Renderer* mRenderer;
};