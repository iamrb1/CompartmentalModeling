#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Our headers
#include "Circle.h"
#include "Surface.h"

///////////////////////////////////////////////////////////////////////////////
// Globals

static const int WINDOW_WIDTH  = 800;
static const int WINDOW_HEIGHT = 600;

static SDL_Window*   gWindow   = nullptr;
static SDL_Renderer* gRenderer = nullptr;

// We'll keep a global Surface
static std::unique_ptr<cse::Surface> gSurface;

// We store circles in a shared_ptr vector
static std::vector<std::shared_ptr<Circle>> gCircles;

// For main loop
static bool isRunning = true;

///////////////////////////////////////////////////////////////////////////////
// Create circles at random positions

void createDemoCircles()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Make 10 random circles
    for(int i = 0; i < 10; ++i) {
        double x      = rand() % WINDOW_WIDTH;
        double y      = rand() % WINDOW_HEIGHT;
        double radius = 15.0;
        double baseSp = 3.0;
        double speed  = 3.0;
        std::string circleType = (i % 2 == 0) ? "red" : "blue";

        auto circle = std::make_shared<Circle>(x, y, radius, baseSp, speed, circleType);
        gCircles.push_back(circle);

        gSurface->add_circle(circle);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update

void update()
{
    // Randomly move circles and let surface reassign them
    for (auto& circle : gCircles) {
        if (!circle) continue;

        double newX = circle->getX() + ((rand() % 3) - 1) * circle->getSpeed();
        double newY = circle->getY() + ((rand() % 3) - 1) * circle->getSpeed();

        // Clamp to screen
        if (newX < 0) newX = 0;
        if (newX > WINDOW_WIDTH - circle->getRadius())
            newX = WINDOW_WIDTH - circle->getRadius();

        if (newY < 0) newY = 0;
        if (newY > WINDOW_HEIGHT - circle->getRadius())
            newY = WINDOW_HEIGHT - circle->getRadius();

        gSurface->move_circle(circle, static_cast<int>(newX), static_cast<int>(newY));
    }

    // Let Surface do internal updates
    gSurface->update();

    // Optionally check collisions
    gSurface->check_collision();
}

///////////////////////////////////////////////////////////////////////////////
// Draw

void draw()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // black bg
    SDL_RenderClear(gRenderer);

    // Draw each circle
    for (auto& circle : gCircles) {
        if (!circle) continue;

        // color by type
        if (circle->getCircleType() == "red") {
            SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        }

        int cx = static_cast<int>(circle->getX());
        int cy = static_cast<int>(circle->getY());
        int r  = static_cast<int>(circle->getRadius());

        // naive filled circle
        for(int w = -r; w <= r; w++) {
            for(int h = -r; h <= r; h++) {
                if ((w*w + h*h) <= r*r) {
                    SDL_RenderDrawPoint(gRenderer, cx + w, cy + h);
                }
            }
        }
    }

    SDL_RenderPresent(gRenderer);
}

///////////////////////////////////////////////////////////////////////////////
// Main loop (called by emscripten)

void mainLoop()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
    if (!isRunning) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    update();
    draw();
}

///////////////////////////////////////////////////////////////////////////////
// main()

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not init! " << SDL_GetError() << std::endl;
        return 1;
    }

    gWindow = SDL_CreateWindow("Emscripten Demo",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH, WINDOW_HEIGHT,
                               SDL_WINDOW_SHOWN);
    if (!gWindow) {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        return 1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if (!gRenderer) {
        std::cerr << "Renderer could not be created: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create our Surface (with 100px sector size for demo)
    gSurface = std::make_unique<cse::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT, 100);

    // Create random circles
    createDemoCircles();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    // Native
    while (isRunning) {
        mainLoop();
        SDL_Delay(16); // ~60 fps
    }
#endif

    // Cleanup automatically (shared_ptr and unique_ptr do the job)
    gCircles.clear();
    gSurface.reset();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}
