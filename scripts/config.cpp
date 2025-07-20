class CfgPatches
{
	class Vinalhaven_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Scripts",
			"Worlds_Vinalhaven_Data"
		};
	};
};
class CfgMods
{
	class Vinalhaven_Scripts
	{
		hideName = 1;
		hidePicture = 1;
		name = "PlayZ";
		credits = "";
		author = "auxOlives";
		type = "mod";
		dependencies[] = {
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {
					"Vinalhaven/scripts/3_game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {
					"Vinalhaven/scripts/4_world"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {
					"Vinalhaven/scripts/5_mission"
				};
			};
		};
	};
};