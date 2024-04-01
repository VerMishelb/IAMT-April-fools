#ifndef Collision_h_
#define Collision_h_

#include <stdio.h>

struct HitCircle {
    int x, y, r;
};


static int distanceSquared(int x1, int y1, int x2, int y2) {
    return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

static bool collides(HitCircle c1, HitCircle c2) {
    //fprintf_s(stdout, "Collision check xyr: %d, %d, %d\t%d, %d, %d\n", c1.x, c1.y, c1.r, c2.x, c2.y, c2.r);
    int totalRadiusSquared = c1.r + c2.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    //fprintf_s(stdout, "totalRadiusSquared = %d, distanceSquared() = %d\n", totalRadiusSquared, distanceSquared(c1.x, c1.y, c2.x, c2.y));
    if (distanceSquared(c1.x, c1.y, c2.x, c2.y) < (totalRadiusSquared)) {
        return true;
    }
    return false;
}
#endif