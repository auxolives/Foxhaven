modded class InjuryAnimationHandler
{
    private const float SLOPE_UP_WEAK_THRESHOLD = 15.0;
    private const float SLOPE_UP_MEDIUM_THRESHOLD = 22.0;
    private const float SLOPE_UP_STRONG_THRESHOLD = 31.0;
    private const float SLOPE_UP_CRITICAL_THRESHOLD = 40.0;
	
    private const float SLOPE_DOWN_WEAK_THRESHOLD = 12.0;
    private const float SLOPE_DOWN_MEDIUM_THRESHOLD = 20.0;
	
    private const float LOAD_LVL1_THRESHOLD = 50000.0;
    private const float LOAD_LVL2_THRESHOLD = 60000.0;
    private const float LOAD_LVL3_THRESHOLD = 70000.0;

    private int ConvertMalusToStages(int m)
    {
        if (m >= 6) return 4;
        if (m >= 4) return 3;
        if (m >= 2) return 2;
        if (m >= 1) return 1;
        return 0;
    }

    private eInjuryHandlerLevels GetBaseHealthInjuryLevel()
    {
        float h = m_Player.GetHealth("", "Health");
        if (h <= PlayerConstants.SL_HEALTH_CRITICAL) return eInjuryHandlerLevels.RUINED;
        if (h <= PlayerConstants.SL_HEALTH_LOW) return eInjuryHandlerLevels.BADLY_DAMAGED;
        if (h <= PlayerConstants.SL_HEALTH_NORMAL) return eInjuryHandlerLevels.DAMAGED;
        if (h <= PlayerConstants.SL_HEALTH_HIGH) return eInjuryHandlerLevels.WORN;
        return eInjuryHandlerLevels.PRISTINE;
    }

    private int CalculateTotalMalus()
    {
        int m = 0;
        float b = m_Player.GetHealth("", "Blood");
        PlayerStat<float> e = m_Player.GetStatEnergy();
        PlayerStat<float> w = m_Player.GetStatWater();
        float en = 0;
        if (e) en = e.Get();
        float wa = 0;
        if (w) wa = w.Get();
        if (b <= PlayerConstants.SL_BLOOD_CRITICAL) m += 2; else if (b <= PlayerConstants.SL_BLOOD_LOW) m += 1;
        if (en <= PlayerConstants.SL_ENERGY_CRITICAL) m += 2; else if (en <= PlayerConstants.SL_ENERGY_LOW) m += 1;
        if (wa <= PlayerConstants.SL_WATER_CRITICAL) m += 2; else if (wa <= PlayerConstants.SL_WATER_LOW) m += 1;
        return m;
    }

    private int CalculateLoadPenaltyStages()
    {
        if (!m_Player) return 0;
        float l = m_Player.GetPlayerLoad();
        if (l > LOAD_LVL3_THRESHOLD) return 3;
        if (l > LOAD_LVL2_THRESHOLD) return 2;
        if (l > LOAD_LVL1_THRESHOLD) return 1;
        return 0;
    }

    private int CalculateSlopePenaltyStages()
    {
        if (!m_Player) return 0;
        float a = m_Player.m_SlopeAngle;
        int p = 0;
        if (a > 0)
        {
            if (a > SLOPE_UP_CRITICAL_THRESHOLD) p = 4;
            else if (a > SLOPE_UP_STRONG_THRESHOLD) p = 3;
            else if (a > SLOPE_UP_MEDIUM_THRESHOLD) p = 2;
            else if (a > SLOPE_UP_WEAK_THRESHOLD) p = 1;
        }
        else if (a < 0)
        {
            float d = -a;
            if (d > SLOPE_DOWN_MEDIUM_THRESHOLD) p = 2; else if (d > SLOPE_DOWN_WEAK_THRESHOLD) p = 1;
        }
        return p;
    }

    private eInjuryHandlerLevels GetStatBasedInjuryLevel()
    {
        eInjuryHandlerLevels b = GetBaseHealthInjuryLevel();
        int s = ConvertMalusToStages(CalculateTotalMalus());
        int l = b + s;
        if (l > eInjuryHandlerLevels.RUINED) l = eInjuryHandlerLevels.RUINED;
        return l;
    }

    override void CheckValue(bool forceUpdate = false)
    {
        eInjuryHandlerLevels statLvl = GetStatBasedInjuryLevel();
        int lvl = statLvl;
        #ifdef SERVER
            int slope = CalculateSlopePenaltyStages();
            int load = CalculateLoadPenaltyStages();
            bool mute = false;
            if (statLvl == eInjuryHandlerLevels.PRISTINE && load == 0 && slope > 0) mute = true;
            if (m_Player && m_Player.m_IsSlopePenaltyActive != mute)
            {
                m_Player.m_IsSlopePenaltyActive = mute;
                m_Player.SetSynchDirty();
            }
            lvl = lvl + slope + load;
            if (lvl > eInjuryHandlerLevels.RUINED) lvl = eInjuryHandlerLevels.RUINED;
        #endif
        eInjuryHandlerLevels finalLvl = lvl;
        if (m_ForceInjuryAnimMask) finalLvl = GetOverrideLevel(finalLvl);
        if (m_LastHealthUpdate != finalLvl || forceUpdate)
        {
            SendValue(finalLvl);
            m_ChangedStateInvoker.Invoke(finalLvl);
            m_LastHealthUpdate = finalLvl;
            Synchronize(finalLvl);
        }
    }
}