//! Handles UI displaying turret values
//! - azimuth, elevation, range
class SPC_TurretInfo : SCR_InfoDisplayExtended
{
	[Attribute(defvalue: "580", desc: "How far should elements be placed from center\n[px]")]
	protected float m_fElementsRadius;	// TODO: Convert from PX to angle

	[Attribute(defvalue: "6400", desc: "Mils in full 360 degrees angle \nSoviet: 6000\nNATO: 6400")]
	protected float m_fMils;
	
	protected ref SPC_TurretInfoWidgets m_Widgets = new SPC_TurretInfoWidgets();

	//------------------------------------------------------------------------------------------------
	void SetElementsRadius(float radius)
	{
		m_fElementsRadius = radius;

		// Set elements
		FrameSlot.SetPosY(m_Widgets.m_wAzimuth, radius); // down
		FrameSlot.SetPosX(m_Widgets.m_wElevation, -radius); // left
		FrameSlot.SetPosX(m_Widgets.m_wRange, radius); // right
	}

	//------------------------------------------------------------------------------------------------
	override void DisplayStartDraw(IEntity owner)
	{
		if (!m_wRoot)
			return;
		
		m_Widgets = new SPC_TurretInfoWidgets();
		m_Widgets.Init(m_wRoot);

		SetElementsRadius(m_fElementsRadius);
	}
	
	//------------------------------------------------------------------------------------------------
	float GetMils()
	{
		return m_fMils;
	}
}