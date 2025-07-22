modded class MissionBase
{
	override void InitialiseWorldData()
	{
		string worldName = "empty";
		GetGame().GetWorldName(worldName);
		worldName.ToLower();

		PrintFormat("[Foxhaven Logs] Detected world name: %1", worldName);

		switch (worldName)
		{
			case "vinalhaven":
				Print("[Foxhaven Logs] Initializing Vinalhaven world data");
				m_WorldData = new VinalhavenData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;

			case "chernarusplus":
				Print("[Foxhaven Logs] Initializing ChernarusPlus world data");
				m_WorldData = new ChernarusPlusData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistryChernarus();
				break;

			case "enoch":
				Print("[Foxhaven Logs] Initializing Enoch world data");
				m_WorldData = new EnochData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistryEnoch();
				break;

			case "sakhal":
				Print("[Foxhaven Logs] Initializing Sakhal world data");
				m_WorldData = new SakhalData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistrySakhal();
				break;

			#ifdef PLATFORM_CONSOLE
			case "mainmenuscenexbox":
				Print("[Foxhaven Logs] Initializing MainMenu Xbox Scene world data");
				m_WorldData = new MainMenuWorldData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;
			#endif

			default:
				PrintFormat("[Foxhaven Logs] Unknown world '%1', falling back to ChernarusPlus", worldName);
				m_WorldData = new ChernarusPlusData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;
		}
	}
}
