#define GMMODULE
#include "GarrysMod/Lua/Interface.h"
#include "discord_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>

#define DISCORD_RPC_VERSION 121

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

LUA_FUNCTION(UpdateDiscordStatus) {
    DiscordRichPresence discordP;
    memset(&discordP, 0, sizeof(discordP));
    LUA->GetField(1, "state");
    discordP.state = LUA->GetString();
    LUA->GetField(1, "details");
    discordP.details = LUA->GetString();
    LUA->GetField(1, "startTimestamp");
    discordP.startTimestamp = LUA->GetNumber();
    LUA->GetField(1, "endTimestamp");
    discordP.endTimestamp = LUA->GetNumber();
    LUA->GetField(1, "largeImageKey");
    discordP.largeImageKey = LUA->GetString();
    LUA->GetField(1, "largeImageText");
    discordP.largeImageText = LUA->GetString();
    LUA->GetField(1, "smallImageKey");
    discordP.smallImageKey = LUA->GetString();
    LUA->GetField(1, "smallImageText");
    discordP.smallImageText = LUA->GetString();
    LUA->GetField(1, "partyId");
    discordP.partyId = LUA->GetString();
    LUA->GetField(1, "partySize");
    discordP.partySize = LUA->GetNumber();
    LUA->GetField(1, "partyMax");
    discordP.partyMax = LUA->GetNumber();
    LUA->GetField(1, "partyPrivacy");
    discordP.partyPrivacy = LUA->GetNumber();
    LUA->GetField(1, "matchSecret");
    discordP.matchSecret = LUA->GetString();
    LUA->GetField(1, "joinSecret");
    discordP.joinSecret = LUA->GetString();
    LUA->GetField(1, "spectateSecret");
    discordP.spectateSecret = LUA->GetString();
    LUA->GetField(1, "instance");
    discordP.instance = LUA->GetNumber();

    int buttonI = 0;

    LUA->GetField(1, "buttonPrimaryLabel");
    if (LUA->GetString() != NULL) {
        discordP.buttons[buttonI] = (DiscordRichPresenceButton *)malloc(
            sizeof(DiscordRichPresenceButton));

        discordP.buttons[buttonI]->label = LUA->GetString();

        LUA->GetField(1, "buttonPrimaryUrl");
        LUA->CheckString();

        discordP.buttons[buttonI]->url = LUA->GetString();

        buttonI += 1;
    }

    LUA->GetField(1, "buttonSecondaryLabel");
    if (LUA->GetString() != NULL) {
        discordP.buttons[buttonI] = (DiscordRichPresenceButton *)malloc(
            sizeof(DiscordRichPresenceButton));

        discordP.buttons[buttonI]->label = LUA->GetString();

        LUA->GetField(1, "buttonSecondaryUrl");
        LUA->CheckString();

        discordP.buttons[buttonI]->url = LUA->GetString();
    }

    Discord_UpdatePresence(&discordP);
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
    LUA->PushCFunction(UpdateDiscordStatus);
    LUA->SetField(-2, "DiscordUpdateRPC");
    LUA->Pop();

    // Set the current version of the module
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushNumber(DISCORD_RPC_VERSION);
    LUA->SetField(-2, "DiscordRPCVersion");
    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE() {
    Discord_Shutdown();
    return 0;
}
