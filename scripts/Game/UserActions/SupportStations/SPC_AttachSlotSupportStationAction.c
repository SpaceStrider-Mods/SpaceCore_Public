// ============================================================================
// File: scripts/Game/UserActions/SupportStations/SPC_AttachSlotSupportStationAction.c
// Desc: Install a prefab into a named SlotManager slot via Support Station.
//       - One-per-slot (hides when occupied)
//       - Requires wrench (if enabled)
//       - Client-only ghost preview (non-replicated), positioned using slot xform
// Notes:
//   * No ternary operator used.
//   * No EOnDelete override (UserAction base doesn’t expose it).
//   * We DO NOT attach the ghost to the slot (to avoid affecting slot state).
//   * Replicated spawn happens server-side via Support Station perform.
// Author: SPC_
// ============================================================================

class SPC_AttachSlotSupportStationAction : SCR_BaseAudioSupportStationAction
{
	// --- Designer attributes -------------------------------------------------
	[Attribute(defvalue: "REPAIR", desc: "Which Support Station type gates this action.", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ESupportStationType))]
	protected ESupportStationType m_eStationType;

	[Attribute("1", desc: "Require wrench gadget like repair actions")]
	protected bool m_bRequiresWrench;

	[Attribute(defvalue: "Turret_Decal", desc: "Exact SlotManager slot name on the vehicle")]
	protected string m_sSlotName;

	[Attribute(UIWidgets.ResourceNamePicker, desc: "Attachment prefab to spawn & attach", params: "et")]
	protected ResourceName m_sAttachmentPrefab;

	// Provide a simplified, client-only ghost prefab (NO RplComponent, NO physics).
	// If empty, we will use the attachment prefab itself as the ghost.
	[Attribute("", UIWidgets.ResourceNamePicker, desc: "Optional ghost/preview prefab (client-only). If empty uses AttachmentPrefab.", params: "et")]
	protected ResourceName m_sGhostPrefab;

	[Attribute("", UIWidgets.LocaleEditBox, desc: "Optional UI display name token; if empty, prefab filename is used")]
	protected LocalizedString m_sDisplayNameOverride;

	// If you want fixed costs on this action, your station manager can read it via GetSupplyCostOverride().
	[Attribute("0", desc: "Optional fixed supply cost override (0 = let station/catalog handle it)")]
	protected int m_iSupplyCostOverride;

	// Optional: action hold duration (seconds). If your setup uses a different source for duration, ignore this.
	[Attribute("2.5", UIWidgets.Slider, desc: "Hold duration seconds (used if your framework reads it)", params: "0 20 0.1")]
	protected float m_fDuration;

	// --- Runtime -------------------------------------------------------------
	protected bool m_bInitDone;
	protected SlotManagerComponent m_SlotMgr;
	protected EntitySlotInfo m_Slot;

	protected ResourceName m_GhostToSpawn;
	protected IEntity m_LocalGhost; // client-only

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

		if (m_sGhostPrefab.IsEmpty())
		{
			m_GhostToSpawn = m_sAttachmentPrefab;
		}
		else
		{
			m_GhostToSpawn = m_sGhostPrefab;
		}

		m_bInitDone = true;
	}

	// ------------------------------------------------------------------------
	protected override void OnActionSelected()
	{
		super.OnActionSelected();
		EnsureInit();

		// If your framework reads duration from the action instance, you may set it here.
		// SetDuration(m_fDuration); // Uncomment if your build supports this call.
	}

	// ------------------------------------------------------------------------
	protected override void OnActionDeselected()
	{
		super.OnActionDeselected();
		DestroyGhost();
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

		if (m_sAttachmentPrefab.IsEmpty())
			return false;

		m_Slot = m_SlotMgr.GetSlotByName(m_sSlotName);
		if (!m_Slot)
			return false;

		if (!m_Slot.IsEnabled())
			return false;

		// One-per-slot: hide if already something there
		if (m_Slot.GetAttachedEntity())
			return false;

		bool canShow = super.CanBeShownScript(user);

		// Maintain client-only ghost while action is visible/selectable
		UpdateGhostPreview(canShow);

		return canShow;
	}

	// ------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		EnsureInit();

		if (!m_SlotMgr)
			return false;

		// Must still be empty at perform-time
		m_Slot = m_SlotMgr.GetSlotByName(m_sSlotName);
		if (!m_Slot)
			return false;

		if (m_Slot.GetAttachedEntity())
		{
			SetCanPerform(false, ESupportStationReasonInvalid.RESUPPLY_INVENTORY_FULL);
			return false;
		}

		return super.CanBePerformedScript(user);
	}

	// ------------------------------------------------------------------------
	//! Server-authoritative spawn & attach (Support Station calls PerformAction on master).
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

		// Still free?
		if (m_Slot.GetAttachedEntity())
			return;

		Resource res = Resource.Load(m_sAttachmentPrefab);
		if (!res)
			return;

		if (!res.IsValid())
			return;

		ArmaReforgerScripted game = ArmaReforgerScripted.Cast(GetGame());
		if (!game)
			return;

		IEntity child = game.SpawnEntityPrefab(res, null, null);
		if (!child)
		{
			Print("SPC_AttachSlotSupportStationAction: spawn failed", LogLevel.ERROR);
			return;
		}

		// Attach (void), then verify
		m_Slot.AttachEntity(child);

		if (m_Slot.GetAttachedEntity() != child)
		{
			RplComponent rpl = RplComponent.Cast(child.FindComponent(RplComponent));
			if (rpl)
			{
				rpl.DeleteRplEntity(child, true);
			}
			else
			{
				delete child;
			}

			Print("SPC_AttachSlotSupportStationAction: attach verification failed", LogLevel.ERROR);
			return;
		}
	}

	// ------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		UIInfo uiInfo = GetUIInfo();
		if (!uiInfo)
			return super.GetActionNameScript(outName);

		string token;

		if (m_sDisplayNameOverride.IsEmpty())
		{
			token = m_sAttachmentPrefab;

			int slash = token.LastIndexOf("/");
			if (slash >= 0 && slash < token.Length() - 1)
			{
				token = token.Substring(slash + 1, token.Length() - slash - 1);
			}
		}
		else
		{
			token = m_sDisplayNameOverride;
		}

		outName = WidgetManager.Translate(uiInfo.GetName(), token); // e.g. "Install %1"
		return super.GetActionNameScript(outName);
	}

	// ------------------------------------------------------------------------
	// Client-only ghost management (spawn local, position by world transform)
	protected void UpdateGhostPreview(bool shouldExist)
	{
		// Use replication role to detect client
		if (!Replication.IsClient())
			return;

		// No slot yet? ensure cleanup
		if (!m_Slot)
		{
			DestroyGhost();
			return;
		}

		if (shouldExist)
		{
			if (!m_LocalGhost)
			{
				Resource ghostRes = Resource.Load(m_GhostToSpawn);
				if (!ghostRes)
					return;

				if (!ghostRes.IsValid())
					return;

				// We can use normal SpawnEntityPrefab because the ghost prefab MUST have NO RplComponent,
				// so it will stay local on the client.
				ArmaReforgerScripted game = ArmaReforgerScripted.Cast(GetGame());
				if (!game)
					return;

				m_LocalGhost = game.SpawnEntityPrefab(ghostRes, null, null);
			}

			if (m_LocalGhost)
			{
				// Compute world transform of slot by combining owner world with slot local matrix
				vector ownerW[4];
				vector slotL[4];
				vector worldM[4];

				IEntity owner = GetOwner();
				if (!owner)
					return;

				owner.GetWorldTransform(ownerW);
				m_Slot.GetTransform(slotL);
				Math3D.MatrixMultiply4(ownerW, slotL, worldM);

				m_LocalGhost.SetWorldTransform(worldM);
			}
		}
		else
		{
			DestroyGhost();
		}
	}

	protected void DestroyGhost()
	{
		if (m_LocalGhost)
		{
			delete m_LocalGhost;
			m_LocalGhost = null;
		}
	}

	// ------------------------------------------------------------------------
	int GetSupplyCostOverride()
	{
		return m_iSupplyCostOverride;
	}
}
