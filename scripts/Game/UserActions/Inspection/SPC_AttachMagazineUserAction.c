class SPC_RearmFromVehicleInventoryAction : SCR_InventoryAction
{
	
	[Attribute(desc: "Defines the resource name of the projectile type to be used for rearming at the station.")]
	protected ResourceName m_sCompatibleProjectiles;
	
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
	
	//------------------------------------------------------------------------------------------------
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		//~ Call one frame later as otherwise client slotted entity does not know yet who the vehicle is on init
		GetGame().GetCallqueue().CallLater(DelayedInit, param1: pOwnerEntity, param2: pManagerComponent); 
	}
	//------------------------------------------------------------------------------------------------
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

	//------------------------------------------------------------------------------------------------
	override protected void PerformActionInternal(SCR_InventoryStorageManagerComponent manager, IEntity pOwnerEntity, IEntity pUserEntity)
	{
		WeaponAttachmentsStorageComponent storage = WeaponAttachmentsStorageComponent.Cast(m_WeaponComponent.GetOwner().FindComponent(WeaponAttachmentsStorageComponent));
		SCR_PrefabNamePredicate predicate = new SCR_PrefabNamePredicate();
		predicate.prefabName = m_sCompatibleProjectiles;
		IEntity mag = m_InventoryManager.FindItem(predicate);
		if (!m_InventoryManager.CanMoveItemToStorage(mag, storage, -1))
		{
			BaseMagazineComponent magazine = BaseMagazineComponent.Cast(mag.FindComponent(BaseMagazineComponent));
			TurretControllerComponent ctrl = TurretControllerComponent.Cast(pOwnerEntity.FindComponent(TurretControllerComponent));
			if (ctrl && magazine)
			{
				bool bEmpty = storage.Get(0) == null;
				ctrl.DoReloadWeaponWith(mag);
			}
			else
			{
				m_InventoryManager.TrySwapItemStorages(mag, storage.Get(0));
			}
			manager.PlayItemSound(mag, "SOUND_SWITCH");
		}
		else
		{
			// Check if we want to reload a magazine, and use ReloadWeapopnWith if so.
			// This prevents a race condition were the old magazine appears to be still in the weapon
			// due to network lag
			BaseMagazineComponent magazine = BaseMagazineComponent.Cast(mag.FindComponent(BaseMagazineComponent));
			TurretControllerComponent ctrl = TurretControllerComponent.Cast(pOwnerEntity.FindComponent(TurretControllerComponent));
			if (ctrl && magazine)
			{
				bool bEmpty = storage.Get(0) == null;
				ctrl.DoReloadWeaponWith(mag);
			}
			else
			{
				manager.EquipWeaponAttachment(mag, pUserEntity);		
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if (!user || !m_Vehicle || !m_InventoryOwner)
			return false;

		if (m_DamageManager && m_DamageManager.GetState() == EDamageState.DESTROYED)
			return false;
		
		if(!m_InventoryManager)
		{
			m_InventoryManager = SCR_VehicleInventoryStorageManagerComponent.Cast(m_Vehicle.FindComponent(SCR_VehicleInventoryStorageManagerComponent));
		}
		
		SCR_PrefabNamePredicate predicate = new SCR_PrefabNamePredicate();
		predicate.prefabName = m_sCompatibleProjectiles;
		if (m_InventoryManager.CountItem(predicate) == 0)
			return false;
		
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
		
		SCR_BaseCompartmentManagerComponent compMan = SCR_BaseCompartmentManagerComponent.Cast(m_InventoryOwner.FindComponent(SCR_BaseCompartmentManagerComponent));
		
		array<BaseCompartmentSlot> compartmentSlots = {};
		compMan.GetCompartmentsOfType(compartmentSlots, ECompartmentType.TURRET);
		if (!(compartmentSlots[0]).IsOccupied())
			return false;
			
		if(!m_InventoryManager || !m_WeaponComponent.GetCurrentMagazine())
			return false;
		
		return true;
	}
	
	override bool GetActionNameScript(out string outName)
	{
		SCR_PrefabNamePredicate predicate = new SCR_PrefabNamePredicate();
		predicate.prefabName = m_sCompatibleProjectiles;
		IEntity mag = m_InventoryManager.FindItem(predicate);
		InventoryItemComponent m_ItemComponent = InventoryItemComponent.Cast(mag.FindComponent(InventoryItemComponent));
		if (!m_ItemComponent)
			return false;
		UIInfo actionInfo = GetUIInfo();
		UIInfo itemInfo = m_ItemComponent.GetUIInfo();
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
}
