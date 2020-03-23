/*
####################################
	          B1G Utils
	      Created by Atom
	GitHub: github.com/RealAtom
####################################
*/

#if defined _INC_B1G_UTILS_SAMP
	#endinput
#endif
#define _INC_B1G_UTILS_SAMP

#include <a_samp>
#include <Pawn.RakNet>

/* RPC ID'S */
new
	RPC_SetPlayerName = 11,
	RPC_CreateExplosion = 79,
	RPC_ToggleWidescreen = 111,
	RPC_SetGravity = 146,
	RPC_SetPlayerSkin = 153,
	RPC_UpdateScoresAndPings = 155
;

/* PLAYER INFO */
enum b1g_enum
{
	PING,
	bool:FAKE_PING,
	bool:TOGGLE_WIDESCREEN,
	PLAYER_SKIN[MAX_PLAYERS]
}
new B1G_PLAYER[MAX_PLAYERS][b1g_enum];
new B1G_NAMES[MAX_PLAYERS][MAX_PLAYERS][24];

public OnOutcomingRPC(playerid, rpcid, BitStream:bs)
{
	if(rpcid == RPC_UpdateScoresAndPings)
	{
		new 
			ping,
			target_player,
			BitStream:bs_two = BS_New()
		;

		BS_ReadValue(
			bs,
			PR_UINT16, target_player
		);

		if(B1G_PLAYER[target_player][FAKE_PING] == true) ping = B1G_PLAYER[target_player][PING];
		else ping = GetPlayerPing(target_player);

		BS_WriteValue(
			bs_two,
			PR_UINT16, playerid,
			PR_INT32, GetPlayerScore(target_player),
			PR_UINT32, ping
		);
				
		BS_RPC(bs_two, playerid, rpcid);
		BS_Delete(bs_two);
		return 0;
	}
	return 1;
}

/* PLAYER FUNCTIONS */
stock SetPlayerFakePing(playerid, ping)
{
	if(!IsPlayerConnected(playerid)) return 0;

	if(ping == -1) return B1G_PLAYER[playerid][FAKE_PING] = false;

	B1G_PLAYER[playerid][FAKE_PING] = true;
	B1G_PLAYER[playerid][PING] = ping;
	return 1;
}

stock GetPlayersOnIP(const ip[])
{
	new 
		temp_ip[16],
		count = 0
	;

	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
		    GetPlayerIp(i, temp_ip, 16);
		    if(!strcmp(temp_ip, ip)) count ++;
		}
	}
	return count;
}

stock ClearPlayerChat(playerid, ammount = 20)
{
	for(new i = 0; i != ammount; i++) SendClientMessage(playerid, -1, " ");
	return 1;
}

stock IsPlayerInWater(playerid)
{
	new lib[16], anims[32];
    GetAnimationName(GetPlayerAnimationIndex(playerid), lib, sizeof(lib), anims, sizeof(anims));
	if(!isnull(lib) && !strcmp(lib, "SWIM")) return 1;
    switch(GetPlayerAnimationIndex(playerid))
	{
	   case 1538..1542, 1544, 1250, 1062: return 1;
	   default: return 0;
	}
    return 0;
}

stock IsPlayerAiming(playerid)
{
	switch(GetPlayerAnimationIndex(playerid))
	{
	   case 1160..1163, 1167, 1365, 1643, 1453, 220: return 1;
	   default: return 0;
	}
 	return 0;
}

stock IsPlayerFalling(playerid)
{
    return (GetPlayerAnimationIndex(playerid) == 1195 || GetPlayerAnimationIndex(playerid) == 1197 || GetPlayerAnimationIndex(playerid) == 1133 || GetPlayerAnimationIndex(playerid) == 1130);
}

stock SetPlayerGravity(playerid, Float:gravity)
{
	if(!IsPlayerConnected(playerid)) return 0;

	new BitStream:bs = BS_New();

	BS_WriteValue(
		bs_,
		PR_FLOAT, gravity
	);
				
	BS_RPC(bs, playerid, RPC_SetGravity);
	BS_Delete(bs);
	return 1;
}

stock CreatePlayerExplosion(playerid, Float:X, Float:Y, Float:Z, type, Float:Radius)
{
	if(!IsPlayerConnected(playerid)) return 0;

	new BitStream:bs = BS_New();

	BS_WriteValue(
		bs_,
		PR_FLOAT, X,
		PR_FLOAT, Y,
		PR_FLOAT, Z,
		PR_UINT16, type,
		PR_FLOAT, Radius
	);
				
	BS_RPC(bs, playerid, RPC_CreateExplosion);
	BS_Delete(bs);
	return 1;
}

