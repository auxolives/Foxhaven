modded class InjurySoundHandlerClient
{
    override void ProcessSound(eInjurySoundZones zone)
    {
        if (m_Player && m_Player.m_IsSlopePenaltyActive) return;
        super.ProcessSound(zone);
    }
}
