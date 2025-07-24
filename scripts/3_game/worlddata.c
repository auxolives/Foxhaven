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