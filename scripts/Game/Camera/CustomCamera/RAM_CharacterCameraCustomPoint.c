// Just move camera to point info
class RAM_CharacterCameraCustomPoint extends CharacterCameraBase
{
	protected RAM_CustomCameraPointInfo m_CameraPointInfo;
	protected Widget m_wOverlayWidget;

	override void OnActivate(ScriptedCameraItem pPrevCamera, ScriptedCameraItemResult pPrevCameraResult)
	{
		super.OnActivate(pPrevCamera, pPrevCameraResult);

		m_CameraPointInfo = m_CharacterCameraHandler.RAM_GetCustomPointInfo();

		GetGame().GetInputManager().AddActionListener("CharacterAction", EActionTrigger.PRESSED, ExitCamera);

		if (m_wOverlayWidget)
			m_wOverlayWidget.RemoveFromHierarchy();
		m_wOverlayWidget = null;

		ResourceName overlayWidgetLayout = m_CameraPointInfo.GetCameraOverlayLayout();
		if (overlayWidgetLayout != "")
		{
			m_wOverlayWidget = GetGame().GetWorkspace().CreateWidgets(overlayWidgetLayout);
		}
	}

	override void OnDeactivate(ScriptedCameraItem pNextCamera)
	{
		GetGame().GetInputManager().RemoveActionListener("CharacterAction", EActionTrigger.PRESSED, ExitCamera);

		if (m_wOverlayWidget)
			m_wOverlayWidget.RemoveFromHierarchy();
		m_wOverlayWidget = null;
	}

	override void OnBlendOut()
	{
		if (m_wOverlayWidget)
			m_wOverlayWidget.RemoveFromHierarchy();
		m_wOverlayWidget = null;
	}

	void ExitCamera()
	{
		m_CharacterCameraHandler.RAM_SetCustomPointInfo(null);
	}

	override void OnUpdate(float pDt, out ScriptedCameraItemResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);

		if (m_CameraPointInfo.GetShowCursor())
			g_Game.GetInputManager().ActivateContext("RAM_CustomCameraContext");

		pOutResult.m_fFOV = m_CameraPointInfo.GetCameraFoV();
		pOutResult.m_pWSAttachmentReference = m_CameraPointInfo;
		pOutResult.m_bAllowInterpolation = true;
		pOutResult.m_bAllowCollisionSolver = true;
		pOutResult.m_bWSCameraSmoothing = true;
		pOutResult.m_fUseHeading = 0;

		if (m_CharacterCameraHandler)
			m_CharacterCameraHandler.AddShakeToToTransform(pOutResult.m_CameraTM, pOutResult.m_fFOV);

		// Prevent shooting
		GetGame().GetCallqueue().Call(ClearContext);
	}

	void ClearContext()
	{
		g_Game.GetInputManager().ResetContext("TurretContext");
	}

	void ~RAM_CharacterCameraCustomPoint()
	{
		if (m_wOverlayWidget)
			m_wOverlayWidget.RemoveFromHierarchy();
		m_wOverlayWidget = null;
	}
}
