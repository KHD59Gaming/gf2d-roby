#include "simple_logger.h"

#include "gfc_shape.h"

#include "roby.h"
#include "camera.h"
#include "physics.h"
#include "level.h"
#include "collisions.h"
#include "projectile.h"

int jumpFrames = 0;
int gravFrames = 0;

void roby_think(Entity *self);


Entity *roby_new(Vector2D position)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/roby/roby_idle.png");
    ent->think = roby_think;
    vector2d_copy(ent->position,position);
    ent->drawOffset = vector2d(32,32);
    ent->speed = ROBY_SPEED;
    ent->shape = gfc_shape_rect(-32,0,64,32);
    ent->roby_power = ROBY_PROTO_POWER;
    ent->is_roby = true;
    ent->dead = false;
    ent->cooldown = 0;
    ent->health = 1;
    //ent->is_battery = false;
    return ent;
}

void roby_think(Entity *self)
{
    //slog("current power id %d",self->roby_power);
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    self->velocity.x = 0.0;
    self->velocity.y = 0.0;
    float speed_mult = 1;
    if (keys[SDL_SCANCODE_A]) {
        //slog("A key pressed");
        roby_edit_sprite(self, ROBY_LEFT_FRAME);
        if (self->roby_power == ROBY_SPEED_POWER) {
            speed_mult = 1.5;
        }
        self->velocity.x -= (ROBY_SPEED * speed_mult);
        self->direction = ENTITY_DIR_LEFT;
    }
    else if (keys[SDL_SCANCODE_D]) {
        //slog("D key pressed");
        roby_edit_sprite(self, ROBY_RIGHT_FRAME);
        if (self->roby_power == ROBY_SPEED_POWER) {
            speed_mult = 1.5;
        }
        self->velocity.x += (ROBY_SPEED * speed_mult);
        self->direction = ENTITY_DIR_RIGHT;
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        //slog("Spacebar pressed");
    }
    if (keys[SDL_SCANCODE_W]) {
        //slog("W key pressed");
        if ((jumpFrames <= ROBY_MAX_JUMP_FRAMES)) {
            float bounce_mult = 1;
            if (self->roby_power == ROBY_BOUNCE_POWER) {
                bounce_mult = 1.5;
            }
            self->velocity.y -= ((ROBY_MAX_JUMP_HEIGHT * bounce_mult) - (0.1 * jumpFrames));
            jumpFrames++;
        }
        if (keys[SDL_SCANCODE_A]) {
            roby_edit_sprite(self, ROBY_LEFTAIR_FRAME);
            self->direction = ENTITY_DIR_LEFT;
        }
        else if (keys[SDL_SCANCODE_D]) {
            roby_edit_sprite(self, ROBY_RIGHTAIR_FRAME);
            self->direction = ENTITY_DIR_RIGHT;
        }
        else {
            roby_edit_sprite(self, ROBY_AIR_FRAME);
            self->direction = ENTITY_DIR_RIGHT;
        }
    }
    else if ((!keys[SDL_SCANCODE_W]) && (self->grounded)) {
        jumpFrames = 0;
    }
    else if (!keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_SPACE] && !keys[SDL_SCANCODE_W]) {
        //slog("No key pressed");
        roby_edit_sprite(self, ROBY_IDLE_FRAME);
    }

    if (keys[SDL_SCANCODE_E]) {
        //slog("projectile button press");
        if ((self->roby_power == ROBY_FLARE_POWER) && (self->cooldown == 0)) {
            //slog("FIRE");
            projectile_fire(self, PROJECTILE_TYPE_FLARE);
            self->cooldown = 30;
        }
        else if (self->roby_power == ROBY_VOLT_POWER) {
            //slog("BOLT");
            projectile_fire(self, PROJECTILE_TYPE_BOLT);
        }
        else {
            //slog("No projectile powerup");
        }
    }

    if (!self->grounded) {
        self->velocity.y += ROBY_GRAVITY * gravFrames;
        gravFrames++;
    }
    else {
        self->velocity.x *= 2;
        gravFrames = 0;
    }

    if (self->position.x < LEVEL_MIN)self->position.x = LEVEL_MIN; //prevents going past the map left
    if (self->position.y > LEVEL_MAX_Y)self->position.y = LEVEL_MAX_Y; //prevents falling below the map
    //slog("%d",self->grounded);
    camera_center_at(self->position);
    //slog("Roby Position: %.2f, %.2f", self->position.x, self->position.y);
    if (self->cooldown > 0) {
        self->cooldown--;
    }
}

