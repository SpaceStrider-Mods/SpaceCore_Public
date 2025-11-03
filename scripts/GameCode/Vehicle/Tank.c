//------------------------------------------------------------------------------------------------
modded enum EVehicleType
{
	TANK			= 65536,
};

//------------------------------------------------------------------------------------------------
class TankClass: VehicleClass
{
};

//------------------------------------------------------------------------------------------------
class Tank : Vehicle
{
	[Attribute("0.04", desc: "Track movement speed based off of wheel movement")]
	protected float m_fTrackSpeed;

    float m_RightTrack = 0;
	float m_LeftTrack = 0;
	VehicleWheeledSimulation m_VehicleSim;
	SignalsManagerComponent m_SignalManager;
	int m_RotationSignalR = -1;
	int m_RotationSignalL = -1;
	float m_OriginalRotationR;
	float m_OriginalRotationL;
	float m_CurrentRotationR;
	float m_CurrentRotationL;

	//------------------------------------------------------------------------------------------------
	void Tank(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.PHYSICSACTIVE | EntityEvent.INIT);
	}
    override void EOnInit(IEntity owner)
    {
        super.EOnInit(owner);

        if (SCR_Global.IsEditMode())
            return;

        SetFlags(EntityFlags.ACTIVE, false);
        SetEventMask(EntityEvent.FRAME);
		m_VehicleSim = VehicleWheeledSimulation.Cast(FindComponent(VehicleWheeledSimulation));


		m_SignalManager = SignalsManagerComponent.Cast(FindComponent(SignalsManagerComponent));
		m_RotationSignalR = m_SignalManager.FindSignal("rotation5");
		m_RotationSignalL = m_SignalManager.FindSignal("rotation4");
		m_OriginalRotationR = m_SignalManager.GetSignalValue(m_RotationSignalR);
		m_OriginalRotationL = m_SignalManager.GetSignalValue(m_RotationSignalL);
		//Print(m_OriginalRotationR);
    };
    override void EOnFrame(IEntity owner, float timeSlice)
    {
		m_CurrentRotationR = m_SignalManager.GetSignalValue(m_RotationSignalR);
		m_CurrentRotationL = m_SignalManager.GetSignalValue(m_RotationSignalL);
        m_RightTrack = m_CurrentRotationR * m_fTrackSpeed;
		m_LeftTrack = m_CurrentRotationL * m_fTrackSpeed;
        //Print(m_RightTrack);
        if (m_RightTrack >= 2048)
            m_RightTrack = 0;
		if (m_LeftTrack >= 2048)
			m_LeftTrack = 0;

		//float steering = VehicleWheeledSimulation.Cast(owner.FindComponent(VehicleWheeledSimulation)).GetSteering();

		//vector angularVelocity = vector.Zero;
		//angularVelocity[1] = steering;

		//owner.GetPhysics().SetAngularVelocity((angularVelocity * timeSlice * 2) + owner.GetPhysics().GetAngularVelocity());

    };
};