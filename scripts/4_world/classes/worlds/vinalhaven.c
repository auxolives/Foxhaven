enum EModdedWeatherScenario
{
    CLEAR,
    FOGGY,
    LIGHT_RAIN,
    MEDIUM_RAIN,
    HEAVY_RAIN
};

class VinalhavenData : WorldData
{
    private float SCENARIO_OVERCAST_CLEAR           = 0.15;
    private float SCENARIO_OVERCAST_FOGGY           = 0.20;
    private float SCENARIO_OVERCAST_LIGHT_RAIN      = 0.45;
    private float SCENARIO_OVERCAST_MEDIUM_RAIN     = 0.55;
    private float SCENARIO_OVERCAST_HEAVY_RAIN      = 0.65;

    private float SCENARIO_RAINFALL_CLEAR           = 0.0;
    private float SCENARIO_RAINFALL_FOGGY           = 0.0;
    private float SCENARIO_RAINFALL_LIGHT_RAIN      = 0.05;
    private float SCENARIO_RAINFALL_MEDIUM_RAIN     = 0.50;
    private float SCENARIO_RAINFALL_HEAVY_RAIN      = 1.00;

    private float SCENARIO_VOLFOG_CLEAR             = 0.00;
    private float SCENARIO_VOLFOG_FOGGY             = 0.30;
    private float SCENARIO_VOLFOG_LIGHT_RAIN        = 0.005;
    private float SCENARIO_VOLFOG_MEDIUM_RAIN       = 0.01;
    private float SCENARIO_VOLFOG_HEAVY_RAIN        = 0.02;

    protected EModdedWeatherScenario m_CurrentScenario = EModdedWeatherScenario.CLEAR;

    private const int MIN_TRANSITION_TIME = 300;
    private const int MAX_TRANSITION_TIME = 600;
    private const int MIN_LOCKED_TIME     = 400;
    private const int MAX_LOCKED_TIME     = 900;

    private int GetRandomTransitionTime() { return Math.RandomIntInclusive(MIN_TRANSITION_TIME, MAX_TRANSITION_TIME); }
    private int GetRandomLockedTime()     { return Math.RandomIntInclusive(MIN_LOCKED_TIME,   MAX_LOCKED_TIME);     }

    override void Init()
    {
        super.Init();

        m_Sunrise_Jan = 7;  m_Sunset_Jan = 16;
        m_Sunrise_Jul = 5;  m_Sunset_Jul = 20;

        int i;
		m_MinTemps = {
             1,  2,  4,  9, 15, 19, 28, 27, 26, 13,  9,  4
        };
        m_MaxTemps = {
             6,  8, 10, 17, 24, 27, 33, 30, 29, 18, 12,  8
        };
        if (CfgGameplayHandler.GetEnvironmentMinTemps() && CfgGameplayHandler.GetEnvironmentMinTemps().Count() == 12)
            for (i=0;i<12;i++) m_MinTemps[i] = CfgGameplayHandler.GetEnvironmentMinTemps().Get(i);
        if (CfgGameplayHandler.GetEnvironmentMaxTemps() && CfgGameplayHandler.GetEnvironmentMaxTemps().Count() == 12)
            for (i=0;i<12;i++) m_MaxTemps[i] = CfgGameplayHandler.GetEnvironmentMaxTemps().Get(i);

        if (GetGame().IsServer() || !GetGame().IsMultiplayer())
        {
            m_Weather.SetDynVolFogHeightDensity(1, 60);
            m_Weather.SetDynVolFogHeightBias(0, 0);
            m_Weather.GetFog().Set(0, 0, 1000);
            m_Weather.GetFog().SetLimits(0, 0);
            m_Weather.GetOvercast().SetLimits(0.07, 1);

            if (GetGame().IsMultiplayer())
                m_Weather.GetOvercast().Set(Math.RandomFloat(0, 0.75), 0, 5);
        }

        m_WorldWindCoef                       = 0.55;
        m_CloudsTemperatureEffectModifier     = 2.0;
        m_TemperaturePerHeightReductionModifier = 0.03;
        m_TemperatureInsideBuildingsModifier  = -5.0;
        m_WaterContactTemperatureModifier     = 10.0;
        
        m_CurrentScenario = ChooseModdedWeatherScenario();
    }

