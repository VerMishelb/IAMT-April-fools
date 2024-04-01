#include "../include/DickClark.h"

DickClark::DickClark() :
    next_dickclark(0),
    position_offset(0),
    hitcircle_offset({ 50, 50 }),
    active_dickclarks(0),
    level_finished(false),
    shoot_cooldown(200)
{}
DickClark::~DickClark() {}