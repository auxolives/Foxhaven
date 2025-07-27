modded class Backpack_Base
{
	bool IsInventoryAlwaysAccessible()
	{
		return false;
	}

	override bool CanDisplayCargo()
	{
		PlayerSettings settings = FoxhavenConfig.GetInstance().GetplayerSettings();
		
		if (!settings || !settings.isRestrictedBagAccessEnabled)
		{
			return super.CanDisplayCargo();
		}

		if (IsInventoryAlwaysAccessible())
		{
			return true;
		}

		if (!GetHierarchyParent())
		{
			return true;
		}

		PlayerBase player = PlayerBase.Cast(GetHierarchyParent());
		if (player && player.GetItemInHands() == this)
		{
			return true;
		}
		
		if (player)
		{
			return false;
		}
		
		return super.CanDisplayCargo();
	}
}
