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
#include <array>
#include <cctype>
#include <ranges>
#include <string_view>
#include "Circle.h"
#include "Surface.h"


// Globals
static constexpr int WINDOW_WIDTH  = 800;
static constexpr int WINDOW_HEIGHT = 600;
static constexpr double WIN_THRESHOLD = 0.8;

static SDL_Window*   gWindow   = nullptr;
static SDL_Renderer* gRenderer = nullptr;

static std::unique_ptr<cse::Surface>      gSurface;
static std::vector<std::shared_ptr<cse::Circle>> gCircles;

static bool isRunning = true;

// 3x5 bitmap font 
static constexpr int FONT_SCALE   = 4;
static constexpr int CHAR_W       = 3 * FONT_SCALE;
static constexpr int CHAR_H       = 5 * FONT_SCALE;
static constexpr int CHAR_SPACING = FONT_SCALE;

// Digits
constexpr std::array<std::array<uint8_t, 5>, 10> DIGIT = {{
    {{0b111, 0b101, 0b101, 0b101, 0b111}},  // 0
    {{0b010, 0b110, 0b010, 0b010, 0b111}},  // 1
    {{0b111, 0b001, 0b111, 0b100, 0b111}},  // 2
    {{0b111, 0b001, 0b111, 0b001, 0b111}},  // 3
    {{0b101, 0b101, 0b111, 0b001, 0b001}},  // 4
    {{0b111, 0b100, 0b111, 0b001, 0b111}},  // 5
    {{0b111, 0b100, 0b111, 0b101, 0b111}},  // 6
    {{0b111, 0b001, 0b010, 0b010, 0b010}},  // 7
    {{0b111, 0b101, 0b111, 0b101, 0b111}},  // 8
    {{0b111, 0b101, 0b111, 0b001, 0b111}}   // 9
}};

// Used chat for all the letters
constexpr std::array<std::array<uint8_t, 5>, 26> LETTER = {{
    {{0b111,0b101,0b111,0b101,0b101}}, // A
    {{0b110,0b101,0b110,0b101,0b110}}, // B
    {{0b111,0b100,0b100,0b100,0b111}}, // C
    {{0b110,0b101,0b101,0b101,0b110}}, // D
    {{0b111,0b100,0b111,0b100,0b111}}, // E
    {{0b111,0b100,0b111,0b100,0b100}}, // F
    {{0b111,0b100,0b101,0b101,0b111}}, // G
    {{0b101,0b101,0b111,0b101,0b101}}, // H
    {{0b111,0b010,0b010,0b010,0b111}}, // I
    {{0b001,0b001,0b001,0b101,0b111}}, // J
    {{0b101,0b101,0b110,0b101,0b101}}, // K
    {{0b100,0b100,0b100,0b100,0b111}}, // L
    {{0b101,0b111,0b111,0b101,0b101}}, // M
    {{0b101,0b111,0b111,0b111,0b101}}, // N
    {{0b111,0b101,0b101,0b101,0b111}}, // O
    {{0b111,0b101,0b111,0b100,0b100}}, // P
    {{0b111,0b101,0b101,0b111,0b001}}, // Q
    {{0b111,0b101,0b111,0b110,0b101}}, // R
    {{0b111,0b100,0b111,0b001,0b111}}, // S
    {{0b111,0b010,0b010,0b010,0b010}}, // T
    {{0b101,0b101,0b101,0b101,0b111}}, // U
    {{0b101,0b101,0b101,0b101,0b010}}, // V
    {{0b101,0b101,0b111,0b111,0b101}}, // W
    {{0b101,0b101,0b010,0b101,0b101}}, // X
    {{0b101,0b101,0b010,0b010,0b010}}, // Y
    {{0b111,0b001,0b010,0b100,0b111}}  // Z
}};

// Count circles of a given type
int countCircles(cse::CircleType type)
{
    return std::count_if(gCircles.begin(), gCircles.end(),
                         [=](const auto& c){
                             return c && c->getType() == type;
                         });
}

