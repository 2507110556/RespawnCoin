ref RC_RpcServer g_RC_RpcServer;

class RC_RpcServer
{
    void RC_RpcServer()
    {
        GetRPCManager().AddRPC(RC_MOD_NAME, RC_RPC_REQUEST_STATE, this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC(RC_MOD_NAME, RC_RPC_CONFIRM_USE, this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC(RC_MOD_NAME, RC_RPC_CANCEL_USE, this, SingleplayerExecutionType.Both);

        Print("[RespawnCoin] RC_RpcServer RPC registered");
    }

    void RPC_RequestState(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server)
            return;

        if (!sender)
            return;

        RC_Manager.EnsurePlayerFile(sender);
        RC_Manager.EnsureZoneConfig();
        RC_Manager.SendDialogStateTo(sender);
    }

    void RPC_ConfirmUse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server)
            return;

        if (!sender)
            return;

        PlayerBase player = RC_Manager.FindPlayerByIdentity(sender);
        if (!player)
            return;

        string failReason = "";
        if (!RC_Manager.TryUseCoinAndRespawn(player, failReason))
        {
            RC_Manager.SendDialogStateTo(sender);
        }
    }

    void RPC_CancelUse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Server)
            return;

        if (!sender)
            return;

        RC_Manager.ClearPendingRespawn(sender);
    }
}