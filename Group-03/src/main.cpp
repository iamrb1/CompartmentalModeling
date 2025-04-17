// main.cpp  – no SDL_ttf; built‑in 3×5 bitmap font draws “PREDATORS NN”
//                                                and “PREY NN” in‑window.
//
// Native build
//   g++ main.cpp Circle.cpp Surface.cpp -lSDL2 -std=c++17 -O2 -o circles
//
// Emscripten build
//   emcc main.cpp Circle.cpp Surface.cpp -sUSE_SDL=2 -std=c++17 -O2 -o circles.html

#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#  include <emscripten/emscripten.h>
#endif

#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>

// -------- your own headers ----------
#include "Circle.h"
#include "Surface.h"

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
static constexpr int WINDOW_WIDTH  = 800;
static constexpr int WINDOW_HEIGHT = 600;

static SDL_Window*   gWindow   = nullptr;
static SDL_Renderer* gRenderer = nullptr;

static std::unique_ptr<cse::Surface>      gSurface;
static std::vector<std::shared_ptr<Circle>> gCircles;

static bool isRunning = true;

// ---------------------------------------------------------------------------
// 3×5 bitmap font  (only the characters we need)
// bit rows are 3‑bit values, high‑bit = left pixel
// ---------------------------------------------------------------------------
static constexpr int FONT_SCALE   = 4;           // bitmap‑>screen scale factor
static constexpr int CHAR_W       = 3 * FONT_SCALE;
static constexpr int CHAR_H       = 5 * FONT_SCALE;
static constexpr int CHAR_SPACING = FONT_SCALE;  // gap between glyphs

/* digits 0‑9 */
static const uint8_t DIGIT[10][5] = {
 /*0*/ {0b111,0b101,0b101,0b101,0b111},
 /*1*/ {0b010,0b110,0b010,0b010,0b111},
 /*2*/ {0b111,0b001,0b111,0b100,0b111},
 /*3*/ {0b111,0b001,0b111,0b001,0b111},
 /*4*/ {0b101,0b101,0b111,0b001,0b001},
 /*5*/ {0b111,0b100,0b111,0b001,0b111},
 /*6*/ {0b111,0b100,0b111,0b101,0b111},
 /*7*/ {0b111,0b001,0b010,0b010,0b010},
 /*8*/ {0b111,0b101,0b111,0b101,0b111},
 /*9*/ {0b111,0b101,0b111,0b001,0b111}
};

/* uppercase letters we need: A,D,E,O,P,R,S,T,Y  (index = letter-'A')        */
static const uint8_t LETTER[26][5] = {
/* A */ {0b111,0b101,0b111,0b101,0b101},
/* B */ {0}, /* C */ {0}, /* D */ {0b110,0b101,0b101,0b101,0b110},
/* E */ {0b111,0b100,0b111,0b100,0b111},
/* F‑H */ {0},{0},{0},
/* I‑O */ {0},{0},{0},{0},{0},{0}, {0b111,0b101,0b101,0b101,0b111},
/* P */ {0b111,0b101,0b111,0b100,0b100},
/* Q,R */ {0},{0b111,0b101,0b111,0b101,0b101},
/* S */ {0b111,0b100,0b111,0b001,0b111},
/* T */ {0b111,0b010,0b010,0b010,0b010},
/* U‑X */ {0},{0},{0},{0},
/* Y */ {0b101,0b101,0b010,0b010,0b010},
/* Z */ {0}
};

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------
int countCircles(const std::string& type)
{
    return std::count_if(gCircles.begin(), gCircles.end(),
                         [&](const auto& c){
                             return c && c->getCircleType() == type;
                         });
}

// draw a single 3×5 glyph (digit OR letter)
void drawGlyph(const uint8_t rows[5], int x, int y, SDL_Color col)
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

/* draw a digit (0‑9) */
void drawDigit(int d, int x, int y, SDL_Color col)
{
    if (d < 0 || d > 9) return;
    drawGlyph(DIGIT[d], x, y, col);
}

/* draw an uppercase A‑Z that we have in LETTER[] */
void drawLetter(char ch, int x, int y, SDL_Color col)
{
    if (ch < 'A' || ch > 'Z') return;
    const uint8_t* rows = LETTER[ch - 'A'];
    if (rows[0] == 0) return;                   // unknown glyph
    drawGlyph(rows, x, y, col);
}

/* width of an n‑char string in pixels */
int textWidth(int len) { return len*CHAR_W + (len-1)*CHAR_SPACING; }

/* draw a string of uppercase letters */
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

/* draw a left‑aligned (max two‑digit) positive integer */
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

