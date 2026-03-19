modded class PlayerBase
{
    protected ref Timer m_RC_RespawnInvulnerabilityTimer;

    override void EEKilled(Object killer)
    {
        RC_EndRespawnInvulnerability();
        super.EEKilled(killer);

        if (GetGame().IsServer())
        {
            RC_Manager.MarkDeath(this);
        }
    }

    void RC_StartRespawnInvulnerability(float durationSeconds)
    {
        RC_EndRespawnInvulnerability();

        if (durationSeconds <= 0)
            return;

        SetAllowDamage(false);

        m_RC_RespawnInvulnerabilityTimer = new Timer(CALL_CATEGORY_SYSTEM);
        m_RC_RespawnInvulnerabilityTimer.Run(durationSeconds, this, "RC_EndRespawnInvulnerability", null, false);

        Print("[RespawnCoin] Invulnerability started for " + durationSeconds.ToString() + "s on player=" + this);
    }

    void RC_EndRespawnInvulnerability()
    {
        SetAllowDamage(true);

        if (m_RC_RespawnInvulnerabilityTimer)
        {
            m_RC_RespawnInvulnerabilityTimer.Stop();
            m_RC_RespawnInvulnerabilityTimer = null;
        }
    }
}
