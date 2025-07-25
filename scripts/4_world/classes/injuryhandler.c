modded class InjuryAnimationHandler
{
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
        MovementPenaltiesSettings movementSettings = FoxhavenConfig.GetInstance().GetMovementPenaltiesSettings();
        if (!movementSettings.playerEnergyWaterBloodSettings.isPenaltyEnabled)
            return 0;

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
        MovementPenaltiesSettings movementSettings = FoxhavenConfig.GetInstance().GetMovementPenaltiesSettings();
        if (!m_Player || !movementSettings.weightSettings.isPenaltyEnabled) 
            return 0;

        WeightSettings weightSettings = movementSettings.weightSettings;
        float l = m_Player.GetPlayerLoad();
        if (l > weightSettings.loadPenaltyLevel3_g) return 3;
        if (l > weightSettings.loadPenaltyLevel2_g) return 2;
        if (l > weightSettings.loadPenaltyLevel1_g) return 1;
        return 0;
    }

    private int CalculateSlopePenaltyStages()
    {
        MovementPenaltiesSettings movementSettings = FoxhavenConfig.GetInstance().GetMovementPenaltiesSettings();
        if (!m_Player || !movementSettings.terrainSlopeSettings.isPenaltyEnabled)
            return 0;
            
        TerrainSlopeSettings slopeSettings = movementSettings.terrainSlopeSettings;
        float a = m_Player.m_SlopeAngle;
        int p = 0;
        if (a > 0)
        {
            if (a > slopeSettings.slopeUpCritical_deg) p = 4;
            else if (a > slopeSettings.slopeUpStrong_deg) p = 3;
            else if (a > slopeSettings.slopeUpMedium_deg) p = 2;
            else if (a > slopeSettings.slopeUpWeak_deg) p = 1;
        }
        else if (a < 0)
        {
            float d = -a;
            if (d > slopeSettings.slopeDownMedium_deg) p = 2; 
            else if (d > slopeSettings.slopeDownWeak_deg) p = 1;
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
            
            if (slope > 0 || load > 0)
            {
                switch (statLvl)
                {
                    case eInjuryHandlerLevels.PRISTINE:
                        mute = true;
                        break;
                    
                    case eInjuryHandlerLevels.WORN:
                        if (Math.RandomFloatInclusive(0, 1) < 0.66)
                            mute = true;
                        break;

                    case eInjuryHandlerLevels.DAMAGED:
                        if (Math.RandomFloatInclusive(0, 1) < 0.33)
                            mute = true;
                        break;
                }
            }

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
