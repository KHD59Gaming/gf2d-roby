#include <stdlib.h>

#include "simple_logger.h"

#include "enemy.h"
#include "entity.h"
#include "projectile.h"
#include "ai.h"

void laservoid_think(Entity *self);
void flarevoid_think(Entity *self);
void chasevoid_think(Entity *self);
void flyvoid_think(Entity *self);

Entity *enemy_new(Vector2D position) {
    Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;
    vector2d_copy(ent->position,position);
    ent->drawOffset = vector2d(32,32);
    ent->shape = gfc_shape_rect(-32,0,64,64);
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

Entity *chasevoid_new(Vector2D position) {
    Entity *ent;
    ent = enemy_new(position);
    if (!ent)return NULL;
    ent->think = chasevoid_think;
    ent->sprite = gf2d_sprite_load_image("images/enemy/chasevoid_idle.png");
    ent->ai_state = Chasevoid_Spawn_State;
    ent->direction = ENTITY_DIR_LEFT;
    ent->is_ground_free = true;
    ent->health = 50;
    ent->damage = 1;
    ent->speed = 2;
    return ent;
}

Entity *flyvoid_new(Vector2D position) {
    Entity *ent;
    ent = enemy_new(position);
    if (!ent)return NULL;
    ent->think = flyvoid_think;
    ent->sprite = gf2d_sprite_load_image("images/enemy/flyvoid_idle.png");
    ent->ai_state = Flyvoid_Spawn_State;
    ent->direction = ENTITY_DIR_LEFT;
    ent->health = 10;
    ent->damage = 1;
    ent->speed = 20;
    return ent;
}

Entity *shyvoid_new(Vector2D position) {
    Entity *ent;
    ent = enemy_new(position);
    if (!ent)return NULL;
    ent->think = shyvoid_think;
    ent->sprite = gf2d_sprite_load_image("images/enemy/shyvoid_idle.png");
    ent->ai_state = Shyvoid_Spawn_State;
    ent->direction = ENTITY_DIR_LEFT;
    ent->is_ground_free = true;
    ent->health = 50;
    ent->damage = 1;
    ent->speed = 2;
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
                //slog("flarevoid fire");
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

void chasevoid_think(Entity *self) {
    if (!self)return;
    Entity *roby = get_roby();
    if (!roby)return;
    int activity_frames = 30;
    int wait_frames = 60;
    switch(self->ai_state) {
        case Chasevoid_Spawn_State:
            self->ai_state = Chasevoid_Player_Out_Range_State;
            break;
        case Chasevoid_Player_Out_Range_State:
            if (vector2d_distance_between_less_than(self->position, roby->position, 500)) {
                self->ai_state = Chasevoid_Player_In_Range_State;
                self->sprite = gf2d_sprite_load_image("images/enemy/chasevoid_left.png");
            }
            break;
        case Chasevoid_Player_In_Range_State:
            if (!vector2d_distance_between_less_than(self->position, roby->position, 500)) {
                self->ai_state = Chasevoid_Player_Out_Range_State;
                self->sprite = gf2d_sprite_load_image("images/enemy/chasevoid_idle.png");
                break;
            }
            //slog("in chasevoid range");
            if (activity_frames > 0) {
                self->velocity.x = 0.0;
                if (roby->position.x < self->position.x) {
                    self->velocity.x -= self->speed;
                    self->sprite = gf2d_sprite_load_image("images/enemy/chasevoid_left.png");
                }
                else {
                    self->velocity.x += self->speed;
                    self->sprite = gf2d_sprite_load_image("images/enemy/chasevoid_right.png");
                }
                activity_frames--;
                wait_frames = 60;
            }
            else {
                if (wait_frames <= 0) {
                    activity_frames = 30;
                }
                else {
                    wait_frames--;
                }
            }
            //slog("%.1f",self->velocity.x);
            break;
        default:
            break;
    }
}

void flyvoid_think(Entity *self) {
    if (!self)return;
    Entity *roby = get_roby();
    if (!roby)return;
    self->velocity.x = 0.0;
    self->velocity.y = 0.0;
    switch(self->ai_state) {
        case Flyvoid_Spawn_State:
            self->ai_state = Flyvoid_Player_Out_Range_State;
            self->cooldown = 100;
            break;
        case Flyvoid_Player_Out_Range_State:
            if (abs(roby->position.x - self->position.x) <= 10) {
                self->ai_state = Flyvoid_Player_In_Range_State;
            }
            break;
        case Flyvoid_Player_In_Range_State:
            if (self->cooldown == 0) {
                self->ai_state = Flyvoid_Drop_State;
            }
            else {
                self->velocity.x += roby->velocity.x;
                self->cooldown--;
                slog("flyframes %d",self->cooldown);
            }
            break;
        case Flyvoid_Drop_State:
            slog("dropping");
            if (self->grounded) {
                self->ai_state = Flyvoid_Grounded_State;
            }
            else {
                self->velocity.y += self->speed;
            }
            break;
        case Flyvoid_Grounded_State:
            break;
        default:
            break;
    }
}

void shyvoid_think(Entity *self) {
    if (!self)return;
    Entity *roby = get_roby();
    if (!roby)return;
    int activity_frames = 30;
    int wait_frames = 60;
    switch(self->ai_state) {
        case Shyvoid_Spawn_State:
            self->ai_state = Shyvoid_Player_Out_Range_State;
            break;
        case Shyvoid_Player_Out_Range_State:
            if (vector2d_distance_between_less_than(self->position, roby->position, 500)) {
                self->ai_state = Shyvoid_Player_In_Range_State;
                self->sprite = gf2d_sprite_load_image("images/enemy/shyvoid_right.png");
            }
            break;
        case Shyvoid_Player_In_Range_State:
            if (!vector2d_distance_between_less_than(self->position, roby->position, 500)) {
                self->ai_state = Shyvoid_Player_Out_Range_State;
                self->sprite = gf2d_sprite_load_image("images/enemy/shyvoid_idle.png");
                break;
            }
            //slog("in chasevoid range");
            if (activity_frames > 0) {
                self->velocity.x = 0.0;
                if (roby->position.x < self->position.x) {
                    self->velocity.x += self->speed;
                    self->sprite = gf2d_sprite_load_image("images/enemy/shyvoid_right.png");
                }
                else {
                    self->velocity.x -= self->speed;
                    self->sprite = gf2d_sprite_load_image("images/enemy/shyvoid_left.png");
                }
                activity_frames--;
                wait_frames = 60;
            }
            else {
                if (wait_frames <= 0) {
                    activity_frames = 30;
                }
                else {
                    wait_frames--;
                }
            }
            //slog("%.1f",self->velocity.x);
            break;
        default:
            break;
    }
}