stock TogglePlayerWidescreen(playerid, bool:set)
{
	if(!IsPlayerConnected(playerid)) return 0;

	new BitStream:bs = BS_New();

	BS_WriteValue(
		bs_,
		PR_BOOL, set
	);

	B1G_PLAYER[playerid][TOGGLE_WIDESCREEN] = set;
				
	BS_RPC(bs, playerid, RPC_ToggleWidescreen);
	BS_Delete(bs);
	return 1;
}

stock IsPlayerWidescreenToggled(playerid)
{
	return B1G_PLAYER[playerid][TOGGLE_WIDESCREEN];
}

stock SetPlayerSkinForPlayer(playerid, target_player, skin)
{
	if(!IsPlayerConnected(playerid)) return 0;

	new BitStream:bs = BS_New();

	BS_WriteValue(
		bs_,
		PR_UINT16, playerid,
		PR_UINT32, skin
	);

	B1G_PLAYER[playerid][PLAYER_SKIN][target_player] = set;
				
	BS_RPC(bs, target_player, RPC_SetPlayerSkin);
	BS_Delete(bs);
	return 1;
}

stock GetPlayerSkinForPlayer(playerid, target_player)
{
	return B1G_PLAYER[playerid][PLAYER_SKIN][target_player];
}

stock SetPlayerNameForPlayer(playerid, target_player, const name[])
{
	if(!IsPlayerConnected(playerid)) return 0;

	new BitStream:bs = BS_New();

	BS_WriteValue(
		bs_,
		PR_UINT16, playerid,
		PR_UINT8, sizeof(name),
		PR_STRING, name
	);

	B1G_NAMES[playerid][PLAYER_NAME][target_player] = name;
				
	BS_RPC(bs, target_player, RPC_SetPlayerName);
	BS_Delete(bs);
	return 1;
}

stock GetPlayerNameForPlayer(playerid, target_player)
{
	return B1G_NAMES[playerid][PLAYER_NAME][target_player];
}

/* WEAPONS FUNCTIONS */
stock GetWeaponSlot(weaponid)
{
   	switch(weaponid)
   	{
    	case 0, 1: return 0;
      	case 2..9: return 1;
      	case 10..15: return 10;
      	case 16..18: return 8;
      	case 22..24: return 2;
      	case 25..27: return 3;
      	case 28, 29, 32: return 4;
      	case 30, 31: return 5;
      	case 33, 34: return 6;
      	case 35..38: return 7;
      	case 39: return 8;
      	case 40: return 12;
      	case 41..43: return 9;
      	case 44..46: return 11;
   	}
   	return -1;
}

stock GetWeaponObjectModel(weaponid)
{
	switch(weaponid)
	{
		case 1: return 331;
		case 2: return 333;
		case 3: return 334;
		case 4: return 335;
		case 5: return 336;
		case 6: return 337;
		case 7: return 338;
		case 8: return 339;
		case 9: return 341;
		case 10: return 321;
		case 11: return 322;
		case 12: return 323;
		case 13: return 324;
		case 14: return 325;
		case 15: return 326;
		case 16: return 342;
		case 17: return 343;
		case 18: return 344;
		case 22: return 346;
		case 23: return 347;
		case 24: return 348;
		case 25: return 349;
		case 26: return 350;
		case 27: return 351;
		case 28: return 352;
		case 29: return 353;
		case 30: return 355;
		case 31: return 356;
		case 32: return 372;
		case 33: return 357;
		case 34: return 358;
		case 35: return 359;
		case 36: return 360;
		case 37: return 361;
		case 38: return 362;
		case 39: return 363;
		case 40: return 364;
		case 41: return 365;
		case 42: return 366;
		case 43: return 367;
		case 44: return 368;
		case 45: return 369;
		case 46: return 371;
	}
	return 0;
}

/* COLOR MANIPULATION */
stock HexToRGBA(colour, &r, &g, &b, &a)
{
    r = (colour >> 24) & 0xFF;
    g = (colour >> 16) & 0xFF;
    b = (colour >> 8) & 0xFF;
    a = colour & 0xFF;
}

stock RGBAToHex(r, g, b, a)
{
    return (r<<24 | g<<16 | b<<8 | a);
}

stock RGBAToARGB(col)
{
	return ((((col) << 24) & 0xFF000000) | (((col) >>> 8) & 0xF7F7F7));
}

stock ARGBToRGBA(col)
{
	return ((((col) << 8) & 0xF7F7F700) | (((col) >>> 24) & 0xFF));
}
