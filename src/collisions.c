#include "gfc_shape.h"
#include "gfc_types.h"

#include "simple_logger.h"

#include "entity.h"
#include "collisions.h"

/**
 * @brief check for collison between entities
 * @param e1 Entity object, typically the player entity
 * @param e2 Entity object, typically the non-player entity
 * @returns 0 if not colliding, 1 if colliding
 */
Uint8 entity_shapes_colliding(Entity * e1, Entity * e2) {
    Uint8 result;
    result = gfc_shape_overlap(entity_get_shape(e1), entity_get_shape(e2));
    return result;
}