# CapstoneProject

## Installation

### Windows

a) Install Unreal Engine: https://www.unrealengine.com/en-US/download

b) Install Visual Studio: https://visualstudio.microsoft.com/downloads/

c) Install Visual Studio dependencies: https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/VisualStudioSetup/index.html

d) Clone this repo and open the project solution in Visual Studio

e) Build and run the solution

Each time the solution is built and ran from the editor, all source files will be compiled and the editor will be launched.

## Configuring multiplayer

### General

The project supports two multiplayer subsystems: Null and Steam. The Null subsystem is localhost only and is 
useful for testing multiplayer functionality. The Steam subsystem is used for online multiplayer and packaged
for releases of the game.

### Null subsystem

A player can host a game and other players can join from the same machine only. This is useful for 
quickly testing multiplayer functionality.

a) Navigate to and open the project's `DefaultEngine.ini` file located at `CapstoneProject/Config/DefaultEngine.ini`

b) Search for the section `[OnlineSubsystem]` and change the value `DefaultPlatformService=Steam` to `DefaultPlatformService=Null`

c) Run the editor and modify the [Play in Editor Configuration](https://docs.unrealengine.com/5.0/en-US/play-in-editor-settings-in-unreal-engine/)
and set the number of players to 2. Run the game in **Standalone Game** mode.

d) On one of the game instances, select **Host Game** and check the **LAN** checkbox. On the other game instance, 
select **Join Game** and check the **LAN** checkbox and click the search button.

e) Connect to the game and play!

### Steam

A player can host a game and other players can join from different machines with differing Steam accounts.
This resembles the production environment.

#### Requirements

* [Steam application installed](https://store.steampowered.com/about/)
* Steam application must be running and logged in **before** running the game
* Host must be on a different network than the joining player (VPN may work as well)
* Host and joining player may need to be friends on Steam
* Host and joining player must be on different Steam accounts
* Port forward [Steam's required ports](https://help.steampowered.com/en/faqs/view/2EA8-4D75-DA21-31EB)
* Port forward [Unreal Engine's required ports](https://docs.unrealengine.com/4.26/en-US/Resources/Templates/CollabViewer/Networking/)

#### Setup

a) Navigate to and open the project's `DefaultEngine.ini` file located at `CapstoneProject/Config/DefaultEngine.ini`

b) Search for the section `[OnlineSubsystem]` and change the value `DefaultPlatformService=Null` to `DefaultPlatformService=Steam`

c) Run the editor and [package the game for Windows](https://docs.unrealengine.com/5.0/en-US/packaging-unreal-engine-projects/)

d) Distribute the packaged game to the host and joining player and run the game

e) On the host, select **Create Game** button. On the joining player, select **Join Game** and click the **Refresh** button

f) Connect to the game and play!

#### Testing on a single machine

It's possible to test multiplayer on a single machine by using a virtual machine and VPN. This is useful for testing
a release build of the game until a dedicated server is available.

* Install [VMware Workstation Player](https://www.vmware.com/products/workstation-player/workstation-player-evaluation.html)
* Create a new virtual machine with Windows
* Install a VPN on the virtual machine
* Install Steam on the virtual machine
* While using Steam app id 480, the VPN must be in the same region as the host
* Avoid network complications by making the virtual machine the joining player and the host the host

a) Copy the packaged game to the virtual machine

b) Ensure the host and virtual machine are on different Steam accounts

c) Connect to the VPN on the virtual machine (ensure region matches the host)

d) Follow steps e) and f) from the **Setup** section above, using the virtual machine as the joining player and the host as the host

#### Troubleshooting

* If the joining player cannot see the host's game, ensure no requirements are being violated.
* If attempting to host a game, ensure there isn't a firewall blocking the required ports.
* A log file is generated from the packaged game and can be found at `CapstoneProject/Saved/Logs/CapstoneProject.log`.