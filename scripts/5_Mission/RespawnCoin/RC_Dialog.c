ref RC_DialogManager g_RC_DialogManager;

class RC_DialogManager : ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    protected TextWidget m_Title;
    protected TextWidget m_Content;
    protected TextWidget m_Reason;
    protected ButtonWidget m_BtnConfirm;
    protected ButtonWidget m_BtnCancel;
    protected TextWidget m_BtnConfirmLabel;
    protected TextWidget m_BtnCancelLabel;

    protected Widget m_CurrentlyHighlighted;
    protected bool m_IsOpen = false;
    protected bool m_CanUse = false;

    static RC_DialogManager GetInstance()
    {
        if (!g_RC_DialogManager)
        {
            g_RC_DialogManager = new RC_DialogManager();
        }
        return g_RC_DialogManager;
    }

    static void ShowDialog(int coins, bool canUse, vector deathPos, string reasonKey, string zoneName)
    {
        RC_DialogManager mgr = GetInstance();
        if (!mgr) return;
        mgr.Show(coins, canUse, deathPos, reasonKey, zoneName);
    }

    protected bool Init()
    {
        if (m_Root)
            return true;

        WorkspaceWidget workspace = GetGame().GetWorkspace();
        if (!workspace)
        {
            Print("[RespawnCoin] Init failed: workspace is null");
            return false;
        }

        m_Root = workspace.CreateWidgets("RespawnCoin/gui/layouts/respawncoin_dialog.layout");
        if (!m_Root)
        {
            Print("[RespawnCoin] Init failed: CreateWidgets returned null");
            return false;
        }

        m_Root.SetHandler(this);

        m_Title      = TextWidget.Cast(m_Root.FindAnyWidget("RC_Title"));
        m_Content    = TextWidget.Cast(m_Root.FindAnyWidget("RC_Content"));
        m_Reason          = TextWidget.Cast(m_Root.FindAnyWidget("RC_Reason"));
        m_BtnConfirm      = ButtonWidget.Cast(m_Root.FindAnyWidget("RC_BtnConfirm"));
        m_BtnCancel       = ButtonWidget.Cast(m_Root.FindAnyWidget("RC_BtnCancel"));
        m_BtnConfirmLabel = TextWidget.Cast(m_Root.FindAnyWidget("RC_BtnConfirm_label"));
        m_BtnCancelLabel  = TextWidget.Cast(m_Root.FindAnyWidget("RC_BtnCancel_label"));

        if (!m_Title)      Print("[RespawnCoin] Init warning: RC_Title not found");
        if (!m_Content)    Print("[RespawnCoin] Init warning: RC_Content not found");
        if (!m_Reason)     Print("[RespawnCoin] Init warning: RC_Reason not found");
        if (!m_BtnConfirm)      Print("[RespawnCoin] Init warning: RC_BtnConfirm not found");
        if (!m_BtnCancel)       Print("[RespawnCoin] Init warning: RC_BtnCancel not found");
        if (!m_BtnConfirmLabel) Print("[RespawnCoin] Init warning: RC_BtnConfirm_label not found");
        if (!m_BtnCancelLabel)  Print("[RespawnCoin] Init warning: RC_BtnCancel_label not found");

        RC_Localization.SetText(m_Title, "#STR_RC_DIALOG_TITLE");
        RC_Localization.SetText(m_BtnConfirmLabel, "#STR_RC_BUTTON_CONFIRM");
        RC_Localization.SetText(m_BtnCancelLabel, "#STR_RC_BUTTON_CANCEL");

        m_Root.Show(false);
        return true;
    }

    void Show(int coins, bool canUse, vector deathPos, string reasonKey, string zoneName)
    {
        if (!Init())
            return;

        m_CanUse = canUse;

        if (m_Title)
        {
            RC_Localization.SetText(m_Title, "#STR_RC_DIALOG_TITLE");
        }

        if (m_BtnConfirmLabel)
        {
            RC_Localization.SetText(m_BtnConfirmLabel, "#STR_RC_BUTTON_CONFIRM");
        }

        if (m_BtnCancelLabel)
        {
            RC_Localization.SetText(m_BtnCancelLabel, "#STR_RC_BUTTON_CANCEL");
        }

        if (m_Content)
        {
            m_Content.SetText(RC_Localization.FormatCoinsMessage(coins));
        }

        if (m_Reason)
        {
            string reason = RC_Localization.ResolveReasonText(reasonKey, zoneName);

            if (reason != "")
            {
                m_Reason.SetText(reason);
                m_Reason.SetColor(ARGB(255, 220, 70, 70));
            }
            else if (canUse)
            {
                m_Reason.SetText(RC_Localization.Translate("#STR_RC_DIALOG_CONFIRM_HINT"));
                m_Reason.SetColor(ARGB(255, 210, 210, 210));
            }
            else
            {
                m_Reason.SetText(RC_Localization.Translate("#STR_RC_DIALOG_CANNOT_USE"));
                m_Reason.SetColor(ARGB(255, 220, 70, 70));
            }

            m_Reason.Show(true);
        }

        if (m_BtnConfirm)
        {
            m_BtnConfirm.Enable(canUse);
            m_BtnConfirm.Show(true);
            ColorNormal(m_BtnConfirm);
        }

        if (m_BtnCancel)
        {
            m_BtnCancel.Enable(true);
            m_BtnCancel.Show(true);
            ColorNormal(m_BtnCancel);
        }

        m_Root.Show(true);
        m_IsOpen = true;

        LockInput(true);

        if (canUse && m_BtnConfirm)
            SetFocus(m_BtnConfirm);
        else if (m_BtnCancel)
            SetFocus(m_BtnCancel);
    }

    void Hide()
    {
        if (m_Root)
        {
            m_Root.Show(false);
        }

        m_IsOpen = false;
        m_CanUse = false;
        m_CurrentlyHighlighted = null;
        LockInput(false);
    }

    protected void LockInput(bool state)
    {
        if (state)
        {
            GetGame().GetInput().ChangeGameFocus(1);
            GetGame().GetUIManager().ShowUICursor(true);
        }
        else
        {
            GetGame().GetUIManager().ShowUICursor(false);
            GetGame().GetInput().ResetGameFocus();
        }
    }

    protected bool IsFocusable(Widget w)
    {
        if (!w) return false;

        if (w == m_BtnCancel)
            return true;

        if (w == m_BtnConfirm && m_CanUse)
            return true;

        return false;
    }

    protected void ButtonSetColor(Widget w, int color)
    {
        Widget panel = w.FindWidget(w.GetName() + "_panel");
        if (panel)
            panel.SetColor(color);
    }

    protected void ButtonSetTextColor(Widget w, int color)
    {
        TextWidget label = TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
        if (label)
            label.SetColor(color);
    }

    protected void ColorHighlight(Widget w)
    {
        if (!w)
            return;

        if (m_CurrentlyHighlighted != w)
        {
            if (m_CurrentlyHighlighted)
                ColorNormal(m_CurrentlyHighlighted);

            m_CurrentlyHighlighted = w;
        }

        ButtonSetColor(w, ARGB(220, 20, 20, 20));
        ButtonSetTextColor(w, ARGB(255, 215, 70, 70));
    }

    protected void ColorDisabled(Widget w)
    {
        if (!w)
            return;

        ButtonSetColor(w, ARGB(120, 0, 0, 0));
        ButtonSetTextColor(w, ARGB(255, 120, 120, 120));
    }

    protected void ColorNormal(Widget w)
    {
        if (!w)
            return;

        if (w == m_BtnConfirm && !m_CanUse)
        {
            ColorDisabled(w);
            return;
        }

        ButtonSetColor(w, ARGB(180, 0, 0, 0));
        ButtonSetTextColor(w, ARGB(255, 255, 255, 255));
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (!m_IsOpen)
            return false;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return false;

        if (w == m_BtnConfirm && m_CanUse)
        {
            GetRPCManager().SendRPC(RC_MOD_NAME, RC_RPC_CONFIRM_USE, null, true, null, player);
            Hide();
            return true;
        }

        if (w == m_BtnCancel)
        {
            GetRPCManager().SendRPC(RC_MOD_NAME, RC_RPC_CANCEL_USE, null, true, null, player);
            Hide();
            return true;
        }

        return false;
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
        if (!m_IsOpen) return false;

        if (IsFocusable(w))
        {
            ColorHighlight(w);
            return true;
        }

        return false;
    }

    override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        if (!m_IsOpen) return false;

        if (w == m_CurrentlyHighlighted)
            m_CurrentlyHighlighted = null;

        if (w == m_BtnConfirm || w == m_BtnCancel)
        {
            ColorNormal(w);
            return true;
        }

        return false;
    }

    override bool OnFocus(Widget w, int x, int y)
    {
        if (!m_IsOpen) return false;

        if (IsFocusable(w))
        {
            ColorHighlight(w);
            return true;
        }

        return false;
    }

    override bool OnFocusLost(Widget w, int x, int y)
    {
        if (!m_IsOpen) return false;

        if (w == m_CurrentlyHighlighted)
            m_CurrentlyHighlighted = null;

        if (w == m_BtnConfirm || w == m_BtnCancel)
        {
            ColorNormal(w);
            return true;
        }

        return false;
    }
}
