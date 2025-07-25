modded class Shemag_ColorBase
{
    override bool CanBeEatenOrDrunkThrough(int slot_id)
    {
        if (slot_id == InventorySlots.MASK)
        {
            return false;
        }

        return true;
    }
}
