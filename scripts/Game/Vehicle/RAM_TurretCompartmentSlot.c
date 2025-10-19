class RAM_TurretCompartmentSlot : TurretCompartmentSlot
{
	[Attribute("")]
	bool m_bCameraIgnoreTurretRotation;
	
	[Attribute("")]
	ref RAM_CustomCameraPointInfo m_CustomADSCameraPointInfo;
	
	bool IsCameraRotateByTurret()
	{
		return !m_bCameraIgnoreTurretRotation;
	}
}