// main.cpp – draws "PREDATORS NN" and "PREY NN" using a built-in 3x5 bitmap font (no SDL_ttf)

// Native build:
//   g++ main.cpp Circle.cpp Surface.cpp -lSDL2 -std=c++17 -O2 -o circles
// Emscripten build:
//   emcc main.cpp Circle.cpp Surface.cpp -sUSE_SDL=2 -std=c++17 -O2 -o circles.html

#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include "Circle.h"
#include "Surface.h"

namespace cse {

// Globals
static constexpr int WINDOW_WIDTH  = 800;
static constexpr int WINDOW_HEIGHT = 600;
static constexpr double WIN_THRESHOLD = 0.8;

static SDL_Window*   gWindow   = nullptr;
static SDL_Renderer* gRenderer = nullptr;

static std::unique_ptr<cse::Surface>      gSurface;
static std::vector<std::shared_ptr<Circle>> gCircles;

static bool isRunning = true;

// 3x5 bitmap font 
static constexpr int FONT_SCALE   = 4;
static constexpr int CHAR_W       = 3 * FONT_SCALE;
static constexpr int CHAR_H       = 5 * FONT_SCALE;
static constexpr int CHAR_SPACING = FONT_SCALE;

// Digits
static const uint8_t DIGIT[10][5] = {//chatgpt
 {0b111,0b101,0b101,0b101,0b111},  // 0
 {0b010,0b110,0b010,0b010,0b111},  // 1
 {0b111,0b001,0b111,0b100,0b111},  // 2
 {0b111,0b001,0b111,0b001,0b111},  // 3
 {0b101,0b101,0b111,0b001,0b001},  // 4
 {0b111,0b100,0b111,0b001,0b111},  // 5
 {0b111,0b100,0b111,0b101,0b111},  // 6
 {0b111,0b001,0b010,0b010,0b010},  // 7
 {0b111,0b101,0b111,0b101,0b111},  // 8
 {0b111,0b101,0b111,0b001,0b111}   // 9
};

// Letters A, D, E, O, P, R, S, T, Y 
static const uint8_t LETTER[26][5] = {//chatgpt
{0b111,0b101,0b111,0b101,0b101}, // A
{0}, {0}, {0b110,0b101,0b101,0b101,0b110}, // D
{0b111,0b100,0b111,0b100,0b111}, // E
{0},{0},{0}, {0},{0},{0},{0},{0},{0},
{0b111,0b101,0b101,0b101,0b111}, // O
{0b111,0b101,0b111,0b100,0b100}, // P
{0}, {0b111,0b101,0b111,0b101,0b101}, // R
{0b111,0b100,0b111,0b001,0b111}, // S
{0b111,0b010,0b010,0b010,0b010}, // T
{0},{0},{0},{0},
{0b101,0b101,0b010,0b010,0b010}, // Y
{0}
};

// Count circles of a given type
int countCircles(CircleType type)
{
    return std::count_if(gCircles.begin(), gCircles.end(),
                         [=](const auto& c){
                             return c && c->getType() == type;
                         });
}

// Draw a digit or letter
void drawGlyph(const uint8_t rows[5], int x, int y, SDL_Color col)
{
    SDL_SetRenderDrawColor(gRenderer, col.r, col.g, col.b, 255);
    for (int r = 0; r < 5; ++r) { //chatgpt
        uint8_t bits = rows[r];
        for (int c = 0; c < 3; ++c) {
            if (bits & (1 << (2 - c))) {
                SDL_Rect px{ x + c*FONT_SCALE,
                             y + r*FONT_SCALE,
                             FONT_SCALE, FONT_SCALE };
                SDL_RenderFillRect(gRenderer, &px);
            }
        }
    }
}

// Draw a digit (0–9)
void drawDigit(int d, int x, int y, SDL_Color col)
{
    if (d < 0 || d > 9) return;
    drawGlyph(DIGIT[d], x, y, col);
}

// Draw an uppercase letter (A–Z) if available
void drawLetter(char ch, int x, int y, SDL_Color col)
{
    if (ch < 'A' || ch > 'Z') return;
    const uint8_t* rows = LETTER[ch - 'A'];
    if (rows[0] == 0) return;
    drawGlyph(rows, x, y, col);
}

// Calculate width in pixels of an n-character string
int textWidth(int len) { return len*CHAR_W + (len-1)*CHAR_SPACING; }

// Draw a string of uppercase letters
void drawString(const std::string& s, int x, int y, SDL_Color col)
{
    int cx = x;
    for (size_t i = 0; i < s.size(); ++i) {
        char ch = std::toupper(static_cast<unsigned char>(s[i]));
        if (ch >= 'A' && ch <= 'Z')
            drawLetter(ch, cx, y, col);
        cx += CHAR_W + CHAR_SPACING;
    }
}

// Draw a number (0–99), left-aligned
void drawNumber(int n, int x, int y, SDL_Color col)
{
    n = std::clamp(n, 0, 99);
    int tens = n / 10;
    int ones = n % 10;
    if (tens) {
        drawDigit(tens, x, y, col);
        x += CHAR_W + CHAR_SPACING;
    }
    drawDigit(ones, x, y, col);
}

// Create 10 demo circles (half red, half blue)
void createDemoCircles()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int i = 0; i < 10; ++i) {
        double x = rand() % WINDOW_WIDTH;
        double y = rand() % WINDOW_HEIGHT;
        double r = 15.0;
        double s = 3.0;
        CircleType type = (i % 2 == 0)
            ? CircleType::Predator
            : CircleType::Prey;
        auto c = std::make_shared<Circle>(x, y, r, s, type);
        gCircles.push_back(c);
        gSurface->add_circle(c);
    }
}

