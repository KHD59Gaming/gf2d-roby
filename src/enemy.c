#include "simple_logger.h"

#include "enemy.h"
#include "entity.h"
#include "projectile.h"
#include "ai.h"

void laservoid_think(Entity *self);

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
    }

}