    EModdedWeatherScenario ChooseModdedWeatherScenario()
    {
        float weightClear  = 0;
        float weightFoggy  = 0;
        float weightLight  = 0;
        float weightMedium = 0;
        float weightHeavy  = 0;

        switch (m_CurrentScenario)
        {
            case EModdedWeatherScenario.CLEAR:
                weightClear  = 60;
                weightFoggy  = 10;
                weightLight  = 15;
                weightMedium = 10;
                weightHeavy  = 5;
                break;

            case EModdedWeatherScenario.FOGGY:
                weightClear  = 80;
                weightFoggy  = 10;
                weightLight  = 5;
                weightMedium = 3;
                weightHeavy  = 2;
                break;

            case EModdedWeatherScenario.LIGHT_RAIN:
                weightClear  = 40;
                weightFoggy  = 5;
                weightLight  = 20;
                weightMedium = 25;
                weightHeavy  = 10;
                break;

            case EModdedWeatherScenario.MEDIUM_RAIN:
                weightClear  = 45;
                weightFoggy  = 5;
                weightLight  = 25;
                weightMedium = 15;
                weightHeavy  = 10;
                break;

            case EModdedWeatherScenario.HEAVY_RAIN:
                weightClear  = 30;
                weightFoggy  = 5;
                weightLight  = 20;
                weightMedium = 35;
                weightHeavy  = 10;
                break;
        }

        float total = weightClear + weightFoggy + weightLight + weightMedium + weightHeavy;
        float roll  = Math.RandomFloatInclusive(0, total);

        if (roll < weightClear)  return EModdedWeatherScenario.CLEAR;
        roll -= weightClear;
        if (roll < weightFoggy)  return EModdedWeatherScenario.FOGGY;
        roll -= weightFoggy;

        if (roll < weightLight)  return EModdedWeatherScenario.LIGHT_RAIN;
        roll -= weightLight;
        if (roll < weightMedium) return EModdedWeatherScenario.MEDIUM_RAIN;
        return EModdedWeatherScenario.HEAVY_RAIN;
    }

    float RandomizeValue(float value)
    {
        float min = Math.Max(value * 0.9, 0.0);
        float max = Math.Min(value * 1.1, 1.0);
        return Math.RandomFloatInclusive(min, max);
    }

    bool IsRainScenario(EModdedWeatherScenario s)
    { 
        return (s == EModdedWeatherScenario.LIGHT_RAIN || s == EModdedWeatherScenario.MEDIUM_RAIN || s == EModdedWeatherScenario.HEAVY_RAIN); 
    }

