#define GMMODULE
#include "GarrysMod/Lua/Interface.h"
#include "discord_rpc.h"
#include <stdio.h>
#include <string.h>
#include <utility>

DiscordUser cbConUser;
std::pair<int, char> cbDisconnected;
std::pair<int, char> cbError;
char cbJoinSecret;
char cbSpectateSecret;
DiscordUser cbJoinRequest;

static void HandleDiscordReady(const DiscordUser* connectedUser)
{
	cbConUser = *connectedUser;
}

static void HandleDiscordDisconnected(int errcode, const char* message)
{
	cbDisconnected.first = errcode;
	cbDisconnected.second = *message;
}

static void HandleDiscordError(int errcode, const char* message)
{
	cbError.first = errcode;
	cbError.second = *message;
}

static void HandleDiscordJoin(const char* secret)
{
	cbJoinSecret = *secret;
}

static void HandleDiscordSpectate(const char* secret)
{
	cbSpectateSecret = *secret;
}

static void HandleDiscordJoinRequest(const DiscordUser* request)
{
	cbJoinRequest = *request;
}

LUA_FUNCTION( StartDiscordStatus ) {
	
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

LUA_FUNCTION( UpdateDiscordStatus_Basic ) {

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

LUA_FUNCTION( UpdateDiscordStatus_Players ) {

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

LUA_FUNCTION( UpdateDiscordStatus_Elapsed) {

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