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
    Flarevoid_Player_In_Range_State
}AIState;

#endif