    override bool WeatherOnBeforeChange(EWeatherPhenomenon type, float actual, float change, float time)
    {
        if (type != EWeatherPhenomenon.OVERCAST) return false;

        m_CurrentScenario = ChooseModdedWeatherScenario();

        float targetOvercast;
        float targetRainfall = 0.0;
        float targetVolFog   = 0.0;

        switch (m_CurrentScenario)
        {
            case EModdedWeatherScenario.CLEAR:
                targetOvercast = RandomizeValue(SCENARIO_OVERCAST_CLEAR);
                targetVolFog   = SCENARIO_VOLFOG_CLEAR;
                break;
            case EModdedWeatherScenario.FOGGY:
                targetOvercast = RandomizeValue(SCENARIO_OVERCAST_FOGGY);
                targetVolFog   = RandomizeValue(SCENARIO_VOLFOG_FOGGY);
                break;
            case EModdedWeatherScenario.LIGHT_RAIN:
                targetOvercast = RandomizeValue(SCENARIO_OVERCAST_LIGHT_RAIN);
                targetRainfall = RandomizeValue(SCENARIO_RAINFALL_LIGHT_RAIN);
                targetVolFog   = RandomizeValue(SCENARIO_VOLFOG_LIGHT_RAIN);
                break;
            case EModdedWeatherScenario.MEDIUM_RAIN:
                targetOvercast = RandomizeValue(SCENARIO_OVERCAST_MEDIUM_RAIN);
                targetRainfall = RandomizeValue(SCENARIO_RAINFALL_MEDIUM_RAIN);
                targetVolFog   = RandomizeValue(SCENARIO_VOLFOG_MEDIUM_RAIN);
                break;
            case EModdedWeatherScenario.HEAVY_RAIN:
                targetOvercast = SCENARIO_OVERCAST_HEAVY_RAIN;
                targetRainfall = SCENARIO_RAINFALL_HEAVY_RAIN;
                targetVolFog   = RandomizeValue(SCENARIO_VOLFOG_HEAVY_RAIN);
                break;
        }

        int transitionTime = GetRandomTransitionTime();
        int lockedTime     = GetRandomLockedTime();

        m_Weather.GetOvercast().Set(targetOvercast, transitionTime, lockedTime);
        m_Weather.SetDynVolFogDistanceDensity(targetVolFog, transitionTime);
        m_Weather.SetDynVolFogHeightBias(0, 0);
        m_Weather.SetDynVolFogHeightDensity(1, 0);
        m_Weather.GetSnowfall().Set(0.0, 0, 0);

        if (IsRainScenario(m_CurrentScenario))
        {
            m_Weather.GetRain().Set(targetRainfall, transitionTime, transitionTime);
            m_Weather.GetRain().SetLimits(0.0, 1.0);
        }
        else
        {
            m_Weather.GetRain().Set(0.0, 0, 0);
            m_Weather.GetRain().SetLimits(0.0, 0.0);
        }

        float windMag, windDir;
        int   phmnTime   = transitionTime / 5;
        int   phmnLength = lockedTime     / 5;

        CalculateWind(-1, false, windMag, windDir);
        m_Weather.GetWindMagnitude().Set(windMag, phmnTime, phmnLength);
        m_Weather.GetWindDirection().Set(windDir, phmnTime, phmnLength + 1000);

        return true;
    }

    override void CalculateWind(int newWeather, bool suddenChange, out float magnitude, out float direction)
    {
        magnitude = 1.0;
        direction = 0.0;

        float windChance = Math.RandomIntInclusive(0, 100);

        switch (m_CurrentScenario)
        {
            case EModdedWeatherScenario.CLEAR:
                if (windChance < 65)      { magnitude = Math.RandomFloatInclusive(0, 3);  direction = Math.RandomFloatInclusive(-1.0, -0.5); }
                else if (windChance < 90) { magnitude = Math.RandomFloatInclusive(3, 8);  direction = Math.RandomFloatInclusive(-1.3, -0.9); }
                else                      { magnitude = Math.RandomFloatInclusive(8, 20); direction = Math.RandomFloatInclusive(-0.6,  0.0); }
                break;
            case EModdedWeatherScenario.FOGGY:
                if (windChance < 50)      { magnitude = Math.RandomFloatInclusive(0, 1);  direction = Math.RandomFloatInclusive(-3.14, -2.4); }
                else                      { magnitude = Math.RandomFloatInclusive(0, 2);  direction = Math.RandomFloatInclusive(-2.2, -1.4); }
                break;
            case EModdedWeatherScenario.LIGHT_RAIN: case EModdedWeatherScenario.MEDIUM_RAIN:
                if (windChance < 70)      { magnitude = Math.RandomFloatInclusive(0, 4);   direction = Math.RandomFloatInclusive(0.5, 1.0); }
                else if (windChance < 95) { magnitude = Math.RandomFloatInclusive(4, 10);  direction = Math.RandomFloatInclusive(1.0, 1.4); }
                else                      { magnitude = Math.RandomFloatInclusive(10, 20); direction = Math.RandomFloatInclusive(1.4, 1.7); }
                break;
            case EModdedWeatherScenario.HEAVY_RAIN:
                magnitude = Math.RandomFloatInclusive(10, 20);
                direction = Math.RandomFloatInclusive(1.2, 2.2);
                break;
        }
    }

    override protected void CalculateVolFog(float lerpValue, float windMagnitude, float changeTime)
    {

    }
}
