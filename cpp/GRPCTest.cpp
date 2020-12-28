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
bool cbConUserTriggered, cbDisconnectedTriggered, cbErrorTriggered,
    cbJoinTriggered, cbSpectateTriggered, cbJoinRequestTriggered;

static void HandleDiscordReady(const DiscordUser *connectedUser) {
    cbConUser = *connectedUser;
    cbConUserTriggered = true;
}

static void HandleDiscordDisconnected(int errcode, const char *message) {
    cbDisconnected.first = errcode;
    cbDisconnected.second = *message;
    cbDisconnectedTriggered = true;
}

static void HandleDiscordError(int errcode, const char *message) {
    cbError.first = errcode;
    cbError.second = *message;
    cbConUserTriggered = true;
}

static void HandleDiscordJoin(const char *secret) {
    cbJoinSecret = *secret;
    cbJoinTriggered = true;
}

static void HandleDiscordSpectate(const char *secret) {
    cbSpectateSecret = *secret;
    cbSpectateTriggered = true;
}

static void HandleDiscordJoinRequest(const DiscordUser *request) {
    cbJoinRequest = *request;
    cbJoinRequestTriggered = true;
}

LUA_FUNCTION(StartDiscordStatus) {
    const char *discordAppId = LUA->GetString(1);

    // Discord RPC
    DiscordEventHandlers handlers{};
    handlers.ready = HandleDiscordReady;
    handlers.disconnected = HandleDiscordDisconnected;
    handlers.errored = HandleDiscordError;
    handlers.joinGame = HandleDiscordJoin;
    handlers.spectateGame = HandleDiscordSpectate;
    handlers.joinRequest = HandleDiscordJoinRequest;

    Discord_Initialize(discordAppId, &handlers, 1, "4000");
    return 0;
}

LUA_FUNCTION(RunDiscordCallbacks) {
    Discord_RunCallbacks();
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-1, "hook");
    if (cbConUserTriggered) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordReady");
        LUA->PushString(cbConUser.userId);
        LUA->PushString(cbConUser.username);
        LUA->PushString(cbConUser.discriminator);
        LUA->PushString(cbConUser.avatar);
        LUA->Call(5, 0);
        cbConUserTriggered = false;
    }

    if (cbDisconnectedTriggered) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordDisconnected");
        LUA->PushNumber(cbDisconnected.first);
        LUA->PushString(&cbDisconnected.second);
        LUA->Call(3, 0);
        cbDisconnectedTriggered = false;
    }

    if (cbErrorTriggered) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordError");
        LUA->PushNumber(cbError.first);
        LUA->PushString(&cbError.second);
        LUA->Call(3, 0);
        cbErrorTriggered = false;
    }

    if (cbJoinTriggered) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordJoin");
        LUA->PushString(&cbJoinSecret);
        LUA->Call(2, 0);
        cbJoinTriggered = false;
    }

    if (cbSpectateTriggered) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordSpectate");
        LUA->PushString(&cbSpectateSecret);
        LUA->Call(2, 0);
        cbSpectateTriggered = false;
    }

    if (cbJoinRequestTriggered) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordJoinRequest");
        LUA->PushString(cbJoinRequest.userId);
        LUA->PushString(cbJoinRequest.username);
        LUA->PushString(cbJoinRequest.discriminator);
        LUA->PushString(cbJoinRequest.avatar);
        LUA->Call(5, 0);
        cbJoinRequestTriggered = false;
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
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->CreateTable();

    LUA->PushCFunction(StartDiscordStatus);
    LUA->SetField(-2, "Initialize");
    LUA->PushCFunction(RunDiscordCallbacks);
    LUA->SetField(-2, "RunCallbacks");
    LUA->PushCFunction(DiscordRespond);
    LUA->SetField(-2, "Respond");
    LUA->PushCFunction(UpdateDiscordStatus_Basic);
    LUA->SetField(-2, "RPCBasic");
    LUA->PushCFunction(UpdateDiscordStatus_Players);
    LUA->SetField(-2, "RPCPlayers");
    LUA->PushCFunction(UpdateDiscordStatus_Elapsed);
    LUA->SetField(-2, "RPCTime");

    LUA->PushNumber(0);
    LUA->SetField(-2, "DISCORD_REPLY_NO");
    LUA->PushNumber(1);
    LUA->SetField(-2, "DISCORD_REPLY_YES");
    LUA->PushNumber(2);
    LUA->SetField(-2, "DISCORD_REPLY_IGNORE");

    LUA->SetField(-2, "discord");
    LUA->Pop();
    return 0;
}

GMOD_MODULE_CLOSE() {
    Discord_Shutdown();
    return 0;
}