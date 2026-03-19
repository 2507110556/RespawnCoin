class RC_Manager
{
    protected static ref RC_ZoneConfig s_ZoneConfig;

    static string GetDirPath()
    {
        string dir = "$profile:RespawnCoin\\";
        if (!FileExist(dir))
        {
            MakeDirectory(dir);
        }
        return dir;
    }

    static string GetPlayerUID(PlayerIdentity identity)
    {
        if (!identity) return "";

        string uid = identity.GetPlainId();
        if (uid == "")
        {
            uid = identity.GetId();
        }

        return uid;
    }

    static string GetPlayerFilePath(PlayerIdentity identity)
    {
        string uid = GetPlayerUID(identity);
        if (uid == "") return "";
        return GetDirPath() + uid + ".json";
    }

    static string GetZoneConfigPath()
    {
        return GetDirPath() + "config.json";
    }

    static string GetLegacyZoneConfigPath()
    {
        return GetDirPath() + "zones.json";
    }

    static RC_PlayerData LoadPlayerData(PlayerIdentity identity)
    {
        RC_PlayerData data = new RC_PlayerData();
        string path = GetPlayerFilePath(identity);

        if (path == "")
            return data;

        if (FileExist(path))
        {
            JsonFileLoader<RC_PlayerData>.JsonLoadFile(path, data);
        }
        else
        {
            SavePlayerData(identity, data);
        }

        return data;
    }

    static void SavePlayerData(PlayerIdentity identity, RC_PlayerData data)
    {
        string path = GetPlayerFilePath(identity);
        if (path == "")
            return;

        JsonFileLoader<RC_PlayerData>.JsonSaveFile(path, data);
    }

    static void EnsurePlayerFile(PlayerIdentity identity)
    {
        RC_PlayerData data = LoadPlayerData(identity);
        SavePlayerData(identity, data);
    }

    static void EnsureZoneConfig()
    {
        string path = GetZoneConfigPath();
        string legacyPath = GetLegacyZoneConfigPath();

        if (!FileExist(path) && FileExist(legacyPath))
        {
            s_ZoneConfig = new RC_ZoneConfig();
            JsonFileLoader<RC_ZoneConfig>.JsonLoadFile(legacyPath, s_ZoneConfig);
            if (!s_ZoneConfig || !s_ZoneConfig.ForbiddenZones)
            {
                s_ZoneConfig = new RC_ZoneConfig();
            }

            JsonFileLoader<RC_ZoneConfig>.JsonSaveFile(path, s_ZoneConfig);
            Print("[RespawnCoin] Migrated legacy config to: " + path);
        }

        if (FileExist(path))
        {
            s_ZoneConfig = new RC_ZoneConfig();
            JsonFileLoader<RC_ZoneConfig>.JsonLoadFile(path, s_ZoneConfig);
            if (!s_ZoneConfig || !s_ZoneConfig.ForbiddenZones)
            {
                s_ZoneConfig = new RC_ZoneConfig();
                JsonFileLoader<RC_ZoneConfig>.JsonSaveFile(path, s_ZoneConfig);
            }

            if (s_ZoneConfig.RespawnInvulnerabilitySeconds < 0)
            {
                s_ZoneConfig.RespawnInvulnerabilitySeconds = 0;
                JsonFileLoader<RC_ZoneConfig>.JsonSaveFile(path, s_ZoneConfig);
            }
            return;
        }

        s_ZoneConfig = new RC_ZoneConfig();

        RC_ForbiddenZone toxic = new RC_ForbiddenZone();
        toxic.Name = "Example Toxic Zone";
        toxic.Type = "Toxic";
        toxic.Center = "7500 0 7500";
        toxic.Radius = 300;

        RC_ForbiddenZone boss = new RC_ForbiddenZone();
        boss.Name = "Example Boss Zone";
        boss.Type = "Boss";
        boss.Center = "8500 0 8500";
        boss.Radius = 250;

        RC_ForbiddenZone other = new RC_ForbiddenZone();
        other.Name = "Example Other Zone";
        other.Type = "Other";
        other.Center = "9500 0 9500";
        other.Radius = 250;

        s_ZoneConfig.ForbiddenZones.Insert(toxic);
        s_ZoneConfig.ForbiddenZones.Insert(boss);
        s_ZoneConfig.ForbiddenZones.Insert(other);

        JsonFileLoader<RC_ZoneConfig>.JsonSaveFile(path, s_ZoneConfig);

        Print("[RespawnCoin] Created default zones config: " + path);
    }

    static RC_ZoneConfig GetZoneConfig()
    {
        if (!s_ZoneConfig)
        {
            EnsureZoneConfig();
        }
        return s_ZoneConfig;
    }

    static void ReloadZoneConfig()
    {
        s_ZoneConfig = null;
        EnsureZoneConfig();
    }

    static float GetRespawnInvulnerabilityDurationSeconds()
    {
        RC_ZoneConfig cfg = GetZoneConfig();
        if (!cfg || !cfg.EnableRespawnInvulnerability)
            return 0;

        if (cfg.RespawnInvulnerabilitySeconds <= 0)
            return 0;

        return cfg.RespawnInvulnerabilitySeconds;
    }

    static void AddCoins(PlayerIdentity identity, int amount)
    {
        if (!identity || amount == 0) return;

        RC_PlayerData data = LoadPlayerData(identity);
        data.m_Coins = data.m_Coins + amount;

        if (data.m_Coins < 0)
            data.m_Coins = 0;

        SavePlayerData(identity, data);

        Print("[RespawnCoin] AddCoins uid=" + GetPlayerUID(identity) + " amount=" + amount.ToString() + " now=" + data.m_Coins.ToString());
    }

    static void MarkDeath(PlayerBase player)
    {
        if (!GetGame().IsServer()) return;
        if (!player) return;

        PlayerIdentity identity = player.GetIdentity();
        if (!identity) return;

        RC_PlayerData data = LoadPlayerData(identity);
        data.m_HasPendingRespawn = true;
        data.m_DeathPos = player.GetPosition();
        data.m_LastBlockReason = "";
        data.m_LastDeathTime = GetGame().GetTime();

        SavePlayerData(identity, data);

        Print("[RespawnCoin] MarkDeath uid=" + GetPlayerUID(identity) + " pos=" + data.m_DeathPos);
    }

    static void ClearPendingRespawn(PlayerIdentity identity)
    {
        if (!identity) return;

        RC_PlayerData data = LoadPlayerData(identity);
        data.m_HasPendingRespawn = false;
        data.m_LastBlockReason = "";
        SavePlayerData(identity, data);

        Print("[RespawnCoin] ClearPendingRespawn uid=" + GetPlayerUID(identity));
    }

    static bool IsPositionInForbiddenZone(vector pos, out string reasonKey, out string zoneName)
    {
        reasonKey = "";
        zoneName = "";

        RC_ZoneConfig cfg = GetZoneConfig();
        if (!cfg || !cfg.ForbiddenZones)
            return false;

        foreach (RC_ForbiddenZone zone : cfg.ForbiddenZones)
        {
            if (!zone)
                continue;

            float dx = pos[0] - zone.Center[0];
            float dz = pos[2] - zone.Center[2];
            float distSq = dx * dx + dz * dz;
            float radiusSq = zone.Radius * zone.Radius;

            if (distSq <= radiusSq)
            {
                reasonKey = RC_Localization.GetForbiddenZoneReasonKey(zone.Type);
                zoneName = zone.Name;

                return true;
            }
        }

        return false;
    }

    static bool CanUseRespawnCoin(PlayerIdentity identity, out string reasonKey, out string zoneName)
    {
        reasonKey = "";
        zoneName = "";

        if (!identity)
        {
            reasonKey = "#STR_RC_REASON_INVALID_IDENTITY";
            return false;
        }

        RC_PlayerData data = LoadPlayerData(identity);

        if (!data.m_HasPendingRespawn)
        {
            reasonKey = "#STR_RC_REASON_NO_DEATH_RECORD";
            return false;
        }

        if (data.m_Coins <= 0)
        {
            reasonKey = "#STR_RC_REASON_NO_COINS";
            return false;
        }

        if (IsPositionInForbiddenZone(data.m_DeathPos, reasonKey, zoneName))
        {
            return false;
        }

        return true;
    }

    static void SendDialogStateTo(PlayerIdentity identity)
    {
        if (!GetGame().IsServer()) return;
        if (!identity) return;

        RC_PlayerData data = LoadPlayerData(identity);

        if (!data.m_HasPendingRespawn)
            return;

        string reasonKey = "";
        string zoneName = "";
        bool canUse = CanUseRespawnCoin(identity, reasonKey, zoneName);
        string reason = RC_Localization.ResolveReasonText(reasonKey, zoneName);

        data.m_LastBlockReason = reason;
        SavePlayerData(identity, data);

        Param5<int, bool, vector, string, string> payload = new Param5<int, bool, vector, string, string>(data.m_Coins, canUse, data.m_DeathPos, reasonKey, zoneName);

        GetRPCManager().SendRPC(RC_MOD_NAME, RC_RPC_SHOW_DIALOG, payload, true, identity, null);

        Print("[RespawnCoin] SendDialogStateTo uid=" + GetPlayerUID(identity) + " coins=" + data.m_Coins + " canUse=" + canUse.ToString() + " reason=" + reason);
    }

    static bool TryUseCoinAndRespawn(PlayerBase player, out string failReason)
    {
        failReason = "";

        if (!GetGame().IsServer()) 
        {
            failReason = RC_Localization.Translate("#STR_RC_FAIL_NOT_SERVER");
            return false;
        }

        if (!player) 
        {
            failReason = RC_Localization.Translate("#STR_RC_FAIL_INVALID_PLAYER");
            return false;
        }

        PlayerIdentity identity = player.GetIdentity();
        if (!identity) 
        {
            failReason = RC_Localization.Translate("#STR_RC_FAIL_INVALID_PLAYER_IDENTITY");
            return false;
        }

        string failReasonKey = "";
        string failZoneName = "";
        if (!CanUseRespawnCoin(identity, failReasonKey, failZoneName))
        {
            failReason = RC_Localization.ResolveReasonText(failReasonKey, failZoneName);
            Print("[RespawnCoin] TryUseCoinAndRespawn failed uid=" + GetPlayerUID(identity) + " reason=" + failReason);
            return false;
        }

        RC_PlayerData data = LoadPlayerData(identity);

        data.m_Coins = data.m_Coins - 1;
        data.m_HasPendingRespawn = false;
        data.m_LastBlockReason = "";
        SavePlayerData(identity, data);

        vector targetPos = data.m_DeathPos;
        targetPos[1] = targetPos[1] + 0.15;
        player.SetPosition(targetPos);

        float invulnerabilityDuration = GetRespawnInvulnerabilityDurationSeconds();
        if (invulnerabilityDuration > 0)
        {
            player.RC_StartRespawnInvulnerability(invulnerabilityDuration);

            Param1<int> payload = new Param1<int>(Math.Ceil(invulnerabilityDuration));
            GetRPCManager().SendRPC(RC_MOD_NAME, RC_RPC_START_INVULNERABILITY, payload, true, identity, player);
        }

        Print("[RespawnCoin] Respawn success uid=" + GetPlayerUID(identity) + " leftCoins=" + data.m_Coins + " targetPos=" + targetPos + " invulnerability=" + invulnerabilityDuration.ToString());
        return true;
    }

    static PlayerBase FindPlayerByIdentity(PlayerIdentity identity)
    {
        if (!identity) return null;

        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);

        foreach (Man man : players)
        {
            PlayerBase pb = PlayerBase.Cast(man);
            if (!pb) continue;

            PlayerIdentity pid = pb.GetIdentity();
            if (!pid) continue;

            if (pid.GetId() == identity.GetId())
            {
                return pb;
            }
        }

        return null;
    }
}
