//Created as of 5/12/2025
//discord: myvapeblewup
[ComponentEditorProps(category: "Air/Armoured", description: "Single Use engine fire extinguisher system")]
class VAPE_HeliFireExtinguisherComponentClass : ScriptComponentClass {}

class VAPE_HeliFireExtinguisherComponent : ScriptComponent
{
	[RplProp()]
	protected bool m_bUsed = false;

	[Attribute("FE-PLACEHOLDER", UIWidgets.ResourcePickerThumbnail, "Fire Extinguished Effect", "effect", params: "et")]
	protected ResourceName m_FireExtinguishedEffectPrefab; //prefab should preferably be a modified warhead
                                                           //this handles self cleanup that way after use
	protected IEntity m_Owner;

	override void OnPostInit(IEntity owner)
	{
		m_Owner = owner;
		SetEventMask(owner, EntityEvent.INIT);
	}

	bool IsUsed()
	{
		return m_bUsed;
	}

	bool IsOnFire()
	{
		DamageManagerComponent dmg = DamageManagerComponent.Cast(m_Owner.FindComponent(DamageManagerComponent));
		if (!dmg)
			return false;

		array<HitZone> zones = {};            //retreiving the damage manager comp and SCR_FlammableHitZone within
		                                      //were also locating hitzones in this instance its "hull"
		dmg.GetAllHitZones(zones);

		foreach (HitZone zone : zones)
		{
			if (zone.IsInherited(SCR_FlammableHitZone))
			{
				SCR_FlammableHitZone flammable = SCR_FlammableHitZone.Cast(zone);
				if (flammable && flammable.GetFireState() == SCR_EBurningState.BURNING)
					return true;
			}
		}
		return false;
	}

	void TriggerExtinguisher()
	{
		if (!Replication.IsServer())
		{
			Rpc(AskServer_TriggerExtinguisher);
			return;
		}

		if (m_bUsed || !IsOnFire())
			return;

		m_bUsed = true;
		ExtinguishAllFire();

		if (m_FireExtinguishedEffectPrefab != string.Empty)
		{
			Resource effectResource = Resource.Load(m_FireExtinguishedEffectPrefab);
			if (effectResource)
			{
				EntitySpawnParams params = EntitySpawnParams();
				params.Parent = m_Owner;
				params.TransformMode = ETransformMode.LOCAL;
				params.Transform[3] = vector.Zero;

				GetGame().SpawnEntityPrefab(effectResource, GetGame().GetWorld(), params);
			}
			else
			{
				Print("Extinguisher: Failed to load fire extinguished effect resource."); //Debug
			}
		}

		Replication.BumpMe();
		Rpc(RpcClient_NotifyUsed);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void AskServer_TriggerExtinguisher()
	{
		TriggerExtinguisher();
	}

	protected void ExtinguishAllFire()
{
	DamageManagerComponent dmg = DamageManagerComponent.Cast(m_Owner.FindComponent(DamageManagerComponent));
	if (!dmg)
	{
		Print("Extinguisher: No DamageManagerComponent found."); //Debug
		return;
	}

	array<HitZone> zones = {};
	dmg.GetAllHitZones(zones);

	foreach (HitZone zone : zones)
	{
		if (!zone || !zone.IsInherited(SCR_FlammableHitZone))
			continue;

		SCR_FlammableHitZone flammable = SCR_FlammableHitZone.Cast(zone);
		if (!flammable)
			continue;

		Print("Extinguisher: Suppressing fire in zone: " + flammable.GetName()); //System Engaged

		// Were Setting fire rate just before ignition here - vape
		float justBeforeIgnite = flammable.GetFireRateForState(SCR_EBurningState.SMOKING_IGNITING, 0.0);
		flammable.SetFireRate(justBeforeIgnite);

		float currentHealth = flammable.GetHealth();
		float ignitionThreshold = 0.1;
		float buffer = 0.01;

		if (currentHealth < ignitionThreshold + buffer)
			flammable.SetHealth(ignitionThreshold + buffer);
	}
}


	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcClient_NotifyUsed()
	{
		SCR_HintManagerComponent.GetInstance().ShowCustom("Fire suppression system activated.");
	}
}