// Update all circles and game state
void update()
{

    for (auto& circle : gCircles) {
        if (!circle) continue;

        if (circle->getType() == CircleType::Predator) {
            // if regenerating, let it tick up and skip movement/energy drain
            if (circle->isRegenerating()) {
                circle->regenerateEnergy(10);
                continue;
            }
            // otherwise drain
            circle->decreaseEnergy(5);
            // note: once energy<=0, decreaseEnergy() automatically
            // puts you into regenerating state, so no manual setResting needed
        }

        // Random movement
        double dirX = (rand() % 3) - 1;
        double dirY = (rand() % 3) - 1;
        double newX = circle->getX() + dirX * circle->getSpeed();
        double newY = circle->getY() + dirY * circle->getSpeed();
        newX = std::clamp(newX, 0.0, WINDOW_WIDTH  - circle->getRadius());
        newY = std::clamp(newY, 0.0, WINDOW_HEIGHT - circle->getRadius());
        gSurface->move_circle(circle, newX, newY);

        // Count types
        if (circle->getType() == CircleType::Predator) cType1 += 1;
        if (circle->getType() == CircleType::Prey)    cType2 += 1;
    }

    gSurface->update();

    // Handle collisions
    auto [act, vic] = gSurface->check_collision();
    if (act == "delete" && vic) {
        gCircles.erase(std::remove(gCircles.begin(), gCircles.end(), vic), gCircles.end());
        gSurface->remove_circle(vic);
    } else if (act == "add" &&
        vic && vic->getType() == CircleType::Prey) {
        static Uint32 last = 0;
        if (SDL_GetTicks() - last >= 1000) {
            auto b = std::make_shared<Circle>(*vic);
            gCircles.push_back(b);
            gSurface->add_circle(b);
            last = SDL_GetTicks();
        }
    }

    int numRed  = countCircles(CircleType::Predator);
    int numBlue = countCircles(CircleType::Prey);
    int total = numRed + numBlue;
    
    if (total > 0) {
        double redPercent = static_cast<double>(numRed) / total;
        double bluePercent = static_cast<double>(numBlue) / total;
    
        if (redPercent >= WIN_THRESHOLD) {
            std::cout << "Winner: Red" << std::endl;
            isRunning = false;
        } else if (bluePercent >= WIN_THRESHOLD) {
            std::cout << "Winner: Blue" << std::endl;
            isRunning = false;
        }
    }
}

// Draw all circles and labels
void draw()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    // Draw circles
    for (auto& c : gCircles) {
        if (!c) continue;
        SDL_SetRenderDrawColor(gRenderer,
                               c->getType() == CircleType::Predator ? 255 : 0,
                               0,
                               c->getType() == CircleType::Prey    ? 255 : 0,
                               255);
        int cx = (int)c->getX(), cy = (int)c->getY(), r = (int)c->getRadius();
        for (int dx = -r; dx <= r; ++dx)
            for (int dy = -r; dy <= r; ++dy)
                if (dx*dx + dy*dy <= r*r)
                    SDL_RenderDrawPoint(gRenderer, cx + dx, cy + dy);
    }

    // Draw labels and counts
    int preds = countCircles(CircleType::Predator);
    int prey  = countCircles(CircleType::Prey);

    SDL_Color white = {255,255,255,255};
    SDL_Color red   = {255,0,0,255};
    SDL_Color blue  = {0,0,255,255};

    std::string labelPred = "PREDATORS";
    std::string labelPrey = "PREY";

    int wPredLabel = textWidth(labelPred.size());
    int nDigitsPred = (preds >= 10) ? 2 : 1;
    int wPredNum = textWidth(nDigitsPred);
    int wTotalPred = wPredLabel + CHAR_SPACING + wPredNum;

    int wPreyLabel = textWidth(labelPrey.size());
    int nDigitsPrey = (prey >= 10) ? 2 : 1;
    int wPreyNum = textWidth(nDigitsPrey);
    int wTotalPrey = wPreyLabel + CHAR_SPACING + wPreyNum;

    int xPredLabel = WINDOW_WIDTH - 8 - wTotalPred;
    int yPred      = 8;

    int xPreyLabel = WINDOW_WIDTH - 8 - wTotalPrey;
    int yPrey      = yPred + CHAR_H + 2 * CHAR_SPACING;

    drawString(labelPred, xPredLabel, yPred, white);
    drawNumber(preds,    xPredLabel + wPredLabel + CHAR_SPACING, yPred, red);

    drawString(labelPrey, xPreyLabel, yPrey, white);
    drawNumber(prey,      xPreyLabel + wPreyLabel + CHAR_SPACING, yPrey, blue);

    SDL_RenderPresent(gRenderer);
}

// Handle events and run main loop
void mainLoop()
{
    SDL_Event e; //chatgpt
    while (SDL_PollEvent(&e))
        if (e.type == SDL_QUIT) isRunning = false;

    if (!isRunning) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }
    update();
    draw();
}

// Entry point
int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    gWindow = SDL_CreateWindow("Predator / Prey demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gWindow) {
        std::cerr << "Window error: " << SDL_GetError() << '\n';
        return 1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) {
        std::cerr << "Renderer error: " << SDL_GetError() << '\n';
        return 1;
    }

    gSurface = std::make_unique<cse::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT, 100);
    createDemoCircles();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (isRunning) {
        mainLoop();
        SDL_Delay(16);
    }
#endif

    gCircles.clear();
    gSurface.reset();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    return 0;
}

}
