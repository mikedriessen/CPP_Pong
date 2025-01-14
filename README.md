### GitHub Repository Description

# Pong Game in SDL2

This repository contains a classic **Pong game** built using **SDL2** and **SDL_ttf** libraries. The game features simple gameplay mechanics with two paddles and a ball, offering a nostalgic experience for players.

## Features
- **Smooth Gameplay**: Runs at 60 FPS with paddle and ball physics.
- **Score Tracking**: Displays player scores using rendered text.
- **Keyboard Controls**:
  - Player 1: `W` (up), `S` (down)
  - Player 2: `Arrow Up` (up), `Arrow Down` (down)
- **Responsive Design**: Ensures paddles and ball stay within screen boundaries.
- **Modular Code**: Easy to read, extend, and modify.

## Requirements
- SDL2
- SDL_ttf
- C++ Compiler (e.g., GCC, MSVC)

## How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/pong-game-sdl2.git
   ```
2. Build the project:
   ```bash
   g++ -o PongGame main.cpp -lSDL2 -lSDL2_ttf
   ```
3. Run the executable:
   ```bash
   ./PongGame
   ```

## Customization
- Modify screen dimensions, paddle sizes, and speeds via constants:
  ```cpp
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 600;
  const int PADDLE_SPEED = 300;
  const int BALL_SPEED = 300;
  ```

## Screenshot
![Pong](https://github.com/user-attachments/assets/4e07c767-b0bf-43a4-bdb0-123aa5a7b414)


## License
This project is licensed under the MIT License. See `LICENSE` for details.