// Draw a digit or letter
void drawGlyph(const std::array<uint8_t, 5>& rows, int x, int y, SDL_Color col)
{
    SDL_SetRenderDrawColor(gRenderer, col.r, col.g, col.b, 255);
    for (int r = 0; r < 5; ++r) {
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

void drawLetter(char ch, int x, int y, SDL_Color col)
{
    if (ch < 'A' || ch > 'Z') return;
    const auto& glyph = LETTER[ch - 'A'];
    if (glyph[0] == 0) return;
    drawGlyph(glyph, x, y, col);
}

// Calculate width in pixels of an n-character string
int textWidth(int len) { return len*CHAR_W + (len-1)*CHAR_SPACING; }

// Draw a string of uppercase letters
void drawString(std::string_view s, int x, int y, SDL_Color col)
{
    constexpr int ADVANCE = CHAR_W + CHAR_SPACING;

    auto upper = std::views::transform([](char c) {
        return static_cast<char>(
            std::toupper(static_cast<unsigned char>(c)));
    });

    int cx = x;
    for (char ch : s | upper)
    {
        if (ch >= 'A' && ch <= 'Z')
            drawLetter(ch, cx, y, col);

        cx += ADVANCE;
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
        cse::CircleType type = (i % 3 == 0)
            ? cse::CircleType::Predator
            : cse::CircleType::Prey;
        auto c = std::make_shared<cse::Circle>(x, y, r, s, type);
        gCircles.push_back(c);
        gSurface->add_circle(c);
    }
}

// Update all circles and game state
#include <algorithm>   // std::ranges::count_if
#include <ranges>      // std::views, std::ranges

void update()
{
  	gCircles.erase(
        std::remove_if(gCircles.begin(), gCircles.end(),
                       [](auto const& p){ return p == nullptr; }),
        gCircles.end());

    for (auto& circle : gCircles) {
        if (!circle) continue;

        if (circle->getType() == cse::CircleType::Predator) {
            if (circle->isRegenerating()) {
                circle->regenerateEnergy(10);
                continue;
            }

            circle->decreaseEnergy(5);
        }

        const double dirX = (rand() % 3) - 1;
        const double dirY = (rand() % 3) - 1;
        double newX = circle->getX() + dirX * circle->getSpeed();
        double newY = circle->getY() + dirY * circle->getSpeed();

        newX = std::clamp(newX, 0.0, WINDOW_WIDTH  - circle->getRadius());
        newY = std::clamp(newY, 0.0, WINDOW_HEIGHT - circle->getRadius());
        gSurface->move_circle(circle, newX, newY);
    }

    gSurface->update();

    if (auto [act, vic] = gSurface->check_collision(); act == "delete" && vic) {
        gCircles.erase(std::remove(gCircles.begin(), gCircles.end(), vic),
                       gCircles.end());
        gSurface->remove_circle(vic);
    }
    else if (act == "add" &&
             vic && vic->getType() == cse::CircleType::Prey)
    {
        static Uint32 last = 0;
        if (SDL_GetTicks() - last >= 1000) {
            auto clone = std::make_shared<cse::Circle>(*vic);
            gCircles.push_back(clone);
            gSurface->add_circle(clone);
            last = SDL_GetTicks();
        }
    }

    auto is_pred = [](auto const& p) {
        return p && p->getType() == cse::CircleType::Predator;
    };
    auto is_prey = [](auto const& p) {
        return p && p->getType() == cse::CircleType::Prey;
    };

    const int numRed  = std::ranges::count_if(gCircles, is_pred);
    const int numBlue = std::ranges::count_if(gCircles, is_prey);
    const int total   = numRed + numBlue;

    if (total > 0) {
        const double redPct  = static_cast<double>(numRed)  / total;
        const double bluePct = static_cast<double>(numBlue) / total;

        if (redPct  >= WIN_THRESHOLD) { std::cout << "Winner: Red\n";  isRunning = false; }
        if (bluePct >= WIN_THRESHOLD) { std::cout << "Winner: Blue\n"; isRunning = false; }
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
                               c->getType() == cse::CircleType::Predator ? 255 : 0,
                               0,
                               c->getType() == cse::CircleType::Prey    ? 255 : 0,
                               255);
        int cx = (int)c->getX(), cy = (int)c->getY(), r = (int)c->getRadius();
        for (int dx = -r; dx <= r; ++dx)
            for (int dy = -r; dy <= r; ++dy)
                if (dx*dx + dy*dy <= r*r)
                    SDL_RenderDrawPoint(gRenderer, cx + dx, cy + dy);
    }

    // Draw labels and counts
    int preds = countCircles(cse::CircleType::Predator);
    int prey  = countCircles(cse::CircleType::Prey);

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
int main(int argc, char* argv[])
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