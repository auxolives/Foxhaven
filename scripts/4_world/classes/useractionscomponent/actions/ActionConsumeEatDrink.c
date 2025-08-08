string GetRestrictionText(PlayerBase subject)
{
	return FoxhavenOral.Suffix(subject);
}

modded class ActionConsume
{
	override string GetText()
	{
		return super.GetText() + FoxhavenOral.Suffix(PlayerBase.Cast(GetGame().GetPlayer()));
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
		{
			if (FoxhavenOral.IsBlocked(player))
				return FoxhavenOral.AllowIfConsumable(player, item);
			return false;
		}
		return true;
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) return false;
		return super.ActionConditionContinue(action_data);
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) FoxhavenOral.AbortAfter(action_data, 800);
	}
}

modded class ActionConsumeSingle
{
	override string GetText()
	{
		return super.GetText() + FoxhavenOral.Suffix(PlayerBase.Cast(GetGame().GetPlayer()));
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
		{
			if (FoxhavenOral.IsBlocked(player))
				return FoxhavenOral.AllowIfConsumable(player, item);
			return false;
		}
		return true;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		if (FoxhavenOral.IsBlocked(action_data.m_Player))
			return;
		super.OnExecuteServer(action_data);
	}
}


modded class ActionDrinkPondContinuous
{
	override string GetText()
	{
		return super.GetText() + FoxhavenOral.Suffix(PlayerBase.Cast(GetGame().GetPlayer()));
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (FoxhavenOral.IsBlocked(player))
		{
			if (item && item.IsHeavyBehaviour()) return false;
			return true;
		}
		return super.ActionCondition(player, target, item);
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) return false;
		return super.ActionConditionContinue(action_data);
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) FoxhavenOral.AbortAfter(action_data, 1600);
	}
}


modded class ActionDrinkWellContinuous
{
	override string GetText()
	{
		return super.GetText() + FoxhavenOral.Suffix(PlayerBase.Cast(GetGame().GetPlayer()));
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (FoxhavenOral.IsBlocked(player))
		{
			if (item && item.IsHeavyBehaviour()) return false;
			return target.GetObject() && (target.GetObject().GetWaterSourceObjectType() == EWaterSourceObjectType.WELL || target.GetObject().IsWell());
		}
		return super.ActionCondition(player, target, item);
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) return false;
		return super.ActionConditionContinue(action_data);
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) FoxhavenOral.AbortAfter(action_data, 1600);
	}
}


modded class ActionEatSnowContinuous
{
	override string GetText()
	{
		return super.GetText() + FoxhavenOral.Suffix(PlayerBase.Cast(GetGame().GetPlayer()));
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (FoxhavenOral.IsBlocked(player))
		{
			if (item && item.IsHeavyBehaviour()) return false;
			return true;
		}
		return super.ActionCondition(player, target, item);
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) return false;
		return super.ActionConditionContinue(action_data);
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		if (FoxhavenOral.IsBlocked(action_data.m_Player)) FoxhavenOral.AbortAfter(action_data, 800);
	}
}


modded class ActionForceConsume
{
	protected bool m_FH_TargetBlocked;

	override string GetText()
	{
		string t = super.GetText();
		if (m_FH_TargetBlocked)
		{
			t = t + " " + Widget.TranslateString("#STR_Foxhaven_BlockedByGear");
		}
		return t;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		m_FH_TargetBlocked = false;
		if (!super.ActionCondition(player, target, item))
			return false;

		PlayerBase targetPlayer = PlayerBase.Cast(target.GetObject());
		if (!targetPlayer)
			return false;

		ConsumeConditionData dta = new ConsumeConditionData(targetPlayer, item);
		if (!targetPlayer.CanConsumeFood(dta) || !item.CanBeConsumed(dta))
			return false;
		float angleDiff = Math.AbsFloat(player.GetDirection().VectorToAngles()[0] - targetPlayer.GetDirection().VectorToAngles()[0]);
		if (angleDiff < 135 || angleDiff > 225)
			return false;
		if (targetPlayer.GetPerformedActionID() != -1 || targetPlayer.GetActivePrimarySymptomID() == SymptomIDs.SYMPTOM_VOMIT)
			return false;
		if (item.GetQuantity() <= item.GetQuantityMin())
			return false;

		m_FH_TargetBlocked = FoxhavenOral.IsTargetBlocked(targetPlayer);
		return true;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (targetPlayer && FoxhavenOral.IsTargetBlocked(targetPlayer))
			FoxhavenOral.AbortAfter(action_data, 800);
	}
}




modded class ActionForceConsumeSingle
{
	protected bool m_FH_TargetBlocked;

	override string GetText()
	{
		string t = super.GetText();
		if (m_FH_TargetBlocked)
		{
			t = t + " " + Widget.TranslateString("#STR_Foxhaven_BlockedByGear");
		}
		return t;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		m_FH_TargetBlocked = false;
		if (!super.ActionCondition(player, target, item))
			return false;

		PlayerBase targetPlayer = PlayerBase.Cast(target.GetObject());
		if (!targetPlayer)
			return false;

		if (item.GetQuantity() <= item.GetQuantityMin())
			return false;

		ConsumeConditionData dta = new ConsumeConditionData(targetPlayer, item);
		if (!targetPlayer.CanConsumeFood(dta) || !item.CanBeConsumed(dta))
			return false;

		m_FH_TargetBlocked = FoxhavenOral.IsTargetBlocked(targetPlayer);
		return true;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (targetPlayer && FoxhavenOral.IsTargetBlocked(targetPlayer))
			return;
		super.OnExecuteServer(action_data);
	}
}