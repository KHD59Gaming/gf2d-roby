#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_config.h"
#include "gfc_list.h"

#include "gf2d_graphics.h"

#include "entity.h"
#include "camera.h"
#include "level.h"
#include "roby.h"
#include "battery.h"
#include "enemy.h"

void level_build(Level *level);

static Level *activeLevel = NULL;

Level *level_get_active_level()
{
    return activeLevel;
}

void level_set_active_level(Level *level)
{
    activeLevel = level;
}

void level_load_entities(EntLoad *list, int count) {
    if (!list) {
        slog("entity list not found");
        return;
    }
    for (int i = 0; i < count; i++) {
        //slog("hi from load ent");
        if (strcmp(list[i].ent_type,"roby") == 0) {
            Entity *ent;
            ent = roby_new(list[i].spawn);
            if (!ent) {
                slog("roby entity not created");
            }
        }
        else if (strcmp(list[i].ent_type,"battery") == 0) {
            Entity *ent;
            ent = battery_new(list[i].spawn,list[i].power);
            if (!ent) {
                slog("flare_battery entity not created");
            }
        }
        else if (strcmp(list[i].ent_type,"laservoid") == 0) {
            Entity *ent;
            ent = laservoid_new(list[i].spawn);
            if (!ent) {
                slog("laservoid entity not created");
            }
        }
    }
}

