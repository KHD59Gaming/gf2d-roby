#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "level.h"
#include "entity.h"
#include "camera.h"
#include "roby.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    Level *level;
    const Uint8 * keys;
    Sprite *sprite, *ph;
    Entity *ent, *b1, *b2, *b3, *b4, *b5;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;

    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(17,74,82,255), //background color
        0);
    gf2d_graphics_set_frame_delay(16);
    entity_manager_init(1024);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    level = level_load("levels/roby_level_zero.rlmap");
    level_set_active_level(level);

    sprite = gf2d_sprite_load_image("images/backgrounds/roby_bg.png");
    //mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    ent = roby_new(vector2d(100,500));
    b1 = battery_new(vector2d(500,500),ROBY_FLARE_POWER,ent);
    b2 = battery_new(vector2d(700,500),ROBY_VOLT_POWER,ent);
    b3 = battery_new(vector2d(900,150),ROBY_BOUNCE_POWER,ent);
    b4 = battery_new(vector2d(500,150),ROBY_SPEED_POWER,ent);
    b5 = battery_new(vector2d(300,500),ROBY_GOLD_POWER,ent);

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        entity_think_all();
        entity_update_all();
        camera_world_snap();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level_get_active_level());
            entity_draw_all();
        //UI elements last

        gf2d_draw_rect_filled(gfc_rect(20,20,400,50),gfc_color8(27,180,218,255));
        switch(ent->roby_power) {
            case ROBY_PROTO_POWER:
                ph = gf2d_sprite_load_image("images/hud/proto_battery_hud.png");
                break;
            case ROBY_FLARE_POWER:
                ph = gf2d_sprite_load_image("images/hud/flare_battery_hud.png");
                break;
            case ROBY_VOLT_POWER:
                ph = gf2d_sprite_load_image("images/hud/volt_battery_hud.png");
                break;
            case ROBY_BOUNCE_POWER:
                ph = gf2d_sprite_load_image("images/hud/bounce_battery_hud.png");
                break;
            case ROBY_SPEED_POWER:
                ph = gf2d_sprite_load_image("images/hud/speed_battery_hud.png");
                break;
            case ROBY_GOLD_POWER:
                ph = gf2d_sprite_load_image("images/hud/gold_battery_hud.png");
                break;
        }
        gf2d_sprite_draw_image(ph,vector2d(20,20));

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    level_free(level);
    entity_manager_close();
    return 0;
}
/*eol@eof*/