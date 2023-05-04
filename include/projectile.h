#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "gfc_vector.h"
#include "entity.h"

#define PROJECTILE_TYPE_FLARE 0
#define PROJECTILE_TYPE_BOLT 1
#define PROJECTILE_TYPE_LASER 2

/**
 * @brief spawn a new projectile at the given location
 * @param position where to fire the projectile from
 * @param dir whether to fire the projectile right or left
 * @return NULL on failure, or a pointer to the new roby
 */
Entity *projectile_new(Vector2D position, int dir);

void projectile_think(Entity *self);

Bool projectile_evil(int type);

void projectile_fire(Entity *ent, int type);

#endif