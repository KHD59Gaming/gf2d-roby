#include "simple_logger.h"

#include "gfc_vector.h"
#include "entity.h"
#include "battery.h"
#include "roby.h"

#define ROBY_PROTO_POWER 0
#define ROBY_FLARE_POWER 1
#define ROBY_VOLT_POWER 2
#define ROBY_BOUNCE_POWER 3
#define ROBY_SPEED_POWER 4
#define ROBY_GOLD_POWER 5

Entity *roby_addr;

Entity *battery_new(Vector2D position, int power, Entity *roby) {
    Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;
    switch(power) {
        case ROBY_PROTO_POWER:
            ent->sprite = gf2d_sprite_load_image("images/proto_battery.png");
            ent->roby_power = ROBY_PROTO_POWER;
            break;
        case ROBY_FLARE_POWER:
            ent->sprite = gf2d_sprite_load_image("images/flare_battery.png");
            ent->roby_power = ROBY_FLARE_POWER;
            break;
        case ROBY_VOLT_POWER:
            ent->sprite = gf2d_sprite_load_image("images/volt_battery.png");
            ent->roby_power = ROBY_VOLT_POWER;
            break;
        case ROBY_BOUNCE_POWER:
            ent->sprite = gf2d_sprite_load_image("images/bounce_battery.png");
            ent->roby_power = ROBY_BOUNCE_POWER;
            break;
        case ROBY_SPEED_POWER:
            ent->sprite = gf2d_sprite_load_image("images/speed_battery.png");
            ent->roby_power = ROBY_SPEED_POWER;
            break;
        case ROBY_GOLD_POWER:
            ent->sprite = gf2d_sprite_load_image("images/gold_battery.png");
            ent->roby_power = ROBY_GOLD_POWER;
            break;
    }
    ent->think = battery_think;
    vector2d_copy(ent->position,position);
    ent->drawOffset = vector2d(32,32);
    ent->shape = gfc_shape_rect(0,0,16,32);
    roby_addr = roby;
    ent->is_battery = true;
    ent->is_roby = false;
    return ent;
}

void battery_think(Entity *self) {
    
}