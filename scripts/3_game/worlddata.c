//================================================================
//  WORLD WEATHER SETTINGS
//================================================================
//  • DESCRIPTION: This script modifies the world data to adjust
//    weather settings. It specifically disable volumetric fog height bias.
//
//  • MOD COMPATIBILITY: This mod overrides the custom Foxhaven method
//    'SetupWeatherSettings' in the 'WorldData' class. As this is
//    part of the internal mod framework, it poses no risk of conflict
//    with other mods. It also modifies the vanilla
//    'WorldDataWeatherSettings' class.
//================================================================
modded class WorldData
{
    override void SetupWeatherSettings()
    {
        m_WeatherDefaultSettings = new WorldDataWeatherSettings();
    }
};

modded class WorldDataWeatherSettings
{
    int m_FoggyMorningHeigthBiasLowLimit = 0;
    int m_DefaultHeigthBias = 0;
}
