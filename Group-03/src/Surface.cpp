// Surface.cpp
#include "Surface.h"
#include <algorithm>   // for std::clamp
#include <cmath>       // if you need math in future

namespace cse {

Surface::Surface(int width, int height, int sector_size)
    : surface_width(width),
      surface_height(height),
      surface_size(sector_size),
      sector_width(width  / sector_size),
      sector_height(height / sector_size)
{
    sectors.resize(sector_width,
                   std::vector<Sector>(sector_height, Sector(0,0)));
    for (int i = 0; i < sector_width; ++i) {
        for (int j = 0; j < sector_height; ++j) {
            sectors[i][j] = Sector(i, j);
        }
    }
}

void Surface::add_circle(std::shared_ptr<Circle> circle) {
    int x = static_cast<int>(circle->getX()) / surface_size;
    int y = static_cast<int>(circle->getY()) / surface_size;
    sectors[x][y].add_circle(circle);
}

void Surface::move_circle(std::shared_ptr<Circle> circle,
                          double x_pos, double y_pos)
{
    int old_x = static_cast<int>(circle->getX()) / surface_size;
    int old_y = static_cast<int>(circle->getY()) / surface_size;
    int new_x = static_cast<int>(x_pos) / surface_size;
    int new_y = static_cast<int>(y_pos) / surface_size;

    if (old_x != new_x || old_y != new_y) {
        sectors[old_x][old_y].remove_circle(circle);
        sectors[new_x][new_y].add_circle(circle);
    }
    circle->setPosition(x_pos, y_pos);
}

std::pair<std::string, std::shared_ptr<Circle>>
Surface::check_collision()
{
    // Intra-sector
    for (int i = 0; i < sector_width; ++i) {
        for (int j = 0; j < sector_height; ++j) {
            auto& vec = sectors[i][j].circles;
            size_t n = vec.size();
            if (n < 2) continue;

            for (size_t a = 0; a < n; ++a) {
                for (size_t b = a + 1; b < n; ++b) {
                    auto c1 = vec[a], c2 = vec[b];
                    if (c1->overlaps(*c2)) {
                        if (c1->getCircleType() == c2->getCircleType())
                            return { "add",    c1 };
                        if (c1->getCircleType() == "blue")
                            return { "delete", c1 };
                        return { "delete", c2 };
                    }
                }
            }
        }
    }

    // Inter-sector (only 5 forward neighbors)
    static constexpr int OFFSETS[5][2] = {
        { 0,  1}, { 1, -1}, { 1, 0}, { 1, 1}, { 0, -1}
    };

    for (int i = 0; i < sector_width; ++i) {
        for (int j = 0; j < sector_height; ++j) {
            auto& vec = sectors[i][j].circles;
            if (vec.empty()) continue;

            for (auto& off : OFFSETS) {
                int ni = i + off[0], nj = j + off[1];
                if (ni < 0 || ni >= sector_width ||
                    nj < 0 || nj >= sector_height) continue;

                auto& neigh = sectors[ni][nj].circles;
                if (neigh.empty()) continue;

                for (auto& c1 : vec) {
                    for (auto& c2 : neigh) {
                        if (c1->overlaps(*c2)) {
                            if (c1->getCircleType() == c2->getCircleType())
                                return { "add",    c1 };
                            if (c1->getCircleType() == "blue")
                                return { "delete", c1 };
                            return { "delete", c2 };
                        }
                    }
                }
            }
        }
    }

    return { "nothing", nullptr };
}

void Surface::update() {
    for (int i = 0; i < sector_width; ++i) {
        for (int j = 0; j < sector_height; ++j) {
            auto& sector = sectors[i][j];


            for (size_t k = 0; k < sector.circles.size();) {
                auto circle = sector.circles[k];
                // you might call circle->integrate() here if you add that method
                circle->move(surface_width, surface_height);

                int new_x = static_cast<int>(circle->getX()) / surface_size;
                int new_y = static_cast<int>(circle->getY()) / surface_size;
                new_x = std::clamp(new_x, 0, sector_width - 1);
                new_y = std::clamp(new_y, 0, sector_height - 1);

                if (new_x != i || new_y != j) {
                    sector.remove_circle(circle);
                    sectors[new_x][new_y].add_circle(circle);
                } else {
                    ++k;
                }
            }
        }
    }
}

void Surface::remove_circle(std::shared_ptr<Circle> circle) {
    int sx = static_cast<int>(circle->getX()) / surface_size;
    int sy = static_cast<int>(circle->getY()) / surface_size;
    if (sx >= 0 && sx < sector_width &&
        sy >= 0 && sy < sector_height) {
        sectors[sx][sy].remove_circle(circle);
    }
}

} // namespace cse
