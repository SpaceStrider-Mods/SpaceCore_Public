// Use turret sight FoV as camera FoV
class RAM_CustomCameraPointInfoTurret : RAM_CustomCameraPointInfo
{
	IEntity m_TurretEntity;
	TurretComponent m_Turret;
	SCR_2DSightsComponent m_Sight;
	SCR_SightsZoomFOVInfo m_SightsZoomFOVInfo;
	
	// Need be called from turret
	void InitTurret(IEntity owner)
	{
		m_TurretEntity = owner;
		if (!m_TurretEntity)
			return;
		
		m_Turret = TurretComponent.Cast(m_TurretEntity.FindComponent(TurretComponent));
		if (!m_Turret)
			return;
		
		m_Sight = SCR_2DSightsComponent.Cast(m_Turret.FindComponent(SCR_2DSightsComponent));
		if (!m_Sight)
			return;
		
		SightsFOVInfo FOVInfo = m_Sight.GetFOVInfo();
		if (!FOVInfo)
			return;
		
		m_SightsZoomFOVInfo = SCR_SightsZoomFOVInfo.Cast(FOVInfo);
	}
	
	override float GetCameraFoV()
	{
		if (!m_SightsZoomFOVInfo || m_SightsZoomFOVInfo.GetCount() == 0)
			return super.GetCameraFoV();
		return m_SightsZoomFOVInfo.GetFOV();
	}
}