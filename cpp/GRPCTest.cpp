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
bool cbConUserTriggerd, cbDisconnectedTriggerd, cbErrorTriggerd, cbJoinTriggerd,
    cbSpectateTriggerd, cbJoinRequestTriggerd;

static void HandleDiscordReady(const DiscordUser *connectedUser) {
    cbConUser = *connectedUser;
    cbConUserTriggerd = true;
}

static void HandleDiscordDisconnected(int errcode, const char *message) {
    cbDisconnected.first = errcode;
    cbDisconnected.second = *message;
    cbDisconnectedTriggerd = true;
}

static void HandleDiscordError(int errcode, const char *message) {
    cbError.first = errcode;
    cbError.second = *message;
    cbConUserTriggerd = true;
}

static void HandleDiscordJoin(const char *secret) {
    cbJoinSecret = *secret;
    cbJoinTriggerd = true;
}

static void HandleDiscordSpectate(const char *secret) {
    cbSpectateSecret = *secret;
    cbSpectateTriggerd = true;
}

static void HandleDiscordJoinRequest(const DiscordUser *request) {
    cbJoinRequest = *request;
    cbJoinRequestTriggerd = true;
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

    Discord_Initialize(appid, &handlers, 1, 0);

    return 0;
}

LUA_FUNCTION(RunDiscordCallbacks) {
    Discord_RunCallbacks();
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-1, "hook");
    if (cbConUserTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordReady");
        LUA->PushString(cbConUser.userId);
        LUA->PushString(cbConUser.username);
        LUA->PushString(cbConUser.discriminator);
        LUA->PushString(cbConUser.avatar);
        LUA->PushString(cbConUser.username);
        LUA->Call(6, 0);
        cbConUserTriggerd = false;
    }

    if (cbDisconnectedTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordDisconnected");
        LUA->PushNumber(cbDisconnected.first);
        LUA->PushString(&cbDisconnected.second);
        LUA->Call(3, 0);
        cbDisconnectedTriggerd = false;
    }

    if (cbErrorTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordError");
        LUA->PushNumber(cbError.first);
        LUA->PushString(&cbError.second);
        LUA->Call(3, 0);
        cbErrorTriggerd = false;
    }

    if (cbJoinTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordJoin");
        LUA->PushString(&cbJoinSecret);
        LUA->Call(2, 0);
        cbJoinTriggerd = false;
    }

    if (cbSpectateTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordSpectate");
        LUA->PushString(&cbSpectateSecret);
        LUA->Call(2, 0);
        cbSpectateTriggerd = false;
    }

    if (cbJoinRequestTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordJoinRequest");
        LUA->PushString(cbJoinRequest.userId);
        LUA->PushString(cbJoinRequest.username);
        LUA->PushString(cbJoinRequest.discriminator);
        LUA->PushString(cbJoinRequest.avatar);
        LUA->PushString(cbJoinRequest.username);
        LUA->Call(6, 0);
        cbJoinRequestTriggerd = false;
    }

    LUA->Pop(2);

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
    LUA->SetField(-2, "DiscordRPCInitialize");
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(RunDiscordCallbacks);
    LUA->SetField(-2, "DiscordRPCRunCallbacks");
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(DiscordRespond);
    LUA->SetField(-2, "DiscordRPCRespond");
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

GMOD_MODULE_CLOSE() {
    Discord_Shutdown();
    return 0;
}