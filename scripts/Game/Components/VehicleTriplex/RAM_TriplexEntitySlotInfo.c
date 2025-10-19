class RAM_TriplexEntitySlotInfo : RegisteringComponentSlotInfo
{
	[Attribute("")]
	ref RAM_CustomCameraPointInfo m_CameraPointInfo;
	
	override void OnAttachedEntity(IEntity entity) {
		super.OnAttachedEntity(entity);
		
		if (!m_CameraPointInfo)
			return;
		m_CameraPointInfo.Init(GetOwner());
		
		GetGame().GetCallqueue().Call(SetCamera, entity);
	};
	
	void SetCamera(IEntity entity)
	{
		ActionsManagerComponent actionsManagerComponent = ActionsManagerComponent.Cast(entity.FindComponent(ActionsManagerComponent));
		if (!actionsManagerComponent)
			return;
		
		array<BaseUserAction> outActions = {};
		actionsManagerComponent.GetActionsList(outActions);
		foreach (BaseUserAction action : outActions)
		{
			RAM_TriplexAction triplexAction = RAM_TriplexAction.Cast(action);
			if (triplexAction)
				triplexAction.SetCameraPointInfo(m_CameraPointInfo);
		}
	}
}