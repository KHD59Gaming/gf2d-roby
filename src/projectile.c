#include "simple_logger.h"
#include "gfc_shape.h"
#include "gfc_types.h"
#include "gfc_vector.h"
#include "entity.h"
#include "projectile.h"

void projectile_think(Entity *self);

/**
 * @brief spawn a new projectile at the given location
 * @param position where to fire the projectile from
 * @param dir whether to fire the projectile right or left
 * @return NULL on failure, or a pointer to the new projectile
 */
Entity *projectile_new(Vector2D position, int dir) {
    Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->think = projectile_think;
    //ent->shape = gfc_shape_rect(-8,0,16,8);
    vector2d_copy(ent->position,position);
    ent->is_projectile = true;
    ent->drawOffset = vector2d(4,32);
    return ent;
}

void projectile_think(Entity *self) {
    self->velocity.x = 0.0;
    self->velocity.y = 0.0;
    switch(self->proj_type) {
        case PROJECTILE_TYPE_FLARE:
        case PROJECTILE_TYPE_LASER:
            if (self->direction == ENTITY_DIR_RIGHT) {
                self->velocity.x += self->velo_per_frame;
            }
            else {
                self->velocity.x -= self->velo_per_frame;
            }
            break;
        case PROJECTILE_TYPE_BOLT:
            if (self->direction == ENTITY_DIR_RIGHT) {
                self->velocity.x += self->velo_per_frame;
            }
            else {
                self->velocity.x -= self->velo_per_frame;
            }

            if (self->bolt_frame == 0) {
                self->sprite = gf2d_sprite_load_image("images/projectiles/bolt1.png");
                self->bolt_frame = 1;
            }
            else {
                self->sprite = gf2d_sprite_load_image("images/projectiles/bolt0.png");
                self->bolt_frame = 0;
            }
            break;
    }
    //slog("%d",self->grounded);
    //slog("hi from projecile think, velo = %.1f, x: %.1f, y: %.1f",self->velocity.x,self->position.x,self->position.y);
}

Bool projectile_evil(int type) {
    if (type == PROJECTILE_TYPE_LASER) {
        return true;
    }
    return false;
}

void init_projectile_params(Entity *self, int type) {
    if ((!self))return;
    switch(type) {
        case PROJECTILE_TYPE_FLARE:
            self->sprite = gf2d_sprite_load_image("images/projectiles/flare.png");
            self->velo_per_frame = 16;
            self->damage = 10;
            self->proj_type = type;
            break;
        case PROJECTILE_TYPE_BOLT:
            self->sprite = gf2d_sprite_load_image("images/projectiles/bolt0.png");
            self->velo_per_frame = 16;
            self->damage = 2;
            self->bolt_frame = 0;
            self->proj_type = type;
            self->position.x -= 16; //looks better
            break;
        case PROJECTILE_TYPE_LASER:
            self->sprite = gf2d_sprite_load_image("images/projectiles/laser.png");
            self->velo_per_frame = 12;
            self->damage = 1;
            self->proj_type = type;
            self->position.x -= 48;
            self->position.y -= 32;
            break;
    }
}

void projectile_fire(Entity *ent, int type) {
    if ((!ent))return;
    Entity *proj;
    Vector2D spawn_position = ent->position;
    spawn_position.x -= 32;
    spawn_position.y += 32;
    proj = projectile_new(spawn_position,type);
    if ((!proj)) {
        slog("projectile not spawned");
        return;
    }
    init_projectile_params(proj, type);
    proj->direction = ent->direction;
}