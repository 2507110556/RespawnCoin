class CfgPatches
{
    class RespawnCoin
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Scripts",
            "JM_CF_Scripts"
        };
    };
};

class CfgMods
{
    class RespawnCoin
    {
        dir = "RespawnCoin";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "RespawnCoin";
        credits = "";
        author = "单开双控";
        authorID = "0";
        version = "1.0";
        extra = 0;
        type = "mod";

        dependencies[] = { "Game", "World", "Mission" };

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = { "RespawnCoin/scripts/3_Game" };
            };
            class worldScriptModule
            {
                value = "";
                files[] = { "RespawnCoin/scripts/4_World" };
            };
            class missionScriptModule
            {
                value = "";
                files[] = { "RespawnCoin/scripts/5_Mission" };
            };
        };
    };
};

class CfgVehicles
{
    class Paper;
    class RC_CoinVoucher_Base: Paper
    {
        scope = 0;
        displayName = "$STR_RC_ITEM_VOUCHER_BASE_NAME";
        descriptionShort = "$STR_RC_ITEM_VOUCHER_BASE_DESC";
    };

    class RC_CoinVoucher_1: RC_CoinVoucher_Base
    {
        scope = 2;
        displayName = "$STR_RC_ITEM_VOUCHER_1_NAME";
        descriptionShort = "$STR_RC_ITEM_VOUCHER_1_DESC";
    };

    class RC_CoinVoucher_5: RC_CoinVoucher_Base
    {
        scope = 2;
        displayName = "$STR_RC_ITEM_VOUCHER_5_NAME";
        descriptionShort = "$STR_RC_ITEM_VOUCHER_5_DESC";
    };

    class RC_CoinVoucher_10: RC_CoinVoucher_Base
    {
        scope = 2;
        displayName = "$STR_RC_ITEM_VOUCHER_10_NAME";
        descriptionShort = "$STR_RC_ITEM_VOUCHER_10_DESC";
    };
};