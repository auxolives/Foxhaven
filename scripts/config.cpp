class CfgPatches
{
	class Foxhaven_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Scripts",
			"Worlds_Foxhaven_Data"
		};
	};
};
class CfgMods
{
	class Foxhaven_Scripts
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
					"Foxhaven/scripts/3_game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {
					"Foxhaven/scripts/4_world"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {
					"Foxhaven/scripts/5_mission"
				};
			};
		};
	};
};