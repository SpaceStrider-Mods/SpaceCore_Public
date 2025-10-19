class SPC_TurretInfoWidgets
{
	protected static const ResourceName LAYOUT = "{5F36F4AF68A435FB}UI/layouts/HUD/TurretInfo/TurretInfo.layout";

	OverlayWidget m_wElevation;
	OverlayWidget m_wAzimuth;
	OverlayWidget m_wRange;

	//------------------------------------------------------------------------------------------------
	bool Init(Widget root)
	{
		m_wElevation = OverlayWidget.Cast(root.FindWidget("Overlay.m_Elevation"));
		m_wAzimuth = OverlayWidget.Cast(root.FindWidget("Overlay.m_Azimuth"));
		m_wRange = OverlayWidget.Cast(root.FindWidget("Overlay.m_Range"));

		return true;
	}

	//------------------------------------------------------------------------------------------------
	ResourceName GetLayout()
	{
		return LAYOUT;
	}
}