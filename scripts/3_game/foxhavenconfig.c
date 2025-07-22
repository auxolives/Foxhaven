class PlayerConditionsSettings
{
    bool isPenaltyEnabled;
    void PlayerConditionsSettings() { isPenaltyEnabled = true; }
}

class TerrainSlopeSettings
{
    bool isPenaltyEnabled;
    float slopeUpWeak_deg, slopeUpMedium_deg, slopeUpStrong_deg, slopeUpCritical_deg;
    float slopeDownWeak_deg, slopeDownMedium_deg;

    void TerrainSlopeSettings()
    {
        isPenaltyEnabled = true;
        slopeUpWeak_deg = 15.0; slopeUpMedium_deg = 22.0; slopeUpStrong_deg = 31.0; slopeUpCritical_deg = 40.0;
        slopeDownWeak_deg = 36.0; slopeDownMedium_deg = 50.0;
    }
}

class WeightSettings
{
    bool isPenaltyEnabled;
    float loadPenaltyLevel1_g, loadPenaltyLevel2_g, loadPenaltyLevel3_g;

    void WeightSettings()
    {
        isPenaltyEnabled = true;
        loadPenaltyLevel1_g = 50000.0; loadPenaltyLevel2_g = 60000.0; loadPenaltyLevel3_g = 70000.0;
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
    bool isHeatBufferEnabled;
    ref MovementPenaltiesSettings movementPenaltiesSettings;

    void FoxhavenConfigData()
    {
        isHeatBufferEnabled = false;
        movementPenaltiesSettings = new MovementPenaltiesSettings();
    }
}

class FoxhavenConfig
{
    private static const string MOD_FOLDER = "$profile:Foxhaven/";
    private static const string CONFIG_PATH = MOD_FOLDER + "Config.json";

    private static ref FoxhavenConfig m_Instance;
    private ref FoxhavenConfigData m_SettingsData;

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
                }
                else
                {
                    Print("[Foxhaven] !!! Error parsing Foxhaven/Config.json: " + errorMessage);
                    Print("[Foxhaven] !!! Using default values.");
                    Print("[Foxhaven] !!! Please check your config for syntax errors.");
                    Print("[Foxhaven] !!! Please check github for update to Foxhaven/Config.json.");
                    Print("[Foxhaven] !!! Backup and delete to regenerate a clean Foxhaven/Config.json.");
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
        return m_SettingsData.isHeatBufferEnabled;
    }

    MovementPenaltiesSettings GetMovementPenaltiesSettings()
    {
        return m_SettingsData.movementPenaltiesSettings;
    }
}