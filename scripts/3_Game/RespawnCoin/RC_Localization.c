class RC_Localization
{
    static string Translate(string key)
    {
        return Normalize(Widget.TranslateString(key));
    }

    static string Normalize(string text)
    {
        text.Replace("\\n", "\n");
        return text;
    }

    static void SetText(TextWidget widget, string key)
    {
        if (!widget)
            return;

        widget.SetText(Translate(key));
    }

    static string FormatCoinsMessage(int coins)
    {
        string text = Translate("#STR_RC_DIALOG_CONTENT");
        text.Replace("{coins}", coins.ToString());
        return text;
    }

    static string FormatZoneReason(string baseReason, string zoneName)
    {
        if (zoneName == "")
            return baseReason;

        string text = Translate("#STR_RC_REASON_ZONE_APPEND");
        text.Replace("{reason}", baseReason);
        text.Replace("{zone}", zoneName);
        return text;
    }

    static string FormatVoucherGainMessage(int addCoins)
    {
        string text = Translate("#STR_RC_STATUS_VOUCHER_GAIN");
        text.Replace("{coins}", addCoins.ToString());
        return text;
    }

    static string FormatInvulnerabilityCountdown(int secondsRemaining)
    {
        string text = Translate("#STR_RC_STATUS_INVULNERABILITY");
        text.Replace("{seconds}", secondsRemaining.ToString());
        return text;
    }

    static string ResolveReasonText(string reasonKey, string zoneName = "")
    {
        if (reasonKey == "")
            return "";

        string reason = Translate(reasonKey);
        return FormatZoneReason(reason, zoneName);
    }

    static string GetForbiddenZoneReasonKey(string zoneType)
    {
        string zoneTypeLower = zoneType;
        zoneTypeLower.ToLower();

        if (zoneTypeLower == "toxic")
            return "#STR_RC_REASON_FORBIDDEN_TOXIC";

        if (zoneTypeLower == "boss")
            return "#STR_RC_REASON_FORBIDDEN_BOSS";

        return "#STR_RC_REASON_FORBIDDEN_GENERIC";
    }
}
