modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();

        Print("[Foxhaven] Loading configuration...");
        FoxhavenConfig.GetInstance().Load();
    }
}