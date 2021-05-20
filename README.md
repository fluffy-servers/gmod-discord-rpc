# GDiscord
#### Discord Rich Presence for Garry's Mod

This addon provides Discord RPC support for Garry's Mod, through the use of a binary module. For this addon to work, a Lua script is required on servers to sync information, and a binary module is required on clients.

In June 2018, Discord broke updating this through HTTP; so any Lua-based implementations of this are no longer possible. However, it is still possible (however impractical for servers) to use Rich Presence by using a binary module.

## For Clients
Download the latest version of the GDiscord module from the [releases page](https://github.com/fluffy-servers/gmod-discord-rpc/releases). 
The module you need depends on what OS and version you're running (eg. Windows on 32-bit should download **gmcl_gdiscord_win32.dll**). If you're running the 64-bit beta, make sure you download the 64.dll!

Copy this into **lua/bin/**. That's all!

With this module installed, your Discord status should automatically update when playing on supported servers.

## For Servers
Please note that for this to work, **all** clients *will* need to install the binary module.

Download the latest version of cl_discord.lua from the [releases page](https://github.com/fluffy-servers/gmod-discord-rpc/releases).

Copy this into **lua/autorun/client**.

### Configuration
For best results, server owners need to edit cl_discord.lua for server-specific configuration.

You ***should not*** edit or rename the binary module. Please keep this as gdiscord, so clients can use one module across all supported servers!

#### Setting up an Application
The first step is to set up a Discord application so you can slightly customise the display. This application contains the map images that will be displayed in Discord.

Create an application over at https://discordapp.com/developers/

Go into Rich Presence > Art Assets and upload images of each map you want, along with an optional default image. Ensure that each map is named correctly.

Copy the Client ID from General Information.

#### Lua File
Open up the .lua file and change the following variables as needed:
 - *discord_id* should be the Client ID you obtained earlier
 - *refresh_time* should be how frequently you want the display to update
 
#### Default Map Image
To display a default map image, the script needs to know which maps you have specific icons for. There's no way for the Lua script to know what you've uploaded to Discord, so this is a manual process. Sorry.

Put all the maps you have images for into the *map_list* table. Once that's done, change *map_restrict* to true, and set *map_default* to the name of whatever the default icon should be.

**If you have images uploaded for every map in your rotation, you don't need to worry about this step.**
