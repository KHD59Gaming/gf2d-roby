#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include "gfc_shape.h"
#include "gfc_types.h"

#include "simple_logger.h"

#include "entity.h"

/**
 * @brief check for collison between entities
 * @param s1 Shape object, typically the player entity
 * @param s2 Shape object, typically the non-player entity
 * @returns 0 if not colliding, 1 if colliding
 */
Uint8 entity_shapes_colliding(Entity * e1, Entity * e2);

#endif