void roby_edit_power(Entity *self, int p) {
    //slog("power edit to %d",p);
    switch(p) {
            case ROBY_PROTO_POWER:
                self->sprite = gf2d_sprite_load_image("images/roby/roby_idle.png");
                self->roby_power = ROBY_PROTO_POWER;
                break;
            case ROBY_FLARE_POWER:
                self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_idle.png");
                self->roby_power = ROBY_FLARE_POWER;
                break;
            case ROBY_VOLT_POWER:
                self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_idle.png");
                self->roby_power = ROBY_VOLT_POWER;
                break;
            case ROBY_BOUNCE_POWER:
                self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_idle.png");
                self->roby_power = ROBY_BOUNCE_POWER;
                break;
            case ROBY_SPEED_POWER:
                self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_idle.png");
                self->roby_power = ROBY_SPEED_POWER;
                break;
            case ROBY_GOLD_POWER:
                self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_idle.png");
                self->roby_power = ROBY_GOLD_POWER;
                break;
        }
}

void roby_edit_sprite(Entity *self, int frame) {
    if (!self)return;
    switch(frame) {
        case ROBY_IDLE_FRAME:
            switch(self->roby_power) {
                case ROBY_PROTO_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/roby_idle.png");
                    break;
                case ROBY_FLARE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_idle.png");
                    break;
                case ROBY_VOLT_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_idle.png");
                    break;
                case ROBY_BOUNCE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_idle.png");
                    break;
                case ROBY_SPEED_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_idle.png");
                    break;
                case ROBY_GOLD_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_idle.png");
                    break;
            }
            break;
        case ROBY_LEFT_FRAME:
            switch(self->roby_power) {
                case ROBY_PROTO_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/roby_left.png");
                    break;
                case ROBY_FLARE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_left.png");
                    break;
                case ROBY_VOLT_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_left.png");
                    break;
                case ROBY_BOUNCE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_left.png");
                    break;
                case ROBY_SPEED_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_left.png");
                    break;
                case ROBY_GOLD_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_left.png");
                    break;
            }
            break;
        case ROBY_RIGHT_FRAME:
            switch(self->roby_power) {
                case ROBY_PROTO_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/roby_right.png");
                    break;
                case ROBY_FLARE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_right.png");
                    break;
                case ROBY_VOLT_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_right.png");
                    break;
                case ROBY_BOUNCE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_right.png");
                    break;
                case ROBY_SPEED_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_right.png");
                    break;
                case ROBY_GOLD_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_right.png");
                    break;
            }
            break;
        case ROBY_LEFTAIR_FRAME:
            switch(self->roby_power) {
                case ROBY_PROTO_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/roby_airleft.png");
                    break;
                case ROBY_FLARE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_airleft.png");
                    break;
                case ROBY_VOLT_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_airleft.png");
                    break;
                case ROBY_BOUNCE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_airleft.png");
                    break;
                case ROBY_SPEED_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_airleft.png");
                    break;
                case ROBY_GOLD_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_airleft.png");
                    break;
            }
            break;
        case ROBY_RIGHTAIR_FRAME:
            switch(self->roby_power) {
                case ROBY_PROTO_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/roby_airright.png");
                    break;
                case ROBY_FLARE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_airright.png");
                    break;
                case ROBY_VOLT_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_airright.png");
                    break;
                case ROBY_BOUNCE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_airright.png");
                    break;
                case ROBY_SPEED_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_airright.png");
                    break;
                case ROBY_GOLD_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_airright.png");
                    break;
            }
            break;
        case ROBY_AIR_FRAME:
            switch(self->roby_power) {
                case ROBY_PROTO_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/roby_air.png");
                    break;
                case ROBY_FLARE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_air.png");
                    break;
                case ROBY_VOLT_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_air.png");
                    break;
                case ROBY_BOUNCE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_air.png");
                    break;
                case ROBY_SPEED_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_air.png");
                    break;
                case ROBY_GOLD_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_air.png");
                    break;
            }
            break;
        case ROBY_DEATH_FRAME:
            switch(self->roby_power) {
                case ROBY_PROTO_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/roby_death.png");
                    break;
                case ROBY_FLARE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/flare_roby_death.png");
                    break;
                case ROBY_VOLT_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/volt_roby_death.png");
                    break;
                case ROBY_BOUNCE_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/bounce_roby_death.png");
                    break;
                case ROBY_SPEED_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/speed_roby_death.png");
                    break;
                case ROBY_GOLD_POWER:
                    self->sprite = gf2d_sprite_load_image("images/roby/gold_roby_death.png");
                    break;
            }
            break;
    }
}

/*eol@eof*/