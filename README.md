# Foxhaven

Welcome to Foxhaven, a custom DayZ map featuring a summer-themed terrain that blends a Vanilla-style atmosphere with all the DayZ features, new custom ones and unique landmarks like islands, volcanoes, and interior seas.

<p align="center">
  <img src="https://github.com/user-attachments/assets/663068bf-b2f9-4c70-a903-f24ddefceb7d" alt="Foxhaven Screenshot 1" width="32%"/>
  <img src="https://github.com/user-attachments/assets/560cf8ea-bda9-46e6-ad58-4ae884bb0885" alt="Foxhaven Screenshot 2" width="32%"/>
  <img src="https://github.com/user-attachments/assets/540228a7-df0c-40d7-8e10-cef75dcf1ae6" alt="Foxhaven Screenshot 3" width="32%"/>
  <img src="https://github.com/user-attachments/assets/74aa5e3e-fe48-4659-afc4-a61427318453" alt="Foxhaven Screenshot 4" width="65%"/>
  <img src="https://github.com/user-attachments/assets/e958ef1b-fb62-4d35-8441-653072801d30" alt="Foxhaven Screenshot 5" width="32%"/>
</p>

---

## Who is this GitHub for?

> ### **❌ For Players**
> This GitHub repository is intended for server owners and modders. If you're a player looking for servers, news, and community, please join our Discord!
>
> **[➡️ Join the Official Foxhaven Discord](https://discord.gg/SgNhCNtb5N)**

> ### **✅ For Server Admins & Modders**
> Welcome! This repository contains all the necessary files and documentation to set up and customize your own Foxhaven server. We highly recommend joining the Discord as well!
> 
> **[➡️ Join the Official Foxhaven Discord](https://discord.gg/SgNhCNtb5N)**

---

## Server Setup Guide (Beta Phase)

This guide will walk you through setting up the core gameplay mechanics for your Foxhaven server. As the map is in beta, the primary configuration is handled through the `Config.json` file. Future updates will include distinct mission files, central economy files, and more, which will be added to this repository.

### Step 1: Choose Your Gameplay Preset

Foxhaven comes with two main presets. Choose the one that best fits your server's desired experience.

* **Hardcore:** Scarce loot, more difficult dynamic events and enables custom scripts like penalties for low stats, steep terrain, and carrying heavy loads. This is the recommended default.
* **Casual:** Normal loot, central economy and dynamic events. Disables custom scripts. This provides an experience closer to vanilla DayZ.

> Of course, feel free to customize it as you see fit

### Step 2: Setup your serverDZ.cfg

Your serverDZ.cfg needs to be concomitent to your server init.c time management. By default, Foxhaven is meant to boot on July 1st and will circle back every July 21st. You are free to change this to change sunrise, sunset times, sun angle, and temperatures (configurable in cfggameplay.json ; see step 3).

### Step 3: Copy the Mission Files of your Choice

Copy the entire `mpmissions/dayzOffline.foxhaven/` folder [🌐 from your chosen preset](https://github.com/auxolives/Foxhaven/tree/main/server) and past it in your server's `mpmissions/` folder.

> No weather.xml? Yes, the weather is handled in the `profile/Foxhaven/config.json` ; see step 4.

### Step 4: Configure `Config.json`

Copy the entire `profiles/Foxhaven/` folder [🌐 from your chosen preset](https://github.com/auxolives/Foxhaven/tree/main/server) and past it in your server's `profiles/` folder. Or just launch the server once to automatically generate it and adjust it accordingly.

#### Important Configuration Notes

* **Error Handling:** If your `Config.json` has a syntax error, the server will **not** crash. It will log the error and load default values for that session. To fix this, you can delete your `Config.json` to have a clean one generated on the next restart.
* **Manual Updates:** This configuration file does **not** update automatically. When the map receives an update, please check back here for any new parameters to add to your file. If new parameters are missing, it will load their default hardcore values.

---

## Advanced Customization: `Config.json` Parameters Explained

If you want to fine-tune the gameplay mechanics beyond the presets, use the detailed explanations below.

### `ConfigVersion`

This is an internal version number for the config file. If you see a warning in your server logs about the config being outdated, it's because this version doesn't match the latest version in the mod. To fix this, simply delete your `Config.json` and restart the server to generate a new, updated one.

### `isHeatBufferEnabled`

This setting toggles the vanilla "heat buffer" mechanic, which allows players to build up a temporary resistance to cold.

* **`0` or `false`**: The heat buffer mechanic is **disabled**.
* **`1` or `true`**: The heat buffer mechanic is **enabled** (vanilla behavior).

### `movementPenaltiesSettings`

This section contains all settings related to penalties that affect a player's movement and injury animation level.

#### `playerEnergyWaterBloodSettings`

* **`isPenaltyEnabled`**: Toggles penalties from low character stats. When enabled, low blood, energy, or water will increase the player's "injury" level, making them move as if they were injured.

#### `terrainSlopeSettings`

* **`isPenaltyEnabled`**: Toggles penalties from moving up or down steep terrain.
* **`slope..._deg`**: These values are the **angle thresholds in degrees** at which the player's animation level increases. Higher angles result in more severe movement penalties.
  * `slopeUp...`: Penalties for moving **uphill**.
  * `slopeDown...`: Penalties for moving **downhill**.

#### `weightSettings`

* **`isPenaltyEnabled`**: Toggles penalties from carrying a heavy load.
* **`loadPenaltyLevel..._g`**: These values are the **total character weight thresholds in grams** at which the player receives an increasing movement penalty.
  * `1000g` = `1kg`.
  * Default `Level 1` is **50kg**.

### `weatherSettings`

This section controls the dynamic weather system.

* **`ClearWeatherChance_Percent`**, **`CloudyWeatherChance_Percent`**, **`BadWeatherChance_Percent`**:
  * These are the percentage chances for each major weather type to be chosen during a weather change.
  * The sum of these three values should equal **100**. If it doesn't, Foxhaven will automatically adjust them to maintain the correct proportions.

* **`StormThreshold`**:
  * The overcast value (from `0.0` to `1.0`) at which storms (heavy rain, lightning) can begin.
  * A value of `0.9` means storms can only occur when the overcast is 90% or higher.

* **`IsFogEnabled`**:
  * A simple toggle for the random fog system.
  * **`true`**: Random fog events can occur.
  * **`false`**: Random fog events are disabled. Note that rain will still generate a small amount of atmospheric fog.

* **`FogChance`**:
  * The percentage chance for a random fog event to occur during a weather cycle that isn't rainy.

* **`MinFog`** & **`MaxFog`**:
  * These settings control the density of random fog when it appears, ranging from `0.0` (clear) to `1.0` (very dense). Keep in mind that Foxhaven uses volumetric fog with has a very steep exponential curve—visually (Engine), increasing from `0.0` to `0.1` has a similar impact as going from `0.1` to `1.0`.
  * `MinFog` sets the lower bound for how light the fog can be.
  * `MaxFog` sets the upper bound for how dense it can be.
  * Equal values will always render the same amount of fog.
