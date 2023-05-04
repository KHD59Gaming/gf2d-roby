#include "simple_logger.h"

#include "enemy.h"
#include "entity.h"
#include "projectile.h"
#include "ai.h"

void laservoid_think(Entity *self);
void flarevoid_think(Entity *self);

Entity *enemy_new(Vector2D position) {
    Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;
    vector2d_copy(ent->position,position);
    ent->drawOffset = vector2d(32,32);
    ent->shape = gfc_shape_rect(-32,0,32,32);
    ent->is_enemy = true;
    ent->cooldown = 0;
    return ent;
}

Entity *laservoid_new(Vector2D position) {
    Entity *ent;
    ent = enemy_new(position);
    if (!ent)return NULL;
    ent->think = laservoid_think;
    ent->sprite = gf2d_sprite_load_image("images/enemy/laservoid.png");
    ent->ai_state = Laservoid_Spawn_State;
    ent->direction = ENTITY_DIR_LEFT;
    ent->health = 10;
    ent->damage = 1;
    return ent;
}

Entity *flarevoid_new(Vector2D position) {
    Entity *ent;
    ent = enemy_new(position);
    if (!ent)return NULL;
    ent->think = flarevoid_think;
    ent->sprite = gf2d_sprite_load_image("images/enemy/flarevoid.png");
    ent->ai_state = Flarevoid_Spawn_State;
    ent->direction = ENTITY_DIR_LEFT;
    ent->health = 50;
    ent->damage = 1;
    return ent;
}

void laservoid_think(Entity *self) {
    if (!self)return;
    switch(self->ai_state) {
        case Laservoid_Spawn_State:
            self->cooldown = 120;
            self->ai_state = Laservoid_Cooldown_State;
            break;
        case Laservoid_Cooldown_State:
            if (self->cooldown > 0) {
                self->cooldown--;
            }
            if (self->cooldown == 0) {
                self->ai_state = Laservoid_Fire_State;
            }
            break;
        case Laservoid_Fire_State:
            //slog("laservoid fire");
            projectile_fire(self, PROJECTILE_TYPE_LASER);
            self->cooldown = 120;
            self->ai_state = Laservoid_Cooldown_State;
            break;
        default:
            break;
    }

}

void flarevoid_think(Entity *self) {
    if (!self)return;
    Entity *roby = get_roby();
    if (!roby)return;
    switch(self->ai_state) {
        case Flarevoid_Spawn_State:
            self->cooldown = 0;
            self->ai_state = Flarevoid_Player_Out_Range_State;
            break;
        case Flarevoid_Player_Out_Range_State:
            if (vector2d_distance_between_less_than(self->position, roby->position, 300)) {
                self->ai_state = Flarevoid_Player_In_Range_State;
            }
            break;
        case Flarevoid_Player_In_Range_State:
            if (!vector2d_distance_between_less_than(self->position, roby->position, 300)) {
                self->ai_state = Flarevoid_Player_Out_Range_State;
                break;
            }
            if (self->cooldown <= 0) {
                slog("flarevoid fire");
                if (roby->direction == ENTITY_DIR_RIGHT) {
                    self->direction = ENTITY_DIR_LEFT;
                }
                else {
                    self->direction = ENTITY_DIR_RIGHT;
                }
                projectile_fire(self, PROJECTILE_TYPE_FLARE);
                self->cooldown = 60;
            }
            else {
                self->cooldown--;
            }
            break;
        default:
            break;
    }

}