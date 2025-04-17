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
#include <algorithm>

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
    // Circle types
    double cType1 = 0.0;
    double cType2 = 0.0;

    // 1) Move every circle
    for (auto& circle : gCircles) {
        if (!circle) continue;


        // pick a random direction delta ∈ { -1,0,+1 }
        double dirX = (rand() % 3) - 1;
        double dirY = (rand() % 3) - 1;

        // compute new position in double precision
        double newX = circle->getX() + dirX * circle->getSpeed();
        double newY = circle->getY() + dirY * circle->getSpeed();

        // clamp to window
        newX = std::clamp(newX, 0.0, WINDOW_WIDTH  - circle->getRadius());
        newY = std::clamp(newY, 0.0, WINDOW_HEIGHT - circle->getRadius());

        // now actually move it in the Surface (takes doubles)
        gSurface->move_circle(circle, newX, newY);

        // update circle type counter
        if (circle->getCircleType() == "red") {
            cType1 += 1;
        }
        if (circle->getCircleType() == "blue") {
            cType2 += 1;
        }
    }

    // 2) Let the Surface rebucket anything that spilled sectors
    gSurface->update();

    // 3) Handle the first collision (if any)
    auto [action, victim] = gSurface->check_collision();

    if (action == "delete" && victim) {
        // remove from master list
        gCircles.erase(
            std::remove(gCircles.begin(), gCircles.end(), victim),
            gCircles.end()
        );
        // remove from its sector
        gSurface->remove_circle(victim);
    }
    else if (action == "add" && victim) {
        static Uint32 lastAddTime = 0;
        static const Uint32 addCooldown = 1000;
        // Only add if the type is blue
        if (victim->getCircleType() == "blue") {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastAddTime >= addCooldown) {
                // clone parameters
                double x  = victim->getX();
                double y  = victim->getY();
                double r  = victim->getRadius();
                double bs = victim->getBaseSpeed();
                double sp = victim->getSpeed();
                auto t    = victim->getCircleType();
    
                auto baby = std::make_shared<Circle>(x, y, r, bs, sp, t);
                gCircles.push_back(baby);
                gSurface->add_circle(baby);
    
                lastAddTime = currentTime; // update the timer
            }
        }
    }
    
    // else "nothing" → do nothing
    // @TODO this is very basic and can be updated later
    // End game if conditions are met
    double cWinThreshold = 0.8;
    double cPercentage1 = cType1 / (cType1 + cType2);
    double cPercentage2 = cType2 / (cType1 + cType2);

    // std::cout << "Red #: " << cType1 << "; Blue #: " << cType2 << std::endl;
    // std::cout << "Red %: " << cPercentage1 << "; Blue %: " << cPercentage2 << std::endl;

    if (cPercentage1 >= cWinThreshold) {
        std::cout << "Winner: Red" << std::endl;
        isRunning = false;
    }

    if (cPercentage2 >= cWinThreshold) {
        std::cout << "Winner: Blue" << std::endl;
        isRunning = false;
    }
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
