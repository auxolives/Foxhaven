modded class InfluenzaAgent : AgentBase
{
	override bool AutoinfectCheck(float deltaT, PlayerBase player)
	{
		if (FoxhavenConfig.GetInstance() && !FoxhavenConfig.GetInstance().IsHeatBufferEnabled())
		{
			if (!player.m_AgentPool.GetTemporaryResistance(eAgents.INFLUENZA))
			{
				float heatcomfort = player.GetStatHeatComfort().Get();
				
				if (heatcomfort < PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_CRITICAL)
				{
					float subtractTimeModifier = 0;
					float subtractTimeRandomized = Math.RandomFloatInclusive(CONTRACT_TIME_THRESHOLD_MIN, CONTRACT_TIME_THRESHOLD_MAX);
					
					if (heatcomfort <= PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_CRITICAL && heatcomfort > PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_EMPTY)
						subtractTimeModifier = CONTRACT_HEATCOMFORT_MODIFIER[1];
					else if (heatcomfort <= PlayerConstants.THRESHOLD_HEAT_COMFORT_MINUS_EMPTY)
						subtractTimeModifier = CONTRACT_HEATCOMFORT_MODIFIER[2];
					
					float subtractTimeModified = subtractTimeRandomized * subtractTimeModifier;
	
					m_TimeToContract -= subtractTimeModified;
					
					if (m_ElapsedTime >= m_TimeToContract)
					{		
						m_ElapsedTime = 0;
						m_TimeToContract = MAX_TIME_TO_CONTRACT;
						return true;
					}
		
					m_ElapsedTime += deltaT;
				}
				else
				{
					if (m_ElapsedTime > 0)
					{
						player.SetTemporaryResistanceToAgent(eAgents.INFLUENZA, 300);
					}
	
					m_ElapsedTime = Math.Clamp(m_ElapsedTime - deltaT, 0.0, float.MAX);
					m_TimeToContract = Math.Clamp(m_TimeToContract + deltaT*15, 0.0, MAX_TIME_TO_CONTRACT);
				}
			}
			
			return false;
		}
		
		return super.AutoinfectCheck(deltaT, player);
	}
}
