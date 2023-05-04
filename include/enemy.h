#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "simple_logger.h"

#include "entity.h"
#include "projectile.h"
#include "ai.h"

Entity *enemy_new(Vector2D position);

Entity *laservoid_new(Vector2D position);

void laservoid_think(Entity *self);

#endif