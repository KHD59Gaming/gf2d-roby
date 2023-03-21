#include "simple_logger.h"

#include "gfc_shape.h"

#include "roby.h"
#include "camera.h"
#include "physics.h"
#include "level.h"

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
    power = ROBY_PROTO_POWER;
    return ent;
}

void roby_think(Entity *self)
{
    /*Vector2D m,dir,camera;
    int mx,my;
    if (!self)return;
    camera = camera_get_position();
    SDL_GetMouseState(&mx,&my);
    m.x = mx;
    m.y = my;
    vector2d_add(m,m,camera);
    vector2d_sub(dir,m,self->position);
    if (vector2d_magnitude_compare(dir,10)>0)
    {
        vector2d_set_magnitude(&dir,self->speed);
        vector2d_copy(self->velocity,dir);
    }
    else
    {
        vector2d_clear(self->velocity);
    }
    camera_center_at(self->position);
    */

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    self->velocity.x = 0.0;
    self->velocity.y = 0.0;
    if (keys[SDL_SCANCODE_A]) {
        //slog("A key pressed");
        self->sprite = gf2d_sprite_load_image("images/roby/roby_left.png");
        self->velocity.x -= ROBY_SPEED;
    }
    else if (keys[SDL_SCANCODE_D]) {
        //slog("D key pressed");
        self->sprite = gf2d_sprite_load_image("images/roby/roby_right.png");
        self->velocity.x += ROBY_SPEED;
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        //slog("Spacebar pressed");
    }
    if (keys[SDL_SCANCODE_W]) {
        //slog("W key pressed");
        if (jumpFrames <= ROBY_MAX_JUMP_FRAMES) {
            self->velocity.y -= (ROBY_MAX_JUMP_HEIGHT - (0.1 * jumpFrames));
            jumpFrames++;
        }
        if (keys[SDL_SCANCODE_A]) {
            self->sprite = gf2d_sprite_load_image("images/roby/roby_airleft.png");
        }
        else if (keys[SDL_SCANCODE_D]) {
            self->sprite = gf2d_sprite_load_image("images/roby/roby_airright.png");
        }
        else {
            self->sprite = gf2d_sprite_load_image("images/roby/roby_air.png");
        }
    }
    else if (!keys[SDL_SCANCODE_W]) {
        jumpFrames = 0;
    }
    else if (!keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_SPACE] && !keys[SDL_SCANCODE_W]) {
        //slog("No key pressed");
        self->sprite = gf2d_sprite_load_image("images/roby/roby_idle.png");
    }

    if (!self->clipping) {
        self->velocity.y += ROBY_GRAVITY * gravFrames;
        gravFrames++;
    }
    else {
        self->velocity.x *= 2;
        gravFrames = 0;
    }

    if (self->position.y > LEVEL_MAX_Y)self->position.y = LEVEL_MAX_Y; //prevents falling below the map

    /*
    self->velocity.y += ROBY_GRAVITY;

    if (self->position.x < LEVEL_MIN)self->position.x = LEVEL_MIN; //prevents going past the map left
    if (self->position.x > LEVEL_MAX_X)self->position.x = LEVEL_MAX_X; //prevents going past the map right
    if (self->position.y > LEVEL_MAX_Y)self->position.y = LEVEL_MAX_Y; //prevents falling below the map
    */

    camera_center_at(self->position);

    slog("Roby Position: %.2f, %.2f", self->position.x, self->position.y);
}

/*eol@eof*/