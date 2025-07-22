modded class PlayerBase
{
    float m_SlopeAngle;
    float m_SlopeCheckTimer;
    bool  m_IsSlopePenaltyActive;

    const float SLOPE_SAMPLE_DISTANCE = 1.25;
    const float SLOPE_MIN_DIFF        = 0.10;

    void PlayerBase()
    {
        RegisterNetSyncVariableBool("m_IsSlopePenaltyActive");
    }

    override void OnCommandHandlerTick(float delta_time, int pCurrentCommandID)
    {
        super.OnCommandHandlerTick(delta_time, pCurrentCommandID);
        #ifdef SERVER
            SlopePenaltyHandler(delta_time);
        #endif
    }

    void SlopePenaltyHandler(float delta_time)
    {
        m_SlopeCheckTimer -= delta_time;
        if (m_SlopeCheckTimer > 0) return;
        m_SlopeCheckTimer = 0.25;

        vector pos = GetPosition();
        float ground = GetGame().SurfaceY(pos[0], pos[2]);

        vector dir = GetDirection();
        dir[1] = 0;
        float len = dir.Normalize();
        if (len < 0.001)
        {
            m_SlopeAngle = 0;
            return;
        }

        vector ahead = pos + dir * SLOPE_SAMPLE_DISTANCE;
        float aheadY = GetGame().SurfaceY(ahead[0], ahead[2]);
        float dh = aheadY - ground;

        if (Math.AbsFloat(dh) < SLOPE_MIN_DIFF)
            m_SlopeAngle = 0;
        else
            m_SlopeAngle = Math.Atan2(dh, SLOPE_SAMPLE_DISTANCE) * Math.RAD2DEG;

        if (m_InjuryHandler)
            m_InjuryHandler.m_TimeSinceLastTick = m_InjuryHandler.VALUE_CHECK_INTERVAL + 1;
    }
}