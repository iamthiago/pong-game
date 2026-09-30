# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository. The shared teaching and build guidance for all book projects is in `../CLAUDE.md`.

## Chapter 1: "Game Programming Overview"

Pong. Executable: `./cmake-build-debug/pong_game`. Links SDL2 only (no SDL_image).

The code follows the book's Chapter 1 `Game` class: `Initialize` / `RunLoop` / `Shutdown`, and a loop of `ProcessInput` → `UpdateGame` → `GenerateOutput`. The class is declared in `Game.h`, defined in `Game.cpp` and driven from `main.cpp`.
