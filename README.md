# CapstoneProject

## Installation and setup

### Windows

a) Install Unreal Engine: https://www.unrealengine.com/en-US/download

b) Install Visual Studio: https://visualstudio.microsoft.com/downloads/

c) Install Visual Studio dependencies: https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/VisualStudioSetup/index.html

d) Clone this repo and navigate to the project directory

e) Right click on `CapstoneProject.uproject` and select **Generate Visual Studio project files**

f) Open the solution in Visual Studio

e) Build and run the solution

Each time the solution is built and ran from the editor, all source files will be compiled and the editor will be launched.

## Contributing

In order to contribute to the project, you must first fork the project and clone it to your local machine.
Once you have cloned the project, you can make changes and push them to your fork. Once you are ready to
merge your changes into the main project, you can create a pull request. The pull request will be reviewed
and merged into the main project.

When making changes to blueprints, please include a screenshot of the blueprint in your pull request. This
will help reviewers understand the changes you have made.

## Packaging the game

a) Open the project in the editor

b) Navigate to **File** > **Package Project** > **Windows** > **Windows (64-bit)**

c) Select a directory to package the game to

d) Wait for the packaging process to complete

e) Navigate to the packaged game directory and run the executable

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

## Networking

### General

The project uses Unreal Engine's [Online Subsystem](https://docs.unrealengine.com/5.0/en-US/online-subsystem-in-unreal-engine/).
The subsystem is used to create a session, find a session, and join a session and destroy a session. Beyond this, Unreal
Engine's built-in networking is used to replicate actors and components as well as broadcast events and other logic
to all connected players.

### Blueprints and C++

It's important to note that the project is implemented using Unreal Engine's blueprint system. 
Unreal Engine's blueprint system is a visual scripting language that is compiled into binaries, only 
viewable from within the Unreal Engine editor. The blueprint files are located from within the project's
`Content` directory. 

C++ handles some of the more complex logic of the project, such as networking and session management, but
the blueprints themselves are responsible for orchestrating the logic.


### Sessions

All session logic is defined in the `SessionGameInstance` source files and is a child of the UGameInstance 
class provided by Unreal Engine. Implementing Unreal Engine' Session Interface libraries, the `SessionGameInstance`
is responsible for creating a session, finding a session, joining a session, and destroying a session. 
SessionGameInstance is referenced throughout the project where session-related logic is required, such as
the main menu and the game mode.

## HUDs

The project uses Unreal Engine's [Widget's system](https://docs.unrealengine.com/5.0/en-US/creating-widgets-in-unreal-engine/)
to create HUDs. Widgets can all be found from within the project's `Content/UI` directory.
The following is a list of the HUD blueprints implemented in the project:

* `WB_MainMenuScreen` is the widget used for the main menu. It contains the buttons for starting a game, joining a game, and exiting the game
* `WB_SessionList` is the widget that structures the list of all available servers when searching for a game
* `WB_SessionListing` is the widget that structures the layout of an individual listings when searching for a game
* `WB_PauseMenuScreen` is a widget that is displayed when the player pauses the game. 
* `WB_ScoreboardHUD` is a widget that structures the layout of game statistics. It displays a list of
all players and their individual statistics, the map name, server name, and remaining time.
* `WB_ScoreboardEntry` is a widget that structures the layout of an individual player's statistics
which includes their name, kills, deaths, and ping.
It contains the buttons for resuming the game and returning to the main menu
* `FP_HUD` is the widget that displays the player's health, ammo, and score

## BP_FirstPersonCharacter
 * Integral Blueprint that is responsible for or tangent to most things related to each player character including:
    * `Character` and `Camera` movement
    * `Gun` (under ownership of the respective player) related actions such as sprinting, aiming, firing, reloading, pickup 
    * Player health and death
 * These operations are well organized within the BluePrint file and employ RPCs (Remote Procedure Calls) to ensure the server has total authority over all player actions. 

## Animations
### General
* Animation suite is based off enhanced input actions from `BP_FirstPersonCharacter` variables are passed into the event graph to calculate direction, velocity and trigger simple booleans such as `IsAiming`.
* These variables are passed into `Locomotion` which determines anim state and blends between multiple BlendSpaces to achieve smooth transitions between animation types.
### BlendSpaces 
* `Idle/Walk/Run` `Aim` and `Crouching` all have their own BlendSpaces. 
* `Dead` and `Jump` are simple animations but can be blended to from other BlendSpace states.
### Retargeting 
* Animations can be added from the included UE4 standard `Animation Starter Pack` but will need to be retargeted to UE5 to properly work.

