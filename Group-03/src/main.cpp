/**
 * @file main.cpp
 * @author jiang
 */
#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Include your headers
#include "Circle.h"
#include "Surface.h"
// ... you can include DataTracker, FixedPoint, FunctionSet, etc. if you want to use them

// ---------------------------------------------------------------------------
// Globals (avoid big global usage in real apps, but for demo it's simpler)

static const int WINDOW_WIDTH  = 800;
static const int WINDOW_HEIGHT = 600;

// We will have an SDL_Window and SDL_Renderer
static SDL_Window*   gWindow   = nullptr;
static SDL_Renderer* gRenderer = nullptr;

// We keep a global Surface.
// We'll use a "sector_size" of 100, just as an example.
static cse::Surface* gSurface  = nullptr;

// We'll track circles separately as well (though we store them in gSurface, too)
static std::vector<Circle*> gCircles;

// Simulation parameters
static bool isRunning = true;

// ---------------------------------------------------------------------------
// Create some circles in random positions for demonstration
void createDemoCircles()
{
    std::srand((unsigned)std::time(nullptr));

    // For example: Make 10 random circles
    for(int i=0; i<10; i++)
    {
        double x      = rand() % WINDOW_WIDTH;
        double y      = rand() % WINDOW_HEIGHT;
        double radius = 15.0;             // example radius
        double baseSp = 3.0;             // base speed
        double speed  = 3.0;             // current speed
        std::string circleType = (i % 2 == 0) ? "red" : "blue";

        // Using your Circle constructor
        Circle* c = new Circle(x, y, radius, baseSp, speed, circleType);

        // Add to global vector
        gCircles.push_back(c);

        // Also add to our surface's sector structure
        gSurface->add_circle(c);
    }
}

// ---------------------------------------------------------------------------
// Called every frame to update the positions
void update()
{
    // Example: Move all circles randomly by a small amount
    // and let the Surface’s collision detection do its check
    for(auto* circle : gCircles)
    {
        if (!circle) continue;

        // Basic random movement
        double newX = circle->getX() + ((rand() % 3) - 1) * circle->getSpeed();
        double newY = circle->getY() + ((rand() % 3) - 1) * circle->getSpeed();

        // Clamp them within the screen bounds
        if(newX < 0)           newX = 0;
        if(newX > WINDOW_WIDTH - circle->getRadius())
            newX = WINDOW_WIDTH - circle->getRadius();

        if(newY < 0)           newY = 0;
        if(newY > WINDOW_HEIGHT - circle->getRadius())
            newY = WINDOW_HEIGHT - circle->getRadius();

        // Move the circle in the surface
        gSurface->move_circle(circle, (int)newX, (int)newY);
    }

    // Now let the Surface do whatever internal updates it needs
    // (for example reassigning circles to new sectors if they move)
    gSurface->update();

    // Optionally check collisions (purely for demonstration)
    gSurface->check_collision();
}

// ---------------------------------------------------------------------------
// Called every frame to draw the current state
void draw()
{
    // Clear the screen
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // black background
    SDL_RenderClear(gRenderer);

    // Draw each circle
    for (auto* circle : gCircles)
    {
        if (!circle) continue;

        // Choose color based on circleType (just as an example)
        if(circle->getCircleType() == "red") {
            SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        }

        // We’ll draw a filled circle (quick brute-force method)
        int cx = (int)circle->getX();
        int cy = (int)circle->getY();
        int r  = (int)circle->getRadius();

        // A naive circle-draw
        for(int w = -r; w <= r; w++)
        {
            for(int h = -r; h <= r; h++)
            {
                if ((w*w + h*h) <= (r*r))
                {
                    SDL_RenderDrawPoint(gRenderer, cx + w, cy + h);
                }
            }
        }
    }

    // Present the result
    SDL_RenderPresent(gRenderer);
}

// ---------------------------------------------------------------------------
// This is the function that Emscripten calls each "frame"
void mainLoop()
{
    // For SDL events like quitting or key presses
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT) {
            isRunning = false;
        }
    }

    if(!isRunning) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    // Typical game loop steps
    update();
    draw();
}

// ---------------------------------------------------------------------------
// The standard main function
int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return 1;
    }

    // Create SDL window & renderer
    gWindow = SDL_CreateWindow(
        "Emscripten Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if(!gWindow)
    {
        std::cerr << "Window could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return 1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if(!gRenderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: "
                  << SDL_GetError() << std::endl;
        return 1;
    }

    // Create our cse::Surface
    // Suppose we break the screen into 100px sectors:
    gSurface = new cse::Surface(WINDOW_WIDTH, WINDOW_HEIGHT, /*sector_size=*/100);

    // Create some circles just for demo
    createDemoCircles();

#ifdef __EMSCRIPTEN__
    // On the web, Emscripten will drive our main loop at ~60 fps
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    // On native, we just manually loop until user quits
    while(isRunning)
    {
        mainLoop();
        SDL_Delay(16); // ~60 fps
    }
#endif

    // Cleanup
    for (auto* c : gCircles) {
        delete c;
    }
    gCircles.clear();

    delete gSurface;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}


