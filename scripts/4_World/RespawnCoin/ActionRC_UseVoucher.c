class ActionRC_UseVoucher : ActionSingleUseBase
{
    void ActionRC_UseVoucher()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
        m_FullBody = false;
        m_Text = "#STR_RC_ACTION_USE_VOUCHER";
    }

    override typename GetInputType()
    {
        return DefaultActionInput;
    }

    override bool HasTarget()
    {
        return false;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !item)
            return false;

        if (item != player.GetItemInHands())
            return false;

        if (!item.RC_IsCoinVoucher())
            return false;

        return true;
    }

    override void OnExecuteServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_MainItem)
            return;

        PlayerBase player = action_data.m_Player;
        ItemBase item = action_data.m_MainItem;
        PlayerIdentity identity = player.GetIdentity();

        if (!identity)
            return;

        if (!item.RC_IsCoinVoucher())
            return;

        int addCoins = item.RC_GetCoinAmount();
        if (addCoins <= 0)
            return;

        RC_Manager.AddCoins(identity, addCoins);

        Param1<int> payload = new Param1<int>(addCoins);
        GetRPCManager().SendRPC(RC_MOD_NAME, RC_RPC_SHOW_VOUCHER_STATUS, payload, true, identity, player);

        GetGame().ObjectDelete(item);
    }
}