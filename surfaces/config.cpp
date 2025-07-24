class CfgPatches
{
	class DZ_Surfaces_Foxhaven
	{
		requiredAddons[] = {
			"Worlds_Foxhaven_Data",
			"DZ_Surfaces_Bliss",
			"DZ_Surfaces_Sakhal"
		};
	};
};
class CfgWorlds
{
	class DefaultWorld;
	class CAWorld: DefaultWorld
	{
		class DefaultClutter;
		class Clutter
		{
			class BrownGrass: DefaultClutter						{ model="dz\plants_sakhal\clutter\c_BrownGrass.p3d";																scaleMin=0.6;	scaleMax=1.1;	noSatColor=0; };
			class BrownGrass_Flat: DefaultClutter					{ model="dz\plants_sakhal\clutter\c_BrownGrass_Flat.p3d";															scaleMin=0.7;	scaleMax=1.1;	noSatColor=0; };
			class Carduus: DefaultClutter							{ model="dz\plants\clutter\c_Carduus.p3d";																			scaleMin=0.6;	scaleMax=0.9;	noSatColor=0; };
			class Carduus_f: DefaultClutter							{ model="dz\plants\clutter\c_Carduus_f.p3d";																		scaleMin=0.6;	scaleMax=0.9;	noSatColor=0; };
			class Elytrigia_summer: DefaultClutter					{ model="dz\plants_bliss\clutter\c_Elytrigia_summer.p3d";					affectedByWind=0.2;						scaleMin=0.6;	scaleMax=1.1;	noSatColor=0; };
			class ElytrigiaGreen_summer: DefaultClutter				{ model="dz\plants_bliss\clutter\c_Elytrigia_green_summer.p3d";				affectedByWind=0.2;						scaleMin=0.6;	scaleMax=1.1;	noSatColor=0; };
			class GrassDryHypericum_summer: DefaultClutter			{ model="dz\plants_bliss\clutter\c_GrassDryHypericum_summer.p3d";			affectedByWind=0.3;		swLighting=0;	scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class GrassDryCarduus_f_summer: DefaultClutter			{ model="dz\plants_bliss\clutter\c_GrassDryCarduus_f_summer.p3d";													scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersWhite_summer: DefaultClutter			{ model="dz\plants_bliss\clutter\c_GrassFlowersWhite_summer.p3d";			affectedByWind=0.2;						scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersWhiteShort_summer: DefaultClutter		{ model="dz\plants_bliss\clutter\c_GrassFlowersWhiteShort_summer.p3d";		affectedByWind=0.2;						scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersWhiteTall_summer: DefaultClutter		{ model="dz\plants_bliss\clutter\c_GrassFlowersWhiteTall_summer.p3d";		affectedByWind=0.2;						scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersYellow_summer: DefaultClutter			{ model="dz\plants_bliss\clutter\c_GrassFlowersYellow_summer.p3d";			affectedByWind=0.2;						scaleMin=0.6;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersYellowShort_summer: DefaultClutter	{ model="dz\plants_bliss\clutter\c_GrassFlowersYellowShort_summer.p3d";		affectedByWind=0.2;						scaleMin=0.6;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersYellowTall_summer: DefaultClutter		{ model="dz\plants_bliss\clutter\c_GrassFlowersYellowTall_summer.p3d";		affectedByWind=0.2;						scaleMin=0.6;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersPurple_summer: DefaultClutter			{ model="dz\plants_bliss\clutter\c_GrassFlowersPurple_summer.p3d";			affectedByWind=0.2;						scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersPurpleShort_summer: DefaultClutter	{ model="dz\plants_bliss\clutter\c_GrassFlowersPurpleShort_summer.p3d";		affectedByWind=0.2;						scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class GrassFlowersPurpleTall_summer: DefaultClutter		{ model="dz\plants_bliss\clutter\c_GrassFlowersPurpleTall_summer.p3d";		affectedByWind=0.2;						scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class Vaccinium_summer: DefaultClutter					{ model="dz\plants_bliss\clutter\c_vaccinium_summer.p3d";															scaleMin=0.8;	scaleMax=1.1;	noSatColor=0; };
			class Fern_summer: DefaultClutter						{ model="dz\plants_bliss\clutter\c_fern_summer.p3d";																scaleMin=0.8;	scaleMax=1.1;	noSatColor=1; };
			class FernTall_summer: DefaultClutter					{ model="dz\plants_bliss\clutter\c_fernTall_summer.p3d";															scaleMin=0.9;	scaleMax=1.1;	noSatColor=1; };
		};
	};
};
class CfgSurfaceCharacters
{
	class en_soil
	{
		probability[] = {
			//0.376
			0.120, //BrownGrass
			0.250, //BrownGrass_Flat
			0.003, //Carduus
			0.003  //Carduus_f
		};
		names[] = {
			"BrownGrass",
			"BrownGrass_Flat",
			"Carduus",
			"Carduus_f"
		};
	};
	class en_grass1
	{
		probability[] = {
			//1.000
			0.150, // Elytrigia_summer
			0.232, // ElytrigiaGreen_summer
			0.050, // GrassDryHypericum_summer
			0.100, // GrassDryCarduus_f_summer
			0.101, // GrassFlowersWhiteTall_summer
			0.110, // GrassFlowersWhiteShort_summer
			0.110, // GrassFlowersWhite_summer
			0.006, // GrassFlowersYellowTall_summer
			0.060, // GrassFlowersYellowShort_summer
			0.060, // GrassFlowersYellow_summer
			0.001, // GrassFlowersPurpleTall_summer
			0.010, // GrassFlowersPurpleShort_summer
			0.010  // GrassFlowersPurple_summer
		};
		names[] = {
			"Elytrigia_summer",
			"ElytrigiaGreen_summer",
			"GrassDryHypericum_summer",
			"GrassDryCarduus_f_summer",
			"GrassFlowersWhiteTall_summer",
			"GrassFlowersWhiteShort_summer",
			"GrassFlowersWhite_summer",
			"GrassFlowersYellowTall_summer",
			"GrassFlowersYellowShort_summer",
			"GrassFlowersYellow_summer",
			"GrassFlowersPurpleTall_summer",
			"GrassFlowersPurpleShort_summer",
			"GrassFlowersPurple_summer"
		};
	};
	class en_forestCon
	{
		probability[] = {
			//1.000
			0.100, // Elytrigia_summer
			0.100, // ElytrigiaGreen_summer
			0.050, // GrassDryHypericum_summer
			0.030, // GrassDryCarduus_f_summer
			0.450, // Vaccinium_summer
			0.120, // Fern_summer
			0.16 0  // FernTall_summer
		};
		names[] = {
			"Elytrigia_summer",
			"ElytrigiaGreen_summer",
			"GrassDryHypericum_summer",
			"GrassDryCarduus_f_summer",
			"Vaccinium_summer",
			"Fern_summer",
			"FernTall_summer"
		};
	};
};
class CfgSurfaces
{
	class DZ_SurfacesInt;
	class DZ_SurfacesExt;
};