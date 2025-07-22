modded class Environment
{
    override protected void ProcessHeatBuffer(EnvironmentSnapshotData data)
	{
		if (FoxhavenConfig.GetInstance().IsHeatBufferEnabled())
		{
			super.ProcessHeatBuffer(data);
		}
		else
		{
			m_Player.GetStatHeatBuffer().Set(0.0);
			m_Player.SetHeatBufferDynamicMax(0.0);
		
			if (m_Player.GetHeatBufferStage() != 0)
			{
				m_Player.ToggleHeatBufferVisibility(0);
			}
		}
	}
}