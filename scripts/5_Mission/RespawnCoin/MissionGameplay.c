modded class MissionGameplay
{
    protected PlayerBase m_RC_LastPlayer;
    protected bool m_RC_RequestedForCurrentLife = false;
    protected ref RC_InvulnerabilityHud m_RC_InvulnerabilityHud;

    void MissionGameplay()
    {
        if (!g_RC_RpcClient)
        {
            g_RC_RpcClient = new RC_RpcClient();
        }

        m_RC_InvulnerabilityHud = new RC_InvulnerabilityHud();
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        if (player != m_RC_LastPlayer)
        {
            m_RC_LastPlayer = player;
            m_RC_RequestedForCurrentLife = false;
        }

        if (!m_RC_RequestedForCurrentLife && player && player.IsAlive())
        {
            m_RC_RequestedForCurrentLife = true;

            GetRPCManager().SendRPC(RC_MOD_NAME,RC_RPC_REQUEST_STATE,null,true,null,player);
        }

        if (m_RC_InvulnerabilityHud)
        {
            if (!player || !player.IsAlive())
                m_RC_InvulnerabilityHud.Hide();
            else
                m_RC_InvulnerabilityHud.Update(timeslice);
        }
    }

    void RC_StartInvulnerabilityCountdown(int durationSeconds)
    {
        if (!m_RC_InvulnerabilityHud)
            m_RC_InvulnerabilityHud = new RC_InvulnerabilityHud();

        m_RC_InvulnerabilityHud.StartCountdown(durationSeconds);
    }
}
