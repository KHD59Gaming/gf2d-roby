#ifndef __BATTERY_H__
#define __BATTERY_H__

#include "gfc_vector.h"
#include "entity.h"

#define ROBY_PROTO_POWER 0
#define ROBY_FLARE_POWER 1
#define ROBY_VOLT_POWER 2
#define ROBY_BOUNCE_POWER 3
#define ROBY_SPEED_POWER 4
#define ROBY_GOLD_POWER 5

Entity *battery_new(Vector2D position, int power, Entity *roby);

void battery_think(Entity *self);

#endif