modded class ItemBase
{
    int RC_GetCoinAmount()
    {
        return 0;
    }

    bool RC_IsCoinVoucher()
    {
        return false;
    }
}

class RC_CoinVoucher_Base : Paper
{
    override bool RC_IsCoinVoucher()
    {
        return true;
    }

    override int RC_GetCoinAmount()
    {
        return 0;
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionRC_UseVoucher);
    }
}

class RC_CoinVoucher_1 : RC_CoinVoucher_Base
{
    override int RC_GetCoinAmount()
    {
        return 1;
    }
}

class RC_CoinVoucher_5 : RC_CoinVoucher_Base
{
    override int RC_GetCoinAmount()
    {
        return 5;
    }
}

class RC_CoinVoucher_10 : RC_CoinVoucher_Base
{
    override int RC_GetCoinAmount()
    {
        return 10;
    }
}