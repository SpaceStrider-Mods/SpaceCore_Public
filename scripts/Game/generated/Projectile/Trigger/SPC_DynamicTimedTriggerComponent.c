class SPC_DynamicTimedTriggerComponentClass : BaseTriggerComponentClass
{
};

//------------------------------------------------------------------------------------------------
class SPC_DynamicTimedTriggerComponent : BaseTriggerComponent
{
	[Attribute("10", desc: "Time before impact to airburst [seconds]")]
	protected float fuzetimer;
	
	//------------------------------------------------------------------------------------------------
	void EOnSimulate(IEntity owner, float timeSlice)
	{
		vector worldPos = owner.GetOrigin();
		
		autoptr TraceParam param = new TraceParam;
		param.Exclude = owner;
		param.Flags = TraceFlags.WORLD | TraceFlags.OCEAN;
		param.LayerMask = TRACE_LAYER_CAMERA;
		param.Start = worldPos;
		param.End = worldPos + owner.GetPhysics().GetVelocity() * fuzetimer;
		
		#ifdef WORKBENCH
		Shape.CreateArrow(worldPos, worldPos + owner.GetPhysics().GetVelocity() * fuzetimer, 1, Color.RED, ShapeFlags.ONCE | ShapeFlags.NOZBUFFER);
		#endif
		
		float hit = GetGame().GetWorld().TraceMove(param, null);
		//Print(hit);
		if (hit < 1)
		{
			GetGame().GetCallqueue().CallLater(RPC_DoTrigger);
			Rpc(RPC_DoTrigger);
		} 
		
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_DoTrigger()
	{
		BaseTriggerComponent baseTriggerComponent = BaseTriggerComponent.Cast(GetOwner().FindComponent(BaseTriggerComponent));
		if (!baseTriggerComponent)
			return;
		
		baseTriggerComponent.OnUserTriggerOverrideInstigator(GetOwner(), GetInstigator());
	}
	
	//------------------------------------------------------------------------------------------------
	void SPC_DynamicTimedTriggerComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
		SetEventMask(ent, EntityEvent.SIMULATE);
	}
};
