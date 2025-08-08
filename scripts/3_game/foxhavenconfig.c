class PlayerSettings
{
    bool isHeatBufferEnabled;
    bool isFoodandDrinkRestrictionEnabled;
    bool isRestrictedBagAccessEnabled;

    void PlayerSettings()
    {
        isHeatBufferEnabled = false;
        isFoodandDrinkRestrictionEnabled = true;
        isRestrictedBagAccessEnabled = true;
    }
}

class WeatherSettings
{
    int ClearWeatherChance_Percent;
    int CloudyWeatherChance_Percent;
    int BadWeatherChance_Percent;
    float StormThreshold;
    
    bool IsFogEnabled;
    int FogChance;
    float MinFog;
    float MaxFog;

    void WeatherSettings()
    {
        ClearWeatherChance_Percent = 40;
        CloudyWeatherChance_Percent = 35;
        BadWeatherChance_Percent = 25;
        StormThreshold = 0.9;
        
        IsFogEnabled = true;
        FogChance = 15;
        MinFog = 0.05;
        MaxFog = 1.0;
    }
}

class PlayerConditionsSettings
{
    bool isEnergyPenaltyEnabled;
    void PlayerConditionsSettings() { isEnergyPenaltyEnabled = true; }
}

class TerrainSlopeSettings
{
    bool isSlopePenaltyEnabled;
    float slopeUpWeak_degrees, slopeUpMedium_degrees, slopeUpStrong_degrees, slopeUpCritical_degrees;
    float slopeDownWeak_degrees, slopeDownMedium_degrees;

    void TerrainSlopeSettings()
    {
        isSlopePenaltyEnabled = true;
        slopeUpWeak_degrees = 12.0; slopeUpMedium_degrees = 20.0; slopeUpStrong_degrees = 28.0; slopeUpCritical_degrees = 36.0;
        slopeDownWeak_degrees = 24.0; slopeDownMedium_degrees = 32.0;
    }
}

class WeightSettings
{
    bool isWeightPenaltyEnabled;
    float loadPenaltyLevel1_grams, loadPenaltyLevel2_grams, loadPenaltyLevel3_grams;

    void WeightSettings()
    {
        isWeightPenaltyEnabled = true;
        loadPenaltyLevel1_grams = 50000.0; loadPenaltyLevel2_grams = 60000.0; loadPenaltyLevel3_grams = 70000.0;
    }
}

class MovementPenaltiesSettings
{
    ref PlayerConditionsSettings playerEnergyWaterBloodSettings;
    ref TerrainSlopeSettings terrainSlopeSettings;
    ref WeightSettings weightSettings;

    void MovementPenaltiesSettings()
    {
        playerEnergyWaterBloodSettings = new PlayerConditionsSettings();
        terrainSlopeSettings = new TerrainSlopeSettings();
        weightSettings = new WeightSettings();
    }
}

class FoxhavenConfigData
{
    string ConfigVersion;
    ref WeatherSettings weatherSettings;
    ref PlayerSettings playerSettings;
    ref MovementPenaltiesSettings movementPenaltiesSettings;

    void FoxhavenConfigData()
    {
        weatherSettings = new WeatherSettings();
        playerSettings = new PlayerSettings();
        movementPenaltiesSettings = new MovementPenaltiesSettings();
    }
}

class FoxhavenConfig
{
    private static const string MOD_FOLDER = "$profile:Foxhaven/";
    private static const string CONFIG_PATH = MOD_FOLDER + "Config.json";
    private static const string CURRENT_CONFIG_VERSION = "0.34";

    private static ref FoxhavenConfig m_Instance;
    private ref FoxhavenConfigData m_SettingsData;

	PlayerSettings GetPlayerSettings()
	{
		return m_SettingsData.playerSettings;
	}

    static FoxhavenConfig GetInstance()
    {
        if (!m_Instance)
            m_Instance = new FoxhavenConfig();
        return m_Instance;
    }

    private void FoxhavenConfig()
    {
        m_SettingsData = new FoxhavenConfigData();
    }
    
    void Load()
    {
        if (!GetGame().IsServer())
            return;

        if (FileExist(CONFIG_PATH))
        {
            JsonSerializer serializer = new JsonSerializer();
            string fileContent, errorMessage;
            
            FileHandle handle = OpenFile(CONFIG_PATH, FileMode.READ);
            if (handle != 0)
            {
                string line_content;
                while (FGets(handle, line_content) > 0)
                {
                    fileContent += line_content;
                }
                
                CloseFile(handle);
                
                if (serializer.ReadFromString(m_SettingsData, fileContent, errorMessage))
                {
                    Print("[Foxhaven] Config.json loaded successfully.");

                    if (m_SettingsData.ConfigVersion != CURRENT_CONFIG_VERSION)
                    {
                        Print("[Foxhaven] !!! WARNING: Your Config.json is outdated or missing values.");
                        Print("[Foxhaven] !!! It is recommended to delete the old config to allow a new one to be generated.");
                        Print("[Foxhaven] !!! Using default values for any missing settings.");
                    }
                }
                else
                {
                    Print("[Foxhaven] !!! Error parsing Foxhaven/Config.json: " + errorMessage);
                    Print("[Foxhaven] !!! Using default values.");
                }
            }
        }
        else
        {
            Print("[Foxhaven] Config.json not found, creating default config.");
            CreateDefaultConfig();
        }
    }
    
    private void CreateDefaultConfig()
    {
        if (!FileExist(MOD_FOLDER))
        {
            MakeDirectory(MOD_FOLDER);
        }
        
        m_SettingsData = new FoxhavenConfigData();
        m_SettingsData.ConfigVersion = CURRENT_CONFIG_VERSION;

        JsonSerializer serializer = new JsonSerializer();
        string fileContent;
        serializer.WriteToString(m_SettingsData, true, fileContent);

        FileHandle handle = OpenFile(CONFIG_PATH, FileMode.WRITE);
        if (handle != 0)
        {
            FPrintln(handle, fileContent);
            CloseFile(handle);
        }
    }

    bool IsHeatBufferEnabled()
    {
        return m_SettingsData.playerSettings.isHeatBufferEnabled;
    }

    MovementPenaltiesSettings GetMovementPenaltiesSettings()
    {
        return m_SettingsData.movementPenaltiesSettings;
    }

    WeatherSettings GetWeatherSettings()
    {
        return m_SettingsData.weatherSettings;
    }
}