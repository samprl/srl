#if defined _performance_included
    #endinput
#endif
#define _performance_included

#define GetBit(%0,%1) ((%0 >> %1) & 1)
#if !defined SendClientCheck
	native SendClientCheck(playerid, type, arg, offset, size);
#endif

#if defined PAWNRAKNET_INC_
	static pTickCount[MAX_PLAYERS char];
	static pValidSurface[MAX_PLAYERS char];
#endif

static pCheckPaused[MAX_PLAYERS];
static pUpdateTimer[MAX_PLAYERS];
static pKeySendType[MAX_PLAYERS char];
static vEngineMode[MAX_VEHICLES char];
static vEngineState[MAX_VEHICLES char];
static vEngineDefault = 1;

#define pressed(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#define holding(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

static gMaxSpeed[212] =
{
	157, 147, 186, 110, 133, 164, 110, 148, 100, 158, 129, 221, 168, 110,
	105, 192, 154, 270, 115, 149, 145, 154, 140, 99, 135, 270, 173, 165,
	157, 201, 190, 130, 94, 110, 167, 0, 149, 158, 142, 168, 136, 145, 139,
	126, 110, 164, 270, 270, 111, 0, 0, 193, 270, 60, 135, 157, 106, 95, 157,
	136, 270, 160, 111, 142, 145, 145, 147, 140, 144, 270, 157, 110, 190, 190,
	149, 173, 270, 186, 117, 140, 184, 73, 156, 122, 190, 99, 64, 270, 270, 139,
	157, 149, 140, 270, 214, 176, 162, 270, 108, 123, 140, 145, 216, 216, 173,
	140, 179, 166, 108, 79, 101, 270, 270, 270, 120, 142, 157, 157, 164, 270,
	270, 160, 176, 151, 130, 160, 158, 149, 176, 149, 60, 70, 110, 167,
	168, 158, 173, 0, 0, 270, 149, 203, 164, 151, 150, 147, 149, 142, 270,
	153, 145, 157, 121, 270, 144, 158, 113, 113, 156, 178, 169, 154, 178,
	270, 145, 165, 160, 173, 146, 0, 0, 93, 60, 110, 60, 158, 158, 270, 130,
	158, 153, 151, 136, 85, 0, 153, 142, 165, 108, 162, 0, 0, 270, 270, 130,
	190, 175, 175, 175, 158, 151, 110, 169, 171, 148, 152, 0, 0, 0, 108, 0, 0
};

