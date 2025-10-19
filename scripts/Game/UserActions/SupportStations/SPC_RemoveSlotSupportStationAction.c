// ============================================================================
// File: scripts/Game/UserActions/SupportStations/SPC_RemoveSlotSupportStationAction.c
// Desc: Remove (detach & delete) whatever is in a named SlotManager slot via
//       Support Station. No refunds are given.
// Notes:
//   * No ternary operator used.
//   * Shares the same Support Station gating as attach (e.g., REPAIR).
// Author: SPC_
// ============================================================================

class SPC_RemoveSlotSupportStationAction : SCR_BaseAudioSupportStationAction
{
	[Attribute(defvalue: "REPAIR", desc: "Which Support Station type gates this action.", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ESupportStationType))]
	protected ESupportStationType m_eStationType;

	[Attribute("1", desc: "Require wrench gadget like repair actions")]
	protected bool m_bRequiresWrench;

	[Attribute(defvalue: "Turret_Decal", desc: "Exact SlotManager slot name on the vehicle")]
	protected string m_sSlotName;

	// --- Runtime -------------------------------------------------------------
	protected bool m_bInitDone;
	protected SlotManagerComponent m_SlotMgr;
	protected EntitySlotInfo m_Slot;

	// ------------------------------------------------------------------------
	protected override ESupportStationType GetSupportStationType()
	{
		return m_eStationType;
	}

	// ------------------------------------------------------------------------
	protected override bool RequiresGadget()
	{
		return m_bRequiresWrench;
	}

	// ------------------------------------------------------------------------
	protected void EnsureInit()
	{
		if (m_bInitDone)
			return;

		IEntity owner = GetOwner();
		if (!owner)
			return;

		m_SlotMgr = SlotManagerComponent.Cast(owner.FindComponent(SlotManagerComponent));
		m_bInitDone = true;
	}

	// ------------------------------------------------------------------------
	protected override void OnActionSelected()
	{
		super.OnActionSelected();
		EnsureInit();
	}

	// ------------------------------------------------------------------------
	protected override void OnActionDeselected()
	{
		super.OnActionDeselected();
		// No ghost here; nothing to clean.
	}

	// ------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		EnsureInit();

		if (!GetOwner())
			return false;

		if (!m_SlotMgr)
			return false;

		if (m_sSlotName.IsEmpty())
			return false;

		m_Slot = m_SlotMgr.GetSlotByName(m_sSlotName);
		if (!m_Slot)
			return false;

		if (!m_Slot.IsEnabled())
			return false;

		// Only show if there is something attached to remove
		if (!m_Slot.GetAttachedEntity())
			return false;

		return super.CanBeShownScript(user);
	}

	// ------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		EnsureInit();

		if (!m_SlotMgr)
			return false;

		m_Slot = m_SlotMgr.GetSlotByName(m_sSlotName);
		if (!m_Slot)
			return false;

		if (!m_Slot.GetAttachedEntity())
		{
			SetCanPerform(false, ESupportStationReasonInvalid.RESUPPLY_NOT_IN_STORAGE);
			return false;
		}

		return super.CanBePerformedScript(user);
	}

	// ------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		EnsureInit();

		if (!pOwnerEntity)
			return;

		if (!m_SlotMgr)
			return;

		m_Slot = m_SlotMgr.GetSlotByName(m_sSlotName);
		if (!m_Slot)
			return;

		IEntity child = m_Slot.GetAttachedEntity();
		if (!child)
			return;

		// Detach (void). Parameter indicates whether to apply physical change if supported.
		m_Slot.DetachEntity(true);

		// Delete the now-detached replicated child entity
		RplComponent rpl = RplComponent.Cast(child.FindComponent(RplComponent));
		if (rpl)
		{
			rpl.DeleteRplEntity(child, true);
		}
		else
		{
			delete child;
		}
	}

	// ------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		UIInfo uiInfo = GetUIInfo();
		if (!uiInfo)
			return super.GetActionNameScript(outName);

		outName = uiInfo.GetName(); // e.g., "Remove Upgrade"
		return super.GetActionNameScript(outName);
	}
}
