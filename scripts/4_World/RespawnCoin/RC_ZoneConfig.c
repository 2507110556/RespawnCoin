class RC_ForbiddenZone
{
    string Name = "";
    string Type = "Other"; // Toxic / Boss / Other
    vector Center = "0 0 0";
    float Radius = 100.0;
}

class RC_ZoneConfig
{
    bool EnableRespawnInvulnerability = true;
    int RespawnInvulnerabilitySeconds = 15;
    ref array<ref RC_ForbiddenZone> ForbiddenZones;

    void RC_ZoneConfig()
    {
        ForbiddenZones = new array<ref RC_ForbiddenZone>;
    }
}
