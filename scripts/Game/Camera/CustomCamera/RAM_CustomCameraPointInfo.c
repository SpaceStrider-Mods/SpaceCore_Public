// PointInfo with FoV for vehicle cameras setup
class RAM_CustomCameraPointInfo : PointInfo
{
	[Attribute("")]
	protected float m_fCameraFoV;
	
	[Attribute("")]
	protected float m_fRotationAngle;
	
	[Attribute("")]
	protected string m_sCameraName;
	
	[Attribute("")]
	protected bool m_bShowCursor;
	
	[Attribute("")]
	protected ResourceName m_rCameraOverlayLayout;
	
	[Attribute("")]
	protected bool m_bInstantMoveCamera;
	
	[Attribute("")]
	protected string m_sSlotName;
	
	float GetCameraFoV()
	{
		return m_fCameraFoV;
	}
	
	float GetRotationAngle()
	{
		return m_fRotationAngle;
	}
	
	string GetCameraName()
	{
		return m_sCameraName;
	}
	
	bool GetShowCursor()
	{
		return m_bShowCursor;
	}
	
	ResourceName GetCameraOverlayLayout()
	{
		return m_rCameraOverlayLayout;
	}
	
	bool IsCameraMoveInstant()
	{
		return m_bInstantMoveCamera;
	}
	
	string GetSlotName()
	{
		return m_sSlotName;
	}
}