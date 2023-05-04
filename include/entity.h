#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_shape.h"

#include "gf2d_sprite.h"

#include "ai.h"

#define ENTITY_DIR_LEFT 0
#define ENTITY_DIR_RIGHT 1

typedef struct Entity_S
{
    Bool    _inuse;
    Sprite *sprite;
    float   frame;
    float   rotation;
    float   speed;
    Vector2D drawOffset;
    int grounded;
    int roby_power;
    Bool is_roby;
    Bool dead;
    Bool is_battery;
    Bool is_projectile;
    Bool is_enemy;
    
    Shape shape;    //collision shape
    
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    int direction;
    int velo_per_frame;
    int damage;
    int cooldown;
    int proj_type;
    int bolt_frame;
    Bool enemy_proj;

    int health;
    
    void (*think)(struct Entity_S *self);
    int (*update)(struct Entity_S *self);
    void (*draw)(struct Entity_S *self);
    void (*free_entity)(struct Entity_S *self);//cleanup of custom data if defined
    void *data;

    AIState ai_state;
}Entity;

void entity_manager_close();

/**
 * @brief initialize the internal manager for the entity system
 * @note this will automatically queue up the close function for program exit
 * @param max this is the maximum number of supported entities at a given time
 */
void entity_manager_init(Uint32 max);

/**
 * @brief alloate an initialize a new entity
 * @return NULL if there are no entities left, an empty entity otherwise
 */
Entity *entity_new();

int entity_count();

/**
 * @brief free a previously allocated entity
 * @param ent the entity to free
 */
void entity_free(Entity *ent);

/**
 * @brief free all allocated entities
 */
void entity_free_all();

/**
 * @brief draw all active entities if they have graphics
 */
void entity_draw_all();

/**
 * @brief update all active entities
 */
void entity_update_all();

/**
 * @brief call all the think functions for the entities, if they have one
 */
void entity_think_all();

Entity *get_roby();

/**
 * @brief given an entity get its shape in world space
 * @param ent the entity to check
 * @return a shape where its position is set to the world position
 */
Shape entity_get_shape(Entity *ent);

/**
 * @brief given an entity get its shape in world space where it will be after it moves
 * @param ent the entity to check
 * @return a shape where its position + velocity is set to the world position
 */
Shape entity_get_shape_after_move(Entity *ent);

#endif