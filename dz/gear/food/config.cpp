class CfgPatches
{
	class DZ_Gear_Food_Foxhaven
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Gear_Food"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Edible_Base;
	class MushroomBase;
	class AmanitaMushroom: MushroomBase
	{
		inventorySlot[] = {"Ingredient","DirectCookingA","DirectCookingB","DirectCookingC","SmokingA","SmokingB","SmokingC","SmokingD","Trap_Bait_2"};
		class Food
		{
			class FoodStages
			{
				class Raw
				{
					nutrition_properties[] = {2.5, 120, 150, 1, 160, 16, 0.9, 3};
				};
				class Rotten
				{
					nutrition_properties[] = {2.0, 90, 40, 1, 190, 16, 0.7, 4};
				};
				class Baked
				{
					nutrition_properties[] = {1.75, 300, 60, 1, 60, 0, 1.0, 0};
				};
				class Boiled
				{
					nutrition_properties[] = {1.5, 250, 160, 1, 40, 0, 1.1, 0};
				};
				class Dried
				{
					nutrition_properties[] = {0.75, 250, 20, 1, 55, 0, 0.7, 0};
				};
				class Burned
				{
					nutrition_properties[] = {2.0, 90, 0, 1, 100, 16, 0.6, 2};
				};
			};
		};
		class InventorySlotsOffsets
		{
			class DirectCookingA
			{
				position[] = {0.08,0.01,0.0};
				orientation[] = {100,65,0};
			};
			class DirectCookingB
			{
				position[] = {0.04,0.01,0.0};
				orientation[] = {30,65,0};
			};
			class DirectCookingC
			{
				position[] = {0.06,0.01,0.0};
				orientation[] = {140,65,0};
			};
		};
	};
};