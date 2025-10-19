class RAM_TriplexAction : SCR_ScriptedUserAction
{
	RAM_CustomCameraPointInfo m_CameraPointInfo;
	SignalsManagerComponent m_SignalsManagerComponent;
	protected int m_iIsUsed;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_SignalsManagerComponent = SignalsManagerComponent.Cast(pOwnerEntity.FindComponent(SignalsManagerComponent));
		if (m_SignalsManagerComponent)
			m_iIsUsed = m_SignalsManagerComponent.AddOrFindMPSignal("IsUsed", 0.5, 1.0, 0, SignalCompressionFunc.Range01);
	}
	
	void SetCameraPointInfo(RAM_CustomCameraPointInfo cameraPointInfo)
	{
		m_CameraPointInfo = cameraPointInfo;
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		SCR_CharacterCameraHandlerComponent characterCameraHandlerComponent = SCR_CharacterCameraHandlerComponent.Cast(pUserEntity.FindComponent(SCR_CharacterCameraHandlerComponent));
		if (!characterCameraHandlerComponent)
			return;
		characterCameraHandlerComponent.RAM_SetCustomPointInfo(m_CameraPointInfo);
		characterCameraHandlerComponent.RAM_GetOnCustomCameraExit().Insert(OnCustomCameraExit);
		if (m_SignalsManagerComponent)
			m_SignalsManagerComponent.SetSignalValue(m_iIsUsed, 1);
	}
	
	void OnCustomCameraExit(SCR_CharacterCameraHandlerComponent characterCameraHandlerComponent)
	{
		characterCameraHandlerComponent.RAM_GetOnCustomCameraExit().Remove(OnCustomCameraExit);
		if (m_SignalsManagerComponent)
			m_SignalsManagerComponent.SetSignalValue(m_iIsUsed, 0);
	}

	override bool GetActionNameScript(out string outName)
	{
		return true;
	}
}