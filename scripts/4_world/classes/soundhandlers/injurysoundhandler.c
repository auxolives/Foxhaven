//================================================================
//  INJURY SOUND HANDLER (CLIENT)
//================================================================
//  • DESCRIPTION: This client-side script overrides the sound
//    processing for injuries. It checks a server-synced boolean
//    to determine if injury sounds (like heavy breathing) should
//    be muted. This is used to prevent auditory feedback when a
//    healthy player is encumbered only by movement penalties, such
//    as terrain slope or cargo weight.
//
//  • MOD COMPATIBILITY: This mod overrides the 'ProcessSound'
//    method in 'InjurySoundHandlerClient' and will conflict with
//    any other mod that also modifies this method. A 'super' call
//    is used to fall back to vanilla behavior when sounds are not
//    muted.
//================================================================
modded class InjurySoundHandlerClient
{
    override void ProcessSound(eInjurySoundZones zone)
    {
        if (m_Player && m_Player.m_IsSlopePenaltyActive) return;
        super.ProcessSound(zone);
    }
}
