#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Paddle dimensions
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;
const int BALL_SIZE = 15;

// Speed settings
const int PADDLE_SPEED = 300;
const int BALL_SPEED = 300;

class PongGame {
public:
    PongGame()
        : leftPaddle{ 50, (SCREEN_HEIGHT / 2) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT },
        rightPaddle{ SCREEN_WIDTH - 50 - PADDLE_WIDTH, (SCREEN_HEIGHT / 2) - (PADDLE_HEIGHT / 2), PADDLE_WIDTH, PADDLE_HEIGHT },
        ball{ (SCREEN_WIDTH / 2) - (BALL_SIZE / 2), (SCREEN_HEIGHT / 2) - (BALL_SIZE / 2), BALL_SIZE, BALL_SIZE } {
    }

    bool Initialize();
    void Run();
    void Cleanup();

private:
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    void ResetBall();
    void RenderText(const std::string& text, int x, int y);

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    bool isRunning = true;

    SDL_Rect leftPaddle;
    SDL_Rect rightPaddle;
    SDL_Rect ball;

    int ballVelX = BALL_SPEED;
    int ballVelY = BALL_SPEED;

    int leftPaddleVel = 0;
    int rightPaddleVel = 0;

    int leftScore = 0;
    int rightScore = 0;
};

bool PongGame::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf Initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("Nexa-Heavy.ttf", 24);
    if (!font) {
        std::cerr << "Font loading failed: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void PongGame::Run() {
    const int FPS = 60;
    const float deltaTime = 1.0f / FPS;

    while (isRunning) {
        HandleEvents();
        Update(deltaTime);
        Render();
        SDL_Delay(1000 / FPS);
    }
}

void PongGame::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_w: leftPaddleVel = -PADDLE_SPEED; break;
            case SDLK_s: leftPaddleVel = PADDLE_SPEED; break;
            case SDLK_UP: rightPaddleVel = -PADDLE_SPEED; break;
            case SDLK_DOWN: rightPaddleVel = PADDLE_SPEED; break;
            }
        }
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_w:
            case SDLK_s: leftPaddleVel = 0; break;
            case SDLK_UP:
            case SDLK_DOWN: rightPaddleVel = 0; break;
            }
        }
    }
}

void PongGame::Update(float deltaTime) {
    // Update paddle positions
    leftPaddle.y += static_cast<int>(leftPaddleVel * deltaTime);
    rightPaddle.y += static_cast<int>(rightPaddleVel * deltaTime);

    // Clamp paddles to stay on screen
    leftPaddle.y = SDL_clamp(leftPaddle.y, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
    rightPaddle.y = SDL_clamp(rightPaddle.y, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);

    // Update ball position
    ball.x += static_cast<int>(ballVelX * deltaTime);
    ball.y += static_cast<int>(ballVelY * deltaTime);

    // Ball collision with top and bottom walls
    if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - BALL_SIZE) {
        ballVelY = -ballVelY;
    }

    // Ball collision with paddles
    if (SDL_HasIntersection(&ball, &leftPaddle)) {
        if (ball.x < leftPaddle.x + PADDLE_WIDTH) {
            ball.x = leftPaddle.x + PADDLE_WIDTH; // Adjust position
            ballVelX = -ballVelX;                // Bounce back
        }
        else {
            rightScore++;
            ResetBall();
        }
    }
    else if (SDL_HasIntersection(&ball, &rightPaddle)) {
        if (ball.x + BALL_SIZE > rightPaddle.x) {
            ball.x = rightPaddle.x - BALL_SIZE; // Adjust position
            ballVelX = -ballVelX;               // Bounce back
        }
        else {
            leftScore++;
            ResetBall();
        }
    }

    // Check for scoring conditions
    if (ball.x <= 0) {
        rightScore++;
        ResetBall();
    }
    else if (ball.x >= SCREEN_WIDTH - BALL_SIZE) {
        leftScore++;
        ResetBall();
    }
}

void PongGame::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &leftPaddle);
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &ball);

    RenderText(std::to_string(leftScore), SCREEN_WIDTH / 4, 20);
    RenderText(std::to_string(rightScore), 3 * SCREEN_WIDTH / 4, 20);

    SDL_RenderPresent(renderer);
}

void PongGame::RenderText(const std::string& text, int x, int y) {
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void PongGame::ResetBall() {
    ball = { (SCREEN_WIDTH / 2) - (BALL_SIZE / 2), (SCREEN_HEIGHT / 2) - (BALL_SIZE / 2), BALL_SIZE, BALL_SIZE };
    ballVelX = BALL_SPEED * (ballVelX > 0 ? -1 : 1);
}

void PongGame::Cleanup() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    PongGame game;
    if (!game.Initialize()) {
        std::cerr << "Failed to initialize the game." << std::endl;
        return -1;
    }

    game.Run();
    game.Cleanup();
    return 0;
}
