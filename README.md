# Garry's Mod Discord Rich Presence
 Provides Discord Rich Presence support to Garry's Mod
 
 In June 2018, Discord broke updating this through HTTP; so any Lua-based implementations of this are no longer possible. However, it is still possible (however impractical for servers) to use Rich Presence by using a binary module.

 ## Installation
 ### Binary Module (Clients)
 Clients need to install both *gmcl_fluffydiscord_win32.dll* and Discord's own *discord-rpc.dll*
 
 *gmcl_fluffydiscord_win32.dll* needs to go in **lua/bin/**
 
 *discord-rpc.dll* needs to go in the **root directory** (next to hl2.exe)
 
 Grab a copy of Discord RPC from https://github.com/discordapp/discord-rpc/releases

 discord-rpc.dll can be found in win32-dynamic/bin
 
 ### LUA Addon (Servers)
 Servers need to install a LUA script that syncs the game details to Discord.
 This can optionally be installed by players for support of single player games
 
 This .lua file needs to go in lua/autorun/client/
 
 ## Configuration
 ### Setting up an Application
 The first step is to set up a Discord application so you can slightly customise the display
 
 Create an application over at https://discordapp.com/developers/
 
 Go into Rich Presence > Art Assets and upload images of each map you want, along with an optional default image. Ensure that each map is named correctly.
 
 Copy the Client ID from General Information.
 
 ### Configuration
 Open up the .lua file and change the following variables as needed:
  - *discord_id* should be the Client ID you obtained earlier
  - *refresh_time* should be how frequently you want the display to update
  
  ### Default Map Image
  In order to display a default map image, you'll need to painfully create a list of which maps you have icons for.
  Put them into the *map_list* table.
  
  Once that's done, ensure that *map_restrict* is set to true. Change *map_default* to the name of whatever the default icon should be.
 
 ## Building the .dll
 I'll write this section when Visual Studio likes me :(
