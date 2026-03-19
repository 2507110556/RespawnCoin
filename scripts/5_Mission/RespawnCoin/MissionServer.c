modded class MissionServer
{
    void MissionServer()
    {
        if (!g_RC_RpcServer)
        {
            g_RC_RpcServer = new RC_RpcServer();
        }

        RC_Manager.EnsureZoneConfig();
    }
}