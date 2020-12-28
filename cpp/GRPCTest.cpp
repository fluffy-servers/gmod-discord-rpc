#define GMMODULE
#include "GarrysMod/Lua/Interface.h"
#include "discord_rpc.h"
#include <stdio.h>
#include <string.h>
#include <utility>

static void HandleDiscordReady(const DiscordUser *connectedUser) {
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-2, "hook");
    LUA->GetField(-1, "Run");
    LUA->PushString("DiscordReady");
    LUA->PushString(request.userId);
    LUA->PushString(request.username);
    LUA->PushString(request.discriminator);
    LUA->PushString(request.avatar);
    LUA->Call(5, 0);
}

static void HandleDiscordDisconnected(int errcode, const char *message) {
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-2, "hook");
    LUA->GetField(-1, "Run");
    LUA->PushString("DiscordDisconnected");
    LUA->PushNumber(errcode);
    LUA->PushString(&message);
    LUA->Call(3, 0);
}

static void HandleDiscordError(int errcode, const char *message) {
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-2, "hook");
    LUA->GetField(-1, "Run");
    LUA->PushString("DiscordError");
    LUA->PushNumber(errcode);
    LUA->PushString(&message);
    LUA->Call(3, 0);
}

static void HandleDiscordJoin(const char *secret) {
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-2, "hook");
    LUA->GetField(-1, "Run");
    LUA->PushString("DiscordJoin");
    LUA->PushString(&secret);
    LUA->Call(2, 0);
}

static void HandleDiscordSpectate(const char *secret) {
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-2, "hook");
    LUA->GetField(-1, "Run");
    LUA->PushString("DiscordSpectate");
    LUA->PushString(&secret);
    LUA->Call(2, 0);
}

static void HandleDiscordJoinRequest(const DiscordUser *request) {
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-2, "hook");
    LUA->GetField(-1, "Run");
    LUA->PushString("DiscordJoinRequest");
    LUA->PushString(request.userId);
    LUA->PushString(request.username);
    LUA->PushString(request.discriminator);
    LUA->PushString(request.avatar);
    LUA->Call(5, 0);
}

LUA_FUNCTION(StartDiscordStatus) {

    const char *appid = LUA->GetString(1);

    // Discord RPC
    DiscordEventHandlers handlers{};

    handlers.ready = HandleDiscordReady;
    handlers.disconnected = HandleDiscordDisconnected;
    handlers.errored = HandleDiscordError;
    handlers.joinGame = HandleDiscordJoin;
    handlers.spectateGame = HandleDiscordSpectate;
    handlers.joinRequest = HandleDiscordJoinRequest;

    Discord_Initialize(appid, &handlers, 1, "4000");

    return 0;
}

LUA_FUNCTION(RunDiscordCallbacks) {
    Discord_RunCallbacks();
    return 0;
}

LUA_FUNCTION(DiscordRespond) {
    const char *userid = LUA->GetString(1);
    const int reply = LUA->GetNumber(2);
    Discord_Respond(userid, reply);
    return 0;
}

LUA_FUNCTION(UpdateDiscordStatus_Basic) {

    // Read the arguments from the lua function
    const char *dState = LUA->GetString(1);
    const char *dDetails = LUA->GetString(2);
    const char *dImage = LUA->GetString(3);
    const char *dSubtitle = LUA->GetString(4);

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

LUA_FUNCTION(UpdateDiscordStatus_Players) {

    // Read the arguments from the lua function
    const char *dState = LUA->GetString(1);
    const char *dDetails = LUA->GetString(2);
    const char *dImage = LUA->GetString(3);
    const char *dSubtitle = LUA->GetString(4);
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

LUA_FUNCTION(UpdateDiscordStatus_Elapsed) {
    // Read the arguments from the lua function
    const char *dState = LUA->GetString(1);
    const char *dDetails = LUA->GetString(2);
    const char *dImage = LUA->GetString(3);
    const char *dSubtitle = LUA->GetString(4);
    const int dPlayers = LUA->GetNumber(5);
    const int dMaxPlayers = LUA->GetNumber(6);
    const int64_t dTime = LUA->GetNumber(7);
    const char *dPartyId = LUA->GetString(8);
    const char *dJoinSecret = LUA->GetString(9);
    const char *dSpectateSecret = LUA->GetString(10);

    // Prepare rich presence
    DiscordRichPresence discordP;
    memset(&discordP, 0, sizeof(discordP));

    // Update the rich presence
    discordP.state = dState;
    discordP.details = dDetails;
    discordP.largeImageKey = dImage;
    discordP.largeImageText = dSubtitle;
    if (dPartyId != NULL) {
        discordP.partyId = dPartyId;
    } else {
        discordP.partyId = "hahalolparty";
    }
    discordP.partySize = dPlayers;
    discordP.partyMax = dMaxPlayers;
    discordP.startTimestamp = dTime;
    if (dJoinSecret != NULL) {
        discordP.joinSecret = dJoinSecret;
    }
    if (dSpectateSecret != NULL) {
        discordP.spectateSecret = dSpectateSecret;
    }
    Discord_UpdatePresence(&discordP);

    // Return 0 arguments to lua
    return 0;
}

GMOD_MODULE_OPEN() {
    // Create the functions
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(StartDiscordStatus);
    LUA->SetField(-2, "DiscordInitialize");
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(RunDiscordCallbacks);
    LUA->SetField(-2, "DiscordRunCallbacks");
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(DiscordRespond);
    LUA->SetField(-2, "DiscordRespond");
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(UpdateDiscordStatus_Basic);
    LUA->SetField(-2, "DiscordBasic");
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(UpdateDiscordStatus_Players);
    LUA->SetField(-2, "DiscordPlayers");
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(UpdateDiscordStatus_Elapsed);
    LUA->SetField(-2, "DiscordTime");
    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE() {
    Discord_Shutdown();
    return 0;
}