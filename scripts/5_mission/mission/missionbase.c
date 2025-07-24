modded class MissionBase
{
	override void InitialiseWorldData()
	{
		string worldName = "empty";
		GetGame().GetWorldName(worldName);
		worldName.ToLower();

		if (GetGame().IsServer())
		{
			PrintFormat("[Foxhaven] Detected world name: %1", worldName);
		}

		switch (worldName)
		{
			case "foxhaven":
				if (GetGame().IsServer())
				{
					Print("[Foxhaven] Initializing Foxhaven world data");
				}
				m_WorldData = new FoxhavenData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;

			case "chernarusplus":
				if (GetGame().IsServer())
				{
					Print("[Foxhaven] Initializing ChernarusPlus world data");
				}
				m_WorldData = new ChernarusPlusData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistryChernarus();
				break;

			case "enoch":
				if (GetGame().IsServer())
				{
					Print("[Foxhaven] Initializing Enoch world data");
				}
				m_WorldData = new EnochData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistryEnoch();
				break;

			case "sakhal":
				if (GetGame().IsServer())
				{
					Print("[Foxhaven] Initializing Sakhal world data");
				}
				m_WorldData = new SakhalData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistrySakhal();
				break;

			#ifdef PLATFORM_CONSOLE
			case "mainmenuscenexbox":
				if (GetGame().IsServer())
				{
					Print("[Foxhaven] Initializing MainMenu Xbox Scene world data");
				}
				m_WorldData = new MainMenuWorldData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;
			#endif

			default:
				if (GetGame().IsServer())
				{
					PrintFormat("[Foxhaven] Unknown world '%1', falling back to ChernarusPlus", worldName);
				}
				m_WorldData = new ChernarusPlusData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;
		}
	}
}
