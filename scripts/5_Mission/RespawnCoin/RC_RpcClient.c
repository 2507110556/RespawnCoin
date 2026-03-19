ref RC_RpcClient g_RC_RpcClient;

class RC_RpcClient
{
    void RC_RpcClient()
    {
        GetRPCManager().AddRPC(RC_MOD_NAME, RC_RPC_SHOW_DIALOG, this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC(RC_MOD_NAME, RC_RPC_SHOW_VOUCHER_STATUS, this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC(RC_MOD_NAME, RC_RPC_START_INVULNERABILITY, this, SingleplayerExecutionType.Both);
        Print("[RespawnCoin] RC_RpcClient RPC registered");
    }

    void RPC_ShowDialog(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param5<int, bool, vector, string, string> data;
        if (!ctx.Read(data))
            return;

        RC_DialogManager.ShowDialog(data.param1, data.param2, data.param3, data.param4, data.param5);
    }

    void RPC_ShowVoucherStatus(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<int> data;
        if (!ctx.Read(data))
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        player.MessageStatus(RC_Localization.FormatVoucherGainMessage(data.param1));
    }

    void RPC_StartInvulnerability(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client)
            return;

        Param1<int> data;
        if (!ctx.Read(data))
            return;

        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (!mission)
            return;

        mission.RC_StartInvulnerabilityCountdown(data.param1);
    }
}
