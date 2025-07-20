modded class MissionBase
{
	override void InitialiseWorldData()
	{
		string worldName = "empty";
		GetGame().GetWorldName(worldName);
		worldName.ToLower();
		
		switch (worldName)
		{
			case "Vinalhaven":
				m_WorldData = new VinalhavenData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;

			case "chernarusplus":
				m_WorldData = new ChernarusPlusData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistryChernarus();
				break;

			case "enoch":
				m_WorldData = new EnochData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistryEnoch();
				break;

			case "sakhal":
				m_WorldData = new SakhalData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistrySakhal();
				break;
			
			#ifdef PLATFORM_CONSOLE
			case "mainmenuscenexbox":
				m_WorldData = new MainMenuWorldData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break
			#endif

			default:
				m_WorldData = new ChernarusPlusData();
				m_DynamicMusicPlayerRegistry = new DynamicMusicPlayerRegistry();
				break;
		}
	}
}