// ---------------------------------------------------------------------------
// random circles
// ---------------------------------------------------------------------------
void createDemoCircles()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int i = 0; i < 10; ++i) {
        double x = rand() % WINDOW_WIDTH;
        double y = rand() % WINDOW_HEIGHT;
        double r = 15.0;
        double s = 3.0;
        std::string type = (i % 2 == 0) ? "red" : "blue";

        auto c = std::make_shared<Circle>(x, y, r, s, s, type);
        gCircles.push_back(c);
        gSurface->add_circle(c);
    }
}

// ---------------------------------------------------------------------------
// update & draw
// ---------------------------------------------------------------------------
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
    gSurface->update();

    auto [act,vic] = gSurface->check_collision();
    if (act=="delete" && vic) {
        gCircles.erase(std::remove(gCircles.begin(), gCircles.end(), vic),
                       gCircles.end());
        gSurface->remove_circle(vic);
    }
    else if (act=="add" && vic && vic->getCircleType()=="blue") {
        static Uint32 last=0;
        if (SDL_GetTicks()-last>=1000) {
            auto b = std::make_shared<Circle>(*vic);
            gCircles.push_back(b);
            gSurface->add_circle(b);
            last = SDL_GetTicks();
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

void draw()
{
    SDL_SetRenderDrawColor(gRenderer, 0,0,0,255);
    SDL_RenderClear(gRenderer);

    // circles
    for (auto& c : gCircles) {
        if (!c) continue;
        SDL_SetRenderDrawColor(gRenderer,
            c->getCircleType()=="red" ? 255 : 0,
            0,
            c->getCircleType()=="blue"?255:0,
            255);
        int cx = (int)c->getX(), cy = (int)c->getY(), r=(int)c->getRadius();
        for (int dx=-r; dx<=r; ++dx)
            for (int dy=-r; dy<=r; ++dy)
                if (dx*dx+dy*dy<=r*r)
                    SDL_RenderDrawPoint(gRenderer,cx+dx,cy+dy);
    }

    // ------------------------------------------------------------
    // labels + numbers
    // ------------------------------------------------------------
    int preds = countCircles("red");
    int prey  = countCircles("blue");

    SDL_Color white = {255,255,255,255};
    SDL_Color red   = {255,0,0,255};
    SDL_Color blue  = {0,0,255,255};

    std::string labelPred = "PREDATORS";
    std::string labelPrey = "PREY";

    // width calculations
    int wPredLabel = textWidth(labelPred.size());
    int nDigitsPred = (preds>=10)?2:1;
    int wPredNum   = textWidth(nDigitsPred);
    int wTotalPred = wPredLabel + CHAR_SPACING + wPredNum;

    int wPreyLabel = textWidth(labelPrey.size());
    int nDigitsPrey = (prey>=10)?2:1;
    int wPreyNum   = textWidth(nDigitsPrey);
    int wTotalPrey = wPreyLabel + CHAR_SPACING + wPreyNum;

    // right edge margin 8 px
    int xPredLabel = WINDOW_WIDTH - 8 - wTotalPred;
    int yPred      = 8;

    int xPreyLabel = WINDOW_WIDTH - 8 - wTotalPrey;
    int yPrey      = yPred + CHAR_H + 2*CHAR_SPACING;

    // draw predator line
    drawString(labelPred, xPredLabel, yPred, white);
    drawNumber(preds,    xPredLabel + wPredLabel + CHAR_SPACING, yPred, red);

    // draw prey line
    drawString(labelPrey, xPreyLabel, yPrey, white);
    drawNumber(prey,      xPreyLabel + wPreyLabel + CHAR_SPACING, yPrey, blue);

    SDL_RenderPresent(gRenderer);
}

// ---------------------------------------------------------------------------
// event / main loop
// ---------------------------------------------------------------------------
void mainLoop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
        if (e.type==SDL_QUIT) isRunning=false;

    if (!isRunning) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }
    update();
    draw();
}

// ---------------------------------------------------------------------------
// main()
// ---------------------------------------------------------------------------
int main(int argc,char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO)<0){
        std::cerr<<"SDL init failed: "<<SDL_GetError()<<'\n';return 1;}

    gWindow = SDL_CreateWindow("Predator / Prey demo",
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    if(!gWindow){std::cerr<<"Window error: "<<SDL_GetError()<<'\n';return 1;}

    gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
    if(!gRenderer){std::cerr<<"Renderer error: "<<SDL_GetError()<<'\n';return 1;}

    gSurface = std::make_unique<cse::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT, 100);
    createDemoCircles();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while(isRunning){mainLoop();SDL_Delay(16);}
#endif

    gCircles.clear();
    gSurface.reset();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    return 0;
}
// comment
