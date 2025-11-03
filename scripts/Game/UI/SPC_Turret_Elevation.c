class SPC_TurretElevation : SCR_InfoDisplay
{	

	private RichTextWidget m_bearing = null;
	private RichTextWidget m_elevation = null;
	
	private SCR_CharacterControllerComponent m_CharacterController = null;
	protected TurretComponent m_Turret = null;
	protected BaseWeaponManagerComponent m_WeaponManager = null;
	
	static const float milsPerDeg = 17.777778;

	void updateBearing(string s_bearing)
	{
		m_bearing.SetText("AZ: " + s_bearing);
	}
	
	void updateElevation(string s_elevation)
	{
		m_elevation.SetText("EL: " + s_elevation)
	}
	
	override event void OnStartDraw(IEntity owner)
	{
		super.OnStartDraw(owner);
		
		m_bearing = RichTextWidget.Cast(m_wRoot.FindWidget("Bearing"));
		m_elevation = RichTextWidget.Cast(m_wRoot.FindWidget("Elevation"));
		

		
		if(!owner)
			return;
		
		m_Turret = TurretComponent.Cast(owner.FindComponent(TurretComponent));
		if(!m_Turret)
			return;	
		
		Print("We have a turret!");

		TurretControllerComponent turretController = TurretControllerComponent.Cast(owner.FindComponent(TurretControllerComponent));
		if(!turretController)
			return;
		
		BaseCompartmentSlot slot = turretController.GetCompartmentSlot();
		if(!slot)
			return;
		
		ChimeraCharacter character = ChimeraCharacter.Cast(slot.GetOccupant());
		if(!character)
			return;
	}
	
	override event void UpdateValues(IEntity owner, float timeSlice)
	{
		if (!m_Turret)
			m_Turret = TurretComponent.Cast(owner.FindComponent(TurretComponent));
		if (!m_Turret)
			return;
	
		vector bearing = m_Turret.GetAimingRotation();
		if (bearing.LengthSq() < 0.0001)
			return;
	
		vector tform[3];
		owner.GetWorldTransform(tform);
	
		if (tform[0].LengthSq() < 0.0001 || tform[1].LengthSq() < 0.0001 || tform[2].LengthSq() < 0.0001)
			return;
	
		vector turretLocalTForm[3];
		Math3D.AnglesToMatrix(bearing, turretLocalTForm);
	
		if (turretLocalTForm[0].LengthSq() < 0.0001 || turretLocalTForm[1].LengthSq() < 0.0001 || turretLocalTForm[2].LengthSq() < 0.0001)
			return;
	
		vector globalAimTForm[3];
		Math3D.MatrixMultiply3(tform, turretLocalTForm, globalAimTForm);
	
		if (globalAimTForm[0].LengthSq() < 0.0001)
			return;
	
		vector globalBearing = Math3D.MatrixToAngles(globalAimTForm);
	
		while (globalBearing[0] < 0)
			globalBearing[0] = globalBearing[0] + 360;
	
		updateBearing((Math.Round(globalBearing[0] * 100) / 100).ToString());
		updateElevation(globalBearing[1].ToString());
	}
}