Level *level_load(const char *filename)
{
    int tile;
    int i,c;
    int j,d;
    int tileFPL;
    const char *str;
    SJson *json,*lj,*list,*row,*item;
    Level *level;
    if (!filename)return NULL;
    json = sj_load(filename);
    if (!json)return NULL;
    level = level_new();
    if (!level)
    {
        slog("failed to allocate new level");
        sj_free(json);
        return NULL;
    }
    lj = sj_object_get_value(json,"level");
    if (!lj)
    {
        slog("file %s missing level object",filename);
        sj_free(json);
        level_free(level);
        return NULL;
    }
    str = sj_object_get_value_as_string(lj,"name");
    if (str)gfc_line_cpy(level->name,str);
    sj_value_as_vector2d(sj_object_get_value(lj,"tileSize"),&level->tileSize);
    sj_object_get_value_as_int(lj,"tileFPL",&tileFPL);
    str = sj_object_get_value_as_string(lj,"tileSet");
    if (str)
    {
        level->tileSet = gf2d_sprite_load_all(str,(Sint32)level->tileSize.x,(Sint32)level->tileSize.y,tileFPL,1);
    }
    list = sj_object_get_value(lj,"tileMap");
    c = sj_array_get_count(list);
    row = sj_array_get_nth(list,0);
    d = sj_array_get_count(row);
    if ((c * d) == 0)
    {
        slog("corrupt row or column count for %s level",filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    level->mapSize.x = d;
    level->mapSize.y = c;
    level->tileMap = gfc_allocate_array(sizeof(int),c * d);
    if (!level->tileMap)
    {
        slog("failed to allocate tileMap for level %s",filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    for (i = 0; i < c; i++)// i is row
    {
        row = sj_array_get_nth(list,i);
        if (!row)continue;
        d = sj_array_get_count(row);
        for (j =0; j < d; j++)// j is column
        {
            item = sj_array_get_nth(row,j);
            if (!item)continue;
            tile = 0;//default
            sj_get_integer_value(item,&tile);
            level->tileMap[(i * (int)level->mapSize.x) + j] = tile;
        }
    }
    lj = sj_object_get_value(json,"entities");
    if (!lj)
    {
        slog("file %s missing level object",filename);
        sj_free(json);
        level_free(level);
        return NULL;
    }
    list = sj_object_get_value(lj,"entityList");
    c = sj_array_get_count(list);
    item = sj_array_get_nth(list,0);
    if (c == 0)
    {
        slog("empty sjson count for %s level",filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    EntLoad *entlist = gfc_allocate_array(sizeof(EntLoad),c);
    if (!entlist)
    {
        slog("failed to allocate entity list for level %s",filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    for (i = 0; i < c; i++) {
        item = sj_array_get_nth(list,i);
        if (!item)continue;
        str = sj_object_get_value_as_string(item,"name");
        if (str)gfc_line_cpy(entlist[i].ent_type,str);
        sj_value_as_vector2d(sj_object_get_value(item,"spawn"),&entlist[i].spawn);
        sj_object_get_value_as_int(item,"power",&entlist[i].power);
        //slog("%s entity allocated",entlist[i].ent_type);
    }
    sj_free(json);
    level_build(level);
    slog("level %s built",level->name);
    level_load_entities(entlist, c);
    slog("%d level entities loaded",entity_count());
    return level;
}

int level_shape_clip(Level *level, Shape shape)
{
    int i,c;
    Shape *clip;
    if (!level)return 0;
    c = gfc_list_get_count(level->clips);
    for (i = 0; i < c; i++)
    {
        clip = gfc_list_get_nth(level->clips,i);
        if (!clip)continue;
        if (gfc_shape_overlap(*clip, shape))return 1;
    }
    return 0;
}

void level_build_clip_space(Level *level)
{
    Shape *shape;
    int i,j;
    if (!level)return;
    for (j = 0;j < level->mapSize.y;j++)//j is row
    {
        for (i = 0; i < level->mapSize.x;i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue;//skip zero
            shape = gfc_allocate_array(sizeof(Shape),1);
            if (!shape)continue;
            *shape = gfc_shape_rect(i * level->tileSize.x, j * level->tileSize.y, level->tileSize.x,level->tileSize.y);
            gfc_list_append(level->clips,shape);
            //slog("tile at %d, %d on list",j*64,i*64);
        }
    }
}

void level_build(Level *level)
{
    int i,j;
    if (!level)return;
    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    level->tileLayer = gf2d_sprite_new();
    if (!level->tileLayer)
    {
        slog("failed to create sprite for tileLayer");
        return;
    }
    // if there is a default surface, free it
    if (level->tileLayer->surface)SDL_FreeSurface(level->tileLayer->surface);
    //create a surface the size we need it
    level->tileLayer->surface = gf2d_graphics_create_surface(level->tileSize.x * level->mapSize.x,level->tileSize.y * level->mapSize.y);
    if (!level->tileLayer->surface)
    {
        slog("failed to create tileLayer surface");
        return;
    }
    //make sure the surface is compatible with our graphics settings
    level->tileLayer->surface = gf2d_graphics_screen_convert(&level->tileLayer->surface);
    if (!level->tileLayer->surface)
    {
        slog("failed to create surface for tileLayer");
        return;
    }
    //draw the tile sprite to the surface
    for (j = 0;j < level->mapSize.y;j++)//j is row
    {
        for (i = 0; i < level->mapSize.x;i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue;//skip zero
            gf2d_sprite_draw_to_surface(
                level->tileSet,
                vector2d(i * level->tileSize.x,j *level->tileSize.y),
                NULL,
                NULL,
                level->tileMap[(j * (int)level->mapSize.x) + i] - 1,
                level->tileLayer->surface);
        }
    }
    //convert it to a texture
    level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(),level->tileLayer->surface);
    SDL_SetTextureBlendMode(level->tileLayer->texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(level->tileLayer->texture,
                    NULL,
                    level->tileLayer->surface->pixels,
                    level->tileLayer->surface->pitch);
    level->tileLayer->frame_w = level->tileLayer->surface->w;
    level->tileLayer->frame_h = level->tileLayer->surface->h;
    level->tileLayer->frames_per_line = 1;
    camera_set_world_size(vector2d(level->tileLayer->frame_w,level->tileLayer->frame_h));
    level_build_clip_space(level);
}

void level_draw(Level *level)
{
    if (!level)return;
    if (!level->tileLayer)return;
    gf2d_sprite_draw_image(level->tileLayer,camera_get_draw_offset());
}

Level *level_new()
{
    Level *level;
    level = gfc_allocate_array(sizeof(Level),1);
    level->clips = gfc_list_new();
    return level;
}

void level_free(Level *level)
{
    if (!level)return;
    if (level->tileSet)gf2d_sprite_free(level->tileSet);
    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    if (level->tileMap)free(level->tileMap);
    gfc_list_foreach(level->clips,free);
    gfc_list_delete(level->clips);
    free(level);
    slog("level freed");
}

/*eol@eof*/