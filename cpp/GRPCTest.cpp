#include "stdafx.h"
#include "Interface.h"
#include "discord_rpc.h"
#include <string>
#pragma comment(lib, "discord-rpc.lib")
//discord::Core* core{};

static void HandleDiscordReady(const DiscordUser* connectedUser)
{
	// Not implemented
}

static void HandleDiscordDisconnected(int errcode, const char* message)
{
	// Not implemented
}

static void HandleDiscordError(int errcode, const char* message)
{
	// Not implemented
}

static void HandleDiscordJoin(const char* secret)
{
	// Not implemented
}

static void HandleDiscordSpectate(const char* secret)
{
	// Not implemented
}

static void HandleDiscordJoinRequest(const DiscordUser* request)
{
	// Not implemented
}

int StartDiscordStatus(lua_State* state) {
	const GarrysMod::Lua::ILuaBase* LUA = state->luabase;
	const char* appid = LUA->GetString(1);

	// Discord RPC
	DiscordEventHandlers handlers{};

	handlers.ready = HandleDiscordReady;
	handlers.disconnected = HandleDiscordDisconnected;
	handlers.errored = HandleDiscordError;
	handlers.joinGame = HandleDiscordJoin;
	handlers.spectateGame = HandleDiscordSpectate;
	handlers.joinRequest = HandleDiscordJoinRequest;

	Discord_Initialize(appid, &handlers, 1, 0);

	return 0;
}

int UpdateDiscordStatus_Basic(lua_State* state) {
	GarrysMod::Lua::ILuaBase* LUA = state->luabase;

	// Read the arguments from the lua function
	const char* dState = LUA->GetString(1);
	const char* dDetails = LUA->GetString(2);
	const char* dImage = LUA->GetString(3);
	const char* dSubtitle = LUA->GetString(4);

	// Prepare rich presence
	DiscordRichPresence discordP{};

	// Update the rich presence
	discordP.state = dState;
	discordP.details = dDetails;
	discordP.largeImageKey = dImage;
	discordP.largeImageText = dSubtitle;
	Discord_UpdatePresence(&discordP);

	// Return 0 arguments to lua
	return 0;
}

int UpdateDiscordStatus_Players(lua_State* state) {
	GarrysMod::Lua::ILuaBase* LUA = state->luabase;

	// Read the arguments from the lua function
	const char* dState = LUA->GetString(1);
	const char* dDetails = LUA->GetString(2);
	const char* dImage = LUA->GetString(3);
	const char* dSubtitle = LUA->GetString(4);
	const int dPlayers = LUA->GetNumber(5);
	const int dMaxPlayers = LUA->GetNumber(6);

	// Prepare rich presence
	DiscordRichPresence discordP;
	memset(&discordP, 0, sizeof(discordP));

	// Update the rich presence
	discordP.state = dState;
	discordP.details = dDetails;
	discordP.largeImageKey = dImage;
	discordP.largeImageText = dSubtitle;
	discordP.partyId = "hahalolparty";
	discordP.partySize = dPlayers;
	discordP.partyMax = dMaxPlayers;
	Discord_UpdatePresence(&discordP);

	// Return 0 arguments to lua
	return 0;
}

int UpdateDiscordStatus_Elapsed(lua_State* state) {
	GarrysMod::Lua::ILuaBase* LUA = state->luabase;

	// Read the arguments from the lua function
	const char* dState = LUA->GetString(1);
	const char* dDetails = LUA->GetString(2);
	const char* dImage = LUA->GetString(3);
	const char* dSubtitle = LUA->GetString(4);
	const int dPlayers = LUA->GetNumber(5);
	const int dMaxPlayers = LUA->GetNumber(6);
	const int64_t dTime = LUA->GetNumber(7);

	// Prepare rich presence
	DiscordRichPresence discordP;
	memset(&discordP, 0, sizeof(discordP));

	// Update the rich presence
	discordP.state = dState;
	discordP.details = dDetails;
	discordP.largeImageKey = dImage;
	discordP.largeImageText = dSubtitle;
	discordP.partyId = "hahalolparty";
	discordP.partySize = dPlayers;
	discordP.partyMax = dMaxPlayers;
	discordP.startTimestamp = dTime;
	Discord_UpdatePresence(&discordP);

	// Return 0 arguments to lua
	return 0;
}

GMOD_MODULE_OPEN()
{
	// Create the functions
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->PushCFunction(StartDiscordStatus);
	LUA->SetField(-2, "DiscordRPCInitialize");
	LUA->Pop();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->PushCFunction(UpdateDiscordStatus_Basic);
	LUA->SetField(-2, "DiscordRPCBasic");
	LUA->Pop();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->PushCFunction(UpdateDiscordStatus_Players);
	LUA->SetField(-2, "DiscordRPCPlayers");
	LUA->Pop();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->PushCFunction(UpdateDiscordStatus_Elapsed);
	LUA->SetField(-2, "DiscordRPCTime");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	Discord_Shutdown();
	return 0;
}