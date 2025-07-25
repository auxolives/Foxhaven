//================================================================
//  CONFIGURATION LOADER
//================================================================
//  • DESCRIPTION: This script hooks into the server's initialization
//    process to load the Foxhaven mod's configuration file
//    ('Config.json').
//
//  • MOD COMPATIBILITY: This mod overrides the 'OnInit' method in
//    'MissionServer'. It is designed for high compatibility by
//    using a 'super' call to ensure other mods' initialization
//    routines are also executed. Direct conflicts are unlikely
//    unless another mod overrides the same method without a
//    'super' call.
//================================================================
modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        Print("[Foxhaven] Loading configuration...");
        FoxhavenConfig.GetInstance().Load();
    }
}
