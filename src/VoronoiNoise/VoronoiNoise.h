#include "../../libraries/voronoinoise.hpp"
#include "splashkit.h"

// This finds which site a coordinate belongs to in the Voronoi map
int find_closest_site(int x, int y, const cinekine::voronoi::Sites& sites) {
    int closest_index = 0;
    float min_dist = screen_width();

    for (int i = 0; i < sites.size(); ++i) {
        float dx = sites[i].x - x;
        float dy = sites[i].y - y;
        float dist_sq = dx*dx + dy*dy;

        if (dist_sq < min_dist) {
            min_dist = dist_sq;
            closest_index = i;
        }
    }
    return closest_index;
}