class SPC_DetachMagazineUserAction : SCR_InventoryAction
{
	[Attribute("1")]
	protected bool m_bShowFromOutside;

	[Attribute("1")]
	protected bool m_bShowInside;
	
	[Attribute()]
	protected int m_iWeaponIndex;

	protected IEntity m_Vehicle;
	protected IEntity m_InventoryOwner;
	protected DamageManagerComponent m_DamageManager;
	
	protected SCR_VehicleInventoryStorageManagerComponent m_InventoryManager;
	
	protected BaseWeaponComponent m_WeaponComponent;
	
	override bool CanBeShownScript(IEntity user)
	{
		if (!user || !m_Vehicle || !m_InventoryOwner)
			return false;

		if (m_DamageManager && m_DamageManager.GetState() == EDamageState.DESTROYED)
			return false;
		
		if(!m_InventoryManager)
		{
			m_InventoryManager = SCR_VehicleInventoryStorageManagerComponent.Cast(user.GetRootParent().FindComponent(SCR_VehicleInventoryStorageManagerComponent));
		}
		
		ChimeraCharacter character = ChimeraCharacter.Cast(user);
		if (character)
		{
			CompartmentAccessComponent compAccess = character.GetCompartmentAccessComponent();
			if (compAccess && (compAccess.IsGettingIn() || compAccess.IsGettingOut()))
				return false;

			if (!m_bShowFromOutside && !character.IsInVehicle())
				return false;

			if (!m_bShowInside && character.IsInVehicle())
				return false;
		}
			
		if(!m_InventoryManager || !m_WeaponComponent.GetCurrentMagazine())
			return false;
		
		IEntity currentMag = m_WeaponComponent.GetCurrentMagazine().GetOwner();
		InventoryItemComponent magInventory = InventoryItemComponent.Cast(currentMag.FindComponent(InventoryItemComponent));
		BaseInventoryStorageComponent magStorage = magInventory.GetParentSlot().GetStorage();
		WeaponAttachmentsStorageComponent wasc = WeaponAttachmentsStorageComponent.Cast(magStorage);
		if (!wasc)
			return false; // Must be a WeaponAttachmentsStorageComponent

		return m_InventoryManager.CanRemoveItemFromStorage(currentMag, magStorage);
	}

	override protected void PerformActionInternal(SCR_InventoryStorageManagerComponent manager, IEntity pOwnerEntity, IEntity pUserEntity)
	{
		IEntity currentMag = m_WeaponComponent.GetCurrentMagazine().GetOwner();
		InventoryItemComponent magInventory = InventoryItemComponent.Cast(currentMag.FindComponent(InventoryItemComponent));
		BaseInventoryStorageComponent magStorage = magInventory.GetParentSlot().GetStorage();
		
		WeaponAttachmentsStorageComponent wasc = WeaponAttachmentsStorageComponent.Cast(magStorage);
		if (!wasc)
		{
			Print("ERROR: Magazine is no longer in the weapon", LogLevel.ERROR);
			return; // Must be a WeaponAttachmentsStorageComponent
		}
		
		BaseInventoryStorageComponent suitableStorage = m_InventoryManager.FindStorageForItem(currentMag);

		if (suitableStorage)
			m_InventoryManager.TryMoveItemToStorage(currentMag, suitableStorage);
		else
			m_InventoryManager.TryRemoveItemFromStorage(currentMag, magStorage);
		
	}

	override bool GetActionNameScript(out string outName)
	{
		if (!m_WeaponComponent)
			return false;

		if (!m_WeaponComponent.GetCurrentMagazine())
			return false;

		IEntity currentMag = m_WeaponComponent.GetCurrentMagazine().GetOwner();
		if (!currentMag)
			return false;

		InventoryItemComponent magItem = InventoryItemComponent.Cast(currentMag.FindComponent(InventoryItemComponent));
		if (!magItem)
			return false;

		UIInfo actionInfo = GetUIInfo();
		UIInfo itemInfo = magItem.GetUIInfo();
		if (actionInfo && itemInfo)
		{
			outName = string.Format("%1%2", actionInfo.GetName(), itemInfo.GetName());
			return true;
		}
		else
		{
			return false;
		}
	}
	
	protected void DelayedInit(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		if (!Vehicle.Cast(pOwnerEntity) && pOwnerEntity.GetRootParent())
			m_Vehicle = pOwnerEntity.GetRootParent();
		else
			m_Vehicle = pOwnerEntity;
		
		m_InventoryOwner = pOwnerEntity;

		if (!m_Vehicle)
			return;

		m_DamageManager = DamageManagerComponent.Cast(m_Vehicle.FindComponent(DamageManagerComponent));
		
		// Must be done in EOnInit and not OnPostInit so weapons are created
		BaseWeaponManagerComponent weaponManager = BaseWeaponManagerComponent.Cast(pOwnerEntity.FindComponent(BaseWeaponManagerComponent));
		if (!weaponManager) 
			return;
		
		array<WeaponSlotComponent> weaponSlots = {};
		weaponManager.GetWeaponsSlots(weaponSlots);
		
		foreach(WeaponSlotComponent weaponSlot : weaponSlots)
		{
			if (weaponSlot.GetWeaponSlotIndex() == m_iWeaponIndex || weaponSlots.Count() <= 1)
			{
				IEntity weapon = weaponSlot.GetWeaponEntity();
				if (!weapon) 
					return;
				m_WeaponComponent = WeaponComponent.Cast(weapon.FindComponent(WeaponComponent));
			}
		}
	}
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		//~ Call one frame later as otherwise client slotted entity does not know yet who the vehicle is on init
		GetGame().GetCallqueue().CallLater(DelayedInit, param1: pOwnerEntity, param2: pManagerComponent); 
	}
};