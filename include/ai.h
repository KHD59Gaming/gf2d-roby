#ifndef __AI_H__
#define __AI_H__

#include "simple_logger.h"

#include "entity.h"

typedef enum
{
    Laservoid_Spawn_State,
    Laservoid_Cooldown_State,
    Laservoid_Fire_State,
    Flarevoid_Spawn_State,
    Flarevoid_Player_Out_Range_State,
    Flarevoid_Player_In_Range_State,
    Chasevoid_Spawn_State,
    Chasevoid_Player_Out_Range_State,
    Chasevoid_Player_In_Range_State,
    Flyvoid_Spawn_State,
    Flyvoid_Player_Out_Range_State,
    Flyvoid_Player_In_Range_State,
    Flyvoid_Drop_State,
    Flyvoid_Grounded_State,
    Shyvoid_Spawn_State,
    Shyvoid_Player_Out_Range_State,
    Shyvoid_Player_In_Range_State
}AIState;

#endif