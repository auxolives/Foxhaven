class FoxhavenOral
{
	static bool IsBlocked(PlayerBase p)
	{
		return p && p.GetEatDrinkRestriction() != Foxhaven_EatDrinkRestriction.NONE;
	}

	static bool IsTargetBlocked(PlayerBase target)
	{
		return IsBlocked(target);
	}

	static bool AllowIfConsumable(PlayerBase p, ItemBase item)
	{
		if (!p || !item) return false;
		ConsumeConditionData dta = new ConsumeConditionData(p, item);
		return p.CanConsumeFood(dta) && item.CanBeConsumed(dta) && item.GetQuantity() > item.GetQuantityMin();
	}

	static string Suffix(PlayerBase p)
	{
		if (!p) return "";
		string suffixText = "";
		if (IsBlocked(p))
		{
			suffixText = " " + Widget.TranslateString("#STR_Foxhaven_BlockedByGear");
		}
		return suffixText;
	}

	static void AbortSoon(ActionData ad)
	{
		if (!ad || !ad.m_Player) return;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FoxhavenOral.__AbortDirect, 1, false, ad);
	}

	static void AbortAfter(ActionData ad, int delay_ms)
	{
		if (!ad || !ad.m_Player) return;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(FoxhavenOral.__AbortDirect, delay_ms, false, ad);
	}

	static void __AbortDirect(ActionData ad)
	{
		if (!ad || !ad.m_Player) return;
		ActionDrinkPondContinuous pond = ActionDrinkPondContinuous.Cast(ad.m_Action);
		if (pond) ad.m_Player.TryHideItemInHands(false);
		if (ad.m_Callback) ad.m_Callback.Cancel();
		ad.m_Player.GetActionManager().Interrupt();
	}
}