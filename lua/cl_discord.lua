-- This requires a special module to be installed before it works correctly
-- Sorry to disappoint you
if file.Find("lua/bin/gmcl_gdiscord_*.dll", "GAME")[1] == nil then return end
require("gdiscord")

-- Configuration
local map_restrict = false -- Should a display default image be displayed if the map is not in a given list?
local map_list = {
    gm_flatgrass = true,
    gm_construct = true
}
local map_default = "default"
local discord_id = "626155559779041331"
local refresh_time = 60

local discord_start = discord_start or -1

function DiscordUpdate()
    -- Determine what type of game is being played
    local playing_message = ""
    if game.SinglePlayer() then
        playing_message = "Singleplayer"
    else
        local ip = game.GetIPAddress()
        if ip == "loopback" then
            if GetConVar("p2p_enabled"):GetBool() then
                playing_message = "Peer 2 Peer"
            else
                playing_message = "Local Server"
            end
        else
            playing_message = string.Replace(ip, ":27015", "")
        end
    end

    -- Determine the max number of players
    local maxplayers = game.MaxPlayers()
    if game.SinglePlayer() then maxplayers = 0 end

    -- Handle map stuff
    -- See the config
    local map = game.GetMap()
    if map_restrict and not map_list[map] then
        map = map_default
    end
    local gamemode = GAMEMODE.Name

    DiscordRPCTime(playing_message, gamemode, map, game.GetMap(), player.GetCount(), maxplayers, discord_start)
end

hook.Add("Initialize", "UpdateDiscordStatus", function()
    discord_start = os.time()
    DiscordRPCInitialize(discord_id)
    DiscordUpdate()

    timer.Create("DiscordRPCTimer", refresh_time, 0, DiscordUpdate)
end)
