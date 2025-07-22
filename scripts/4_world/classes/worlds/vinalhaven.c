class VinalhavenData extends WorldData
{
	protected float m_DebugFastWeather = 0;
	protected bool m_IsFogEventActive = false;

	override void Init()
	{
		super.Init();

		m_Sunrise_Jan = 7;
		m_Sunset_Jan = 16;
		m_Sunrise_Jul = 5;
		m_Sunset_Jul = 20;

		m_MinTemps = { 5, 6, 8, 11, 15, 19, 22, 21, 18, 14, 10, 7 };
		m_MaxTemps = { 12, 14, 16, 19, 23, 27, 30, 29, 25, 20, 16, 13 };

		int tempIdx;
		if (CfgGameplayHandler.GetEnvironmentMinTemps() && CfgGameplayHandler.GetEnvironmentMinTemps().Count() == 12)
		{
			for (tempIdx = 0; tempIdx < CfgGameplayHandler.GetEnvironmentMinTemps().Count(); tempIdx++)
			{
				m_MinTemps[tempIdx] = CfgGameplayHandler.GetEnvironmentMinTemps().Get(tempIdx);
			}
		}
		if (CfgGameplayHandler.GetEnvironmentMaxTemps() && CfgGameplayHandler.GetEnvironmentMaxTemps().Count() == 12)
		{
			for (tempIdx = 0; tempIdx < CfgGameplayHandler.GetEnvironmentMaxTemps().Count(); tempIdx++)
			{
				m_MaxTemps[tempIdx] = CfgGameplayHandler.GetEnvironmentMaxTemps().Get(tempIdx);
			}
		}

		m_WorldWindCoef = 0.4;
		m_CloudsTemperatureEffectModifier = 2.0;
		m_TemperaturePerHeightReductionModifier = 0.012;
		m_UniversalTemperatureSourceCapModifier = -1.0;		
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{			
			m_Weather.GetFog().SetLimits(0, 0);
			m_Weather.GetFog().Set(0, 0, 0);
			
			m_Weather.SetDynVolFogHeightBias(m_WeatherDefaultSettings.m_DefaultHeigthBias);
		
			if (GetGame().IsMultiplayer())
			{
				float startingOvercast = Math.RandomFloat(0.1, 0.5);
				m_Weather.GetOvercast().Set(startingOvercast, 0, 5);
			}
		}
	}	
	
	override void SetupWeatherSettings()
	{
		super.SetupWeatherSettings();
		
		m_WeatherDefaultSettings.m_ClearWeatherChance = 70;
		m_WeatherDefaultSettings.m_BadWeatherChance = 90; 
		
		m_WeatherDefaultSettings.m_StormThreshold		 	= 0.9;
		m_WeatherDefaultSettings.m_GlobalSuddenChance	 	= 0;
		m_WeatherDefaultSettings.m_BadWeatherSuddenChance 	= 0;

		m_WeatherDefaultSettings.m_DefaultHeigthBias 		= 0;
	}
	
	override bool WeatherOnBeforeChange(EWeatherPhenomenon type, float actual, float change, float time)
	{		
		float timeMultiplier = 1.0;
        if (m_DebugFastWeather == 1)
        {
            timeMultiplier = 0.05;
        }

		float phmnTime 	= 5;
		float phmnLength = 10;
		float phmnValue = 0;

		m_Weather.SetStorm(1.0, m_WeatherDefaultSettings.m_StormThreshold, 45);
		m_Weather.SetRainThresholds(m_WeatherDefaultSettings.m_RainThreshold, 1.0, 60);
		m_Weather.SetWindMaximumSpeed(20);
		
		if (m_Weather.GetDynVolFogHeightBias() != 0)
		{
			m_Weather.SetDynVolFogHeightBias(0);
		}

		switch (type)
		{
			case EWeatherPhenomenon.OVERCAST:			
			{
				float windDirection, windMag;

				phmnValue = Math.RandomFloatInclusive(0.2, 0.7);
				phmnTime = Math.RandomIntInclusive(m_WeatherDefaultSettings.m_OvercastMinTime, m_WeatherDefaultSettings.m_OvercastMaxTime) * timeMultiplier;
				phmnLength = Math.RandomIntInclusive(m_WeatherDefaultSettings.m_OvercastMinLength, m_WeatherDefaultSettings.m_OvercastMaxLength) * timeMultiplier;				
			
				m_Chance = Math.RandomIntInclusive(0, 100);
				
				if (m_LastWeather == WorldDataWeatherConstants.CLEAR_WEATHER)
				{
					m_ClearWeatherChance -= (m_StepValue * m_SameWeatherCnt);
				}
				else if (m_LastWeather == WorldDataWeatherConstants.CLOUDY_WEATHER)
				{
					m_ClearWeatherChance += (m_StepValue * m_SameWeatherCnt);
				}
				else if (m_LastWeather == WorldDataWeatherConstants.BAD_WEATHER)
				{
					m_ClearWeatherChance += m_StepValue;
					m_BadWeatherChance += ((m_StepValue * m_SameWeatherCnt) + m_StepValue);
				}
			
				if (m_Chance < m_ClearWeatherChance)
				{
					m_ChoosenWeather = WorldDataWeatherConstants.CLEAR_WEATHER;
					if (m_LastWeather == WorldDataWeatherConstants.CLEAR_WEATHER)
						m_SameWeatherCnt++;
				}
				else if (m_Chance > m_BadWeatherChance)
				{
					m_ChoosenWeather = WorldDataWeatherConstants.BAD_WEATHER;
					if (m_LastWeather == WorldDataWeatherConstants.BAD_WEATHER)
						m_SameWeatherCnt++;
				}
				else
				{
					m_ChoosenWeather = WorldDataWeatherConstants.CLOUDY_WEATHER;
					if (m_LastWeather == WorldDataWeatherConstants.CLOUDY_WEATHER)
						m_SameWeatherCnt++;
				}

				if (m_ChoosenWeather != m_LastWeather)
					m_SameWeatherCnt = 0;

				m_ClearWeatherChance = m_WeatherDefaultSettings.m_ClearWeatherChance;
				m_BadWeatherChance = m_WeatherDefaultSettings.m_BadWeatherChance;
		
				if (m_ChoosenWeather == WorldDataWeatherConstants.CLEAR_WEATHER)
				{
					m_LastWeather = WorldDataWeatherConstants.CLEAR_WEATHER;
					phmnValue = Math.RandomFloatInclusive(0.0, 0.25);
				}
				else if (m_ChoosenWeather == WorldDataWeatherConstants.CLOUDY_WEATHER)
				{
					m_LastWeather = WorldDataWeatherConstants.CLOUDY_WEATHER;
					phmnValue = Math.RandomFloatInclusive(0.25, 0.6);
				}
				else if (m_ChoosenWeather == WorldDataWeatherConstants.BAD_WEATHER)
				{
					m_LastWeather = WorldDataWeatherConstants.BAD_WEATHER;
					phmnValue = Math.RandomFloatInclusive(0.6, 1.0);
				}

				m_Weather.GetOvercast().Set(phmnValue, phmnTime, phmnLength);
				
				if (phmnValue < m_WeatherDefaultSettings.m_RainThreshold)
				{
					float rainStopTime = phmnTime / 3;
					if (rainStopTime < 15) rainStopTime = 15;
					m_Weather.GetRain().Set(0, rainStopTime, 0);
				}
				
				CalculateWind(m_ChoosenWeather, false, windMag, windDirection);
				m_Weather.GetWindMagnitude().Set(windMag, phmnTime * WIND_MAGNITUDE_TIME_MULTIPLIER, phmnTime * (1 - WIND_MAGNITUDE_TIME_MULTIPLIER));
				m_Weather.GetWindDirection().Set(windDirection, phmnTime * WIND_DIRECTION_TIME_MULTIPLIER, phmnTime - (phmnTime * WIND_DIRECTION_TIME_MULTIPLIER) + phmnLength + 1000);
				
				if (m_IsFogEventActive)
				{
					m_Weather.SetDynVolFogDistanceDensity(0, phmnTime / 2);
					m_IsFogEventActive = false;
				}
				else if (phmnValue < m_WeatherDefaultSettings.m_RainThreshold)
				{
					int fogChance = Math.RandomIntInclusive(0, 100);
					if (fogChance < 10)
					{
						float exponent = 4.0; 
						float minFog = 0.05;
						float maxFog = 1.0;

						float randomBase = Math.RandomFloat01(); 
						float biasedRandom = Math.Pow(randomBase, exponent);
						float fogValue = minFog + biasedRandom * (maxFog - minFog);

						m_Weather.SetDynVolFogDistanceDensity(fogValue, phmnTime);
						m_IsFogEventActive = true;
					}
				}
				return true;
			}
			
			case EWeatherPhenomenon.RAIN:
			{
				float actualOvercast = m_Weather.GetOvercast().GetActual();
				
				m_Chance = Math.RandomIntInclusive(0, 100);
				phmnValue = 0.2;
				phmnTime = 90 * timeMultiplier;
				phmnLength = 0;
			
				if (actualOvercast <= m_WeatherDefaultSettings.m_RainThreshold)
				{
					phmnValue = 0;
				}
				else if (actualOvercast > m_WeatherDefaultSettings.m_StormThreshold)
				{
					phmnValue = Math.RandomFloatInclusive(0.8, 1.0);
				}
				else if (actualOvercast < 0.75)
				{
					if (m_Chance < 30) { phmnValue = Math.RandomFloatInclusive(0.1, 0.3); }
					else if (m_Chance < 60) { phmnValue = Math.RandomFloatInclusive(0.2, 0.5); }
					else if (m_Chance < 80) { phmnValue = Math.RandomFloatInclusive(0.0, 0.2); }
					else { phmnValue = 0; }
				}
				else
				{
					if (m_Chance < 25) { phmnValue = Math.RandomFloatInclusive(0.5, 0.7); }
					else if (m_Chance < 50) { phmnValue = Math.RandomFloatInclusive(0.2, 0.4); }
					else if (m_Chance < 75) { phmnValue = Math.RandomFloatInclusive(0.4, 0.6); }
					else { phmnValue = 0; }
				}
				
				if (phmnValue == 0)
				{
					phmnLength = m_WeatherDefaultSettings.m_RainLengthMin * timeMultiplier;
				}
		
				m_Weather.GetRain().Set(phmnValue, phmnTime, phmnLength);
				
				float fogDensity = Math.Pow(phmnValue, 3) * 0.6;
				m_Weather.SetDynVolFogDistanceDensity(fogDensity, phmnTime);

				return true;
			}
	
			case EWeatherPhenomenon.FOG:
			{
				m_Weather.GetFog().Set(0, 0, 0);
				return true;
			}
	
			case EWeatherPhenomenon.WIND_MAGNITUDE:
			{
				phmnTime = Math.RandomInt(m_WeatherDefaultSettings.m_RainTimeMin, m_WeatherDefaultSettings.m_RainTimeMax) * timeMultiplier;
				m_Weather.GetWindMagnitude().Set(m_Weather.GetWindMagnitude().GetActual() * 0.75, phmnTime, m_WeatherDefaultSettings.m_OvercastMaxLength * timeMultiplier);
				return true;
			}
		}

		return false;
	}
	
	protected override void CalculateWind(int newWeather, bool suddenChange, out float magnitude, out float direction)
	{
		magnitude = 5;
		direction = 0;		
		
		float windChance = Math.RandomIntInclusive(0, 100);
		
		if (newWeather == WorldDataWeatherConstants.CLEAR_WEATHER)
		{
			if (windChance < 30) { magnitude = Math.RandomFloatInclusive(6, 10); direction = Math.RandomFloatInclusive(-1.0, -0.5); }
			else if (windChance < 75) { magnitude = Math.RandomFloatInclusive(8, 12); direction = Math.RandomFloatInclusive(-1.3, -0.9); }
			else { magnitude = Math.RandomFloatInclusive(4, 6); direction = Math.RandomFloatInclusive(-0.6, 0.0); }
		}		
		else if (newWeather == WorldDataWeatherConstants.CLOUDY_WEATHER)
		{			
			if (windChance < 45) { magnitude = Math.RandomFloatInclusive(6, 10); direction = Math.RandomFloatInclusive(-3.14, -2.4); }
			else if (windChance < 90) { magnitude = Math.RandomFloatInclusive(8, 12); direction = Math.RandomFloatInclusive(-2.6, -2.0); }
			else { magnitude = Math.RandomFloatInclusive(10, 14); direction = Math.RandomFloatInclusive(-2.2, -1.4); }
		}		
		else
		{
			if (suddenChange || m_Weather.GetOvercast().GetActual() > m_WeatherDefaultSettings.m_StormThreshold || m_Weather.GetOvercast().GetForecast() - m_Weather.GetOvercast().GetActual() >= 0.4)
			{
				magnitude = Math.RandomFloatInclusive(14, 17);
				direction = Math.RandomFloatInclusive(0.9, 1.45);
			}
			else if (windChance < 45) { magnitude = Math.RandomFloatInclusive(9, 12); direction = Math.RandomFloatInclusive(1.45, 1.7); }
			else if (windChance < 90) { magnitude = Math.RandomFloatInclusive(7, 10); direction = Math.RandomFloatInclusive(1.6, 2.0); }
			else { magnitude = Math.RandomFloatInclusive(4, 8); direction = Math.RandomFloatInclusive(1.9, 2.2); }			
		}
	}
	
	protected override void CalculateVolFog(float lerpValue, float windMagnitude, float changeTime)
	{

	}
}