public OnPlayerConnect(playerid)
{
    pKeySendType{playerid} = 0;
    #if defined PAWNRAKNET_INC_
    	pValidSurface{playerid} = 0;
    #endif
	#if defined pt_OnPlayerConnect
		return pt_OnPlayerConnect(playerid);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#if defined pt_OnPlayerConnect
	forward pt_OnPlayerConnect(playerid);
#endif
#define	OnPlayerConnect pt_OnPlayerConnect

public OnPlayerDisconnect(playerid, reason)
{
    if(pUpdateTimer[playerid])
	{
		KillTimer(pUpdateTimer[playerid]);
		pUpdateTimer[playerid] = 0;
	}
    #if defined pt_OnPlayerDisconnect
		return pt_OnPlayerDisconnect(playerid, reason);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#if defined pt_OnPlayerDisconnect
	forward pt_OnPlayerDisconnect(playerid, reason);
#endif
#define	OnPlayerDisconnect pt_OnPlayerDisconnect

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(pressed(KEY_SPRINT)) pKeySendType{playerid} = 1;
    else if(pressed(KEY_JUMP)) pKeySendType{playerid} = 2;
	else if(holding(KEY_SPRINT) || holding(KEY_JUMP)) pKeySendType{playerid} = 0;
	#if defined pt_OnPlayerKeyStateChange
		return pt_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#if defined pt_OnPlayerKeyStateChange
	forward pt_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif
#define	OnPlayerKeyStateChange pt_OnPlayerKeyStateChange

public OnPlayerUpdate(playerid)
{
    pCheckPaused[playerid] = 0;
	#if defined pt_OnPlayerUpdate
		return pt_OnPlayerUpdate(playerid);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#if defined pt_OnPlayerUpdate
	forward pt_OnPlayerUpdate(playerid);
#endif
#define	OnPlayerUpdate pt_OnPlayerUpdate

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(newstate == PLAYER_STATE_DRIVER)
    {
		if(pUpdateTimer[playerid]) KillTimer(pUpdateTimer[playerid]);
    	pUpdateTimer[playerid] = SetTimerEx("pt_UpdateTimer", 200, 1, "i", playerid);
    }
    else
    {
        if(pUpdateTimer[playerid])
	    {
			KillTimer(pUpdateTimer[playerid]);
			pUpdateTimer[playerid] = 0;
		}
	}
	#if defined pt_OnPlayerStateChange
		return pt_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnPlayerStateChange
    #undef OnPlayerStateChange
#else
    #define _ALS_OnPlayerStateChange
#endif
#if defined pt_OnPlayerStateChange
	forward pt_OnPlayerStateChange(playerid, newstate, oldstate);
#endif
#define	OnPlayerStateChange pt_OnPlayerStateChange

forward pt_UpdateTimer(playerid);
public pt_UpdateTimer(playerid) if(++pCheckPaused[playerid] < 3)
{
    #if defined PAWNRAKNET_INC_
	if(++pTickCount{playerid} >= 4)
	{
	    pTickCount{playerid} = 0;
	    SendClientCheck(playerid, 0x2, 0, 0, 2);
	}
	#endif
 	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
 	{
 	    new vehid1 = GetPlayerVehicleID(playerid);
	    if(vehid1> 0 && vEngineState{vehid1} && vEngineMode{vehid1} && pKeySendType{playerid})
	    {
	        #if defined PAWNRAKNET_INC_
	        	if(!pValidSurface{playerid}) return;
	        #endif
		    new Float:rot_x, Float:rot_y, Float:rot_z, model = GetVehicleModel(vehid1);
		    GetVehRot(vehid1, rot_x, rot_y, rot_z);
		    if((-40.0 < rot_x < 40.0) && (-40.0 < rot_y < 40.0) && model > 0)
			{
			    new speed = GetVehSpeed(vehid1);
			    new spmax = gMaxSpeed[model - 400];
			    if(pKeySendType{playerid} == 1)
				{
					if(2 < speed < (spmax + floatround(20 * vEngineMode{vehid1}))) SetVehSpeed(vehid1, (vEngineMode{vehid1} / 100.0));
				}
			    else
				{
					if(speed > 20 && !GetVehGoesBack(vehid1)) SetVehSpeed(vehid1, -((vEngineMode{vehid1} / 4500.0) * speed));
				}
			}
		}
	}
}

static stock GetVehRot(vehid1, &Float:rx, &Float:ry, &Float:rz)
{
	new Float:qw, Float:qx, Float:qy, Float:qz;
	GetVehicleRotationQuat(vehid1, qw, qx, qy, qz);
	rx = asin(2.0 * qy * qz - 2.0 * qx * qw);
	ry = -atan2(qx * qz + qy * qw, 0.5 - qx * qx - qy * qy);
	rz = -atan2(qx * qy + qz * qw, 0.5 - qx * qx - qz * qz);
}

static stock GetVehGoesBack(vehid1)
{
    new Float:pos_x, Float:pos_y, Float:pos_z, Float:pos_a, Float:vel_x, Float:vel_y, Float:vel_z, Float:npos_x, Float:npos_y;
    GetVehiclePos(vehid1, pos_x, pos_y, pos_z);
	GetVehicleZAngle(vehid1, pos_a);
    GetVehicleVelocity(vehid1, vel_x, vel_y, vel_z);
    npos_x = pos_x - (-10.0 * floatsin(pos_a, degrees));
	npos_y = pos_y - (10.0 * floatcos(pos_a, degrees));
	pos_x += vel_x;
	pos_y += vel_y;
	if(VectorSize((pos_x - npos_x), (pos_y - npos_y), 0.0) < 10.0) return 1;
	return 0;
}

static stock GetVehSpeed(vehid1)
{
    new Float:vel_x, Float:vel_y, Float:vel_z;
    GetVehicleVelocity(vehid1, vel_x, vel_y, vel_z);
	return floatround(VectorSize(vel_x, vel_y, vel_z) * 170.0);
}

static stock SetVehSpeed(vehid1, Float:speed)
{
    new Float:vel_x, Float:vel_y, Float:vel_z, Float:pos_a;
    GetVehicleZAngle(vehid1, pos_a);
    GetVehicleVelocity(vehid1, vel_x, vel_y, vel_z);
    SetVehicleVelocity(vehid1, vel_x + (speed * floatsin(-pos_a, degrees)), vel_y + (speed * floatcos(-pos_a, degrees)), vel_z);
}

#if defined PAWNRAKNET_INC_
IRPC:103(playerid, BitStream:bs) // CLIENT_CHECK
{
    if(playerid == -1) return 1;
    new type, arg, response;
    BS_ReadValue(bs, PR_UINT8, type, PR_UINT32, arg, PR_UINT8, response);
	if(type == 0x2) pValidSurface{playerid} = GetBit(arg, 9);
    return 1;
}
#endif

stock GetVehicleEngineState(vehid1)
{
	return vEngineMode{vehid1};
}
		
stock SetVehicleEngineState(vehid1, estate)
{
	if((0 <= estate <= 5) && GetVehicleModel(vehid1))
	{
		vEngineMode{vehid1} = estate;
		if(!estate)
	    {
	        #if defined foreach
	            foreach(Player, playerid)
	        #else
	        	for(new playerid = GetPlayerPoolSize(); playerid >= 0; playerid--) if(IsPlayerConnected(playerid))
	        #endif
	        {
		        if(IsPlayerInVehicle(playerid, vehid1) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		        {
		            if(pUpdateTimer[playerid])
		            {
						KillTimer(pUpdateTimer[playerid]);
						pUpdateTimer[playerid] = 0;
					}
				}
			}
		}
		else
	    {
	        #if defined foreach
	            foreach(Player, playerid)
	        #else
	        	for(new playerid = GetPlayerPoolSize(); playerid >= 0; playerid--) if(IsPlayerConnected(playerid))
	        #endif
	        {
	            if(IsPlayerInVehicle(playerid, vehid1) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	            {
					if(pUpdateTimer[playerid]) KillTimer(pUpdateTimer[playerid]);
			    	pUpdateTimer[playerid] = SetTimerEx("pt_UpdateTimer", 200, 1, "i", playerid);
    			}
   			}
	    }
		return 1;
	}
	return 0;
}
	
stock pt_ManualVehicleEngineLights()
{
	vEngineDefault = 0;
	return ManualVehicleEngineAndLights();
}
#if defined _ALS_ManualVehicleEngineLights
    #undef ManualVehicleEngineLights
#else
    #define _ALS_ManualVehicleEngineLights
#endif
#define	ManualVehicleEngineAndLights pt_ManualVehicleEngineLights

stock pt_SetVehicleParamsEx(vehid1, engine1, lightss1, alarm1, doors1, bonnet1, boot1, objective1)
{
    vEngineState{vehid1} = engine1;
    return SetVehicleParamsEx(vehid1, engine1, lightss1, alarm1, doors1, bonnet1, boot1, objective1);
}
#if defined _ALS_SetVehicleParamsEx
    #undef SetVehicleParamsEx
#else
    #define _ALS_SetVehicleParamsEx
#endif
#define	SetVehicleParamsEx pt_SetVehicleParamsEx

stock pt_CreateVehicle(model, Float:pos_x, Float:pos_y, Float:pos_z, Float:pos_r, col1, col2, respawn, siren = 0)
{
	new vehid1 = CreateVehicle(model, pos_x, pos_y, pos_z, pos_r, col1, col2, respawn, siren);
    if(vehid1 != INVALID_VEHICLE_ID)
	{
		vEngineState{vehid1} = vEngineDefault;
		vEngineMode{vehid1} = 0;
	}
	return vehid1;
}
#if defined _ALS_CreateVehicle
    #undef CreateVehicle
#else
    #define _ALS_CreateVehicle
#endif
#define	CreateVehicle pt_CreateVehicle

stock pt_AddStaticVehicle(model, Float:pos_x, Float:pos_y, Float:pos_z, Float:pos_r, col1, col2)
{
	new vehid1 = AddStaticVehicle(model, pos_x, pos_y, pos_z, pos_r, col1, col2);
	if(vehid1 != INVALID_VEHICLE_ID)
	{
		vEngineState{vehid1} = vEngineDefault;
		vEngineMode{vehid1} = 0;
	}
	return vehid1;
}
#if defined _ALS_AddStaticVehicle
    #undef AddStaticVehicle
#else
    #define _ALS_AddStaticVehicle
#endif
#define	AddStaticVehicle pt_AddStaticVehicle

stock pt_AddStaticVehicleEx(model, Float:pos_x, Float:pos_y, Float:pos_z, Float:pos_r, col1, col2, respawn, siren = 0)
{
	new vehid1 = AddStaticVehicleEx(model, pos_x, pos_y, pos_z, pos_r, col1, col2, respawn, siren);
	if(vehid1 != INVALID_VEHICLE_ID)
	{
		vEngineState{vehid1} = vEngineDefault;
		vEngineMode{vehid1} = 0;
	}
	return vehid1;
}
#if defined _ALS_AddStaticVehicleEx
    #undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif
#define	AddStaticVehicleEx pt_AddStaticVehicleEx
