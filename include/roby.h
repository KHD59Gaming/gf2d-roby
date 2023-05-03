#ifndef __ROBY_H__
#define __ROBY_H__

#include "gfc_vector.h"
#include "entity.h"

#define ROBY_PROTO_POWER 0
#define ROBY_FLARE_POWER 1
#define ROBY_VOLT_POWER 2
#define ROBY_BOUNCE_POWER 3
#define ROBY_SPEED_POWER 4
#define ROBY_GOLD_POWER 5

#define ROBY_IDLE_FRAME 0
#define ROBY_LEFT_FRAME 1
#define ROBY_RIGHT_FRAME 2
#define ROBY_LEFTAIR_FRAME 3
#define ROBY_RIGHTAIR_FRAME 4
#define ROBY_AIR_FRAME 5
#define ROBY_DEATH_FRAME 6

/**
 * @brief spawn a new Roby at the given location
 * @param position where to spawn roby at
 * @return NULL on failure, or a pointer to the new roby
 */
Entity *roby_new(Vector2D position);

void roby_think(Entity *self);

void roby_edit_power(Entity *self, int p);

void roby_edit_sprite(Entity *self, int frame);

#endif