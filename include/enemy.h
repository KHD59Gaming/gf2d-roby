#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "simple_logger.h"

#include "entity.h"
#include "projectile.h"
#include "ai.h"

Entity *enemy_new(Vector2D position);

Entity *laservoid_new(Vector2D position);

Entity *flarevoid_new(Vector2D position);

Entity *chasevoid_new(Vector2D position);

Entity *flyvoid_new(Vector2D position);

Entity *shyvoid_new(Vector2D position);

void laservoid_think(Entity *self);

void flarevoid_think(Entity *self);

void chasevoid_think(Entity *self);

void flyvoid_think(Entity *self);

void shyvoid_think(Entity *self);

#endif