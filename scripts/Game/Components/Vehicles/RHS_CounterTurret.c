//------------------------------------------------------------------------------------------------
[EntityEditorProps(category: "Components/RHS_CounterTurretComponentClass", description: "Component handlinng move of clock hands.")]
class RHS_CounterTurretComponentClass : ScriptComponentClass
{
};

//------------------------------------------------------------------------------------------------

class RHS_CounterTurretComponent : ScriptComponent
{

	protected SignalsManagerComponent m_SignalManager;
	protected SignalsManagerComponent m_SignalManagerParent;
	protected int m_iTurretHorizontal;
	protected int m_iTurretVertical;
	protected int m_iTurretHorizontalParent;
	protected int m_iTurretVerticalParent;
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
	}

	override void EOnFrame(IEntity owner, float timeSlice) //!EntityEvent.FRAME
	{	
		if (!m_SignalManagerParent || !m_SignalManager)
		{
			SetupSignals(owner);
			return;
		}
		
		m_SignalManager.SetSignalValue(m_iTurretHorizontal, -m_SignalManagerParent.GetSignalValue(m_iTurretHorizontalParent));
		m_SignalManager.SetSignalValue(m_iTurretVertical, -m_SignalManagerParent.GetSignalValue(m_iTurretVerticalParent));
	}
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		SetupSignals(owner);
		//PrintFormat("%1 %2 %3 %4",m_iTurretHorizontal,m_iTurretVertical,m_iTurretHorizontalParent,m_iTurretVerticalParent);
	}
	void SetupSignals(IEntity owner)
	{
		IEntity parent = owner.GetParent();
		if (!parent)
			return;
		
		m_SignalManagerParent = SignalsManagerComponent.Cast(parent.FindComponent(SignalsManagerComponent));
		if (!m_SignalManagerParent)
			return;
		
		m_SignalManager = SignalsManagerComponent.Cast(owner.FindComponent(SignalsManagerComponent));
		if (!m_SignalManager)
			return;
		
		m_iTurretHorizontal = m_SignalManager.AddOrFindSignal("AimTurnTurret");
		m_iTurretVertical = m_SignalManager.AddOrFindSignal("AimElevationTurret");
		
		m_iTurretHorizontalParent = m_SignalManagerParent.AddOrFindSignal("AimTurnTurret");
		m_iTurretVerticalParent = m_SignalManagerParent.AddOrFindSignal("AimElevationTurret");
	}
};
