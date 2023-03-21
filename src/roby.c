#include "simple_logger.h"
#include "roby.h"

#include "camera.h"

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
    ent->speed = 2.5;
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
    if (keys[SDL_SCANCODE_A]) {
            slog("A key pressed");
        }
        else if (keys[SDL_SCANCODE_D]) {
            slog("D key pressed");
        }
        if (keys[SDL_SCANCODE_SPACE]) {
            slog("Spacebar pressed");
        }
        if (keys[SDL_SCANCODE_W]) {
            slog("W key pressed");
        }
}

/*eol@eof*/