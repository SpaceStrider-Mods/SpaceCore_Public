void RAM_ScriptInvokerCustomCameraExitMethod(SCR_CharacterCameraHandlerComponent characterCameraHandlerComponent);
typedef func RAM_ScriptInvokerCustomCameraExitMethod;
typedef ScriptInvokerBase<RAM_ScriptInvokerCustomCameraExitMethod> RAM_ScriptInvokerCustomCameraExit;

// Add ability to attach camera to custom point info
modded class SCR_CharacterCameraHandlerComponent
{
	RAM_CustomCameraPointInfo m_RAMPointInfo;
	protected int m_iLastCustomCameraTime;
	protected const int CAMERA_SWITCH_DELAY = 200;
	protected ref RAM_ScriptInvokerCustomCameraExit m_RAMCustomCameraExit = new RAM_ScriptInvokerCustomCameraExit();

	//------------------------------------------------------------------------------------------------
	bool RAM_IsCanSwitch()
	{
		int currentTime = GetGame().GetWorld().GetWorldTime();
		return CAMERA_SWITCH_DELAY < currentTime - m_iLastCustomCameraTime;
	}

	//------------------------------------------------------------------------------------------------
	RAM_CustomCameraPointInfo RAM_GetCustomPointInfo()
	{
		return m_RAMPointInfo;
	}

	//------------------------------------------------------------------------------------------------
	void RAM_SetCustomPointInfo(RAM_CustomCameraPointInfo pointInfo)
	{
		m_iLastCustomCameraTime = GetGame().GetWorld().GetWorldTime();
		m_RAMPointInfo = pointInfo;
		if (!m_RAMPointInfo)
			m_RAMCustomCameraExit.Invoke(this);
	}
	
	//------------------------------------------------------------------------------------------------
	RAM_ScriptInvokerCustomCameraExit RAM_GetOnCustomCameraExit()
	{
		return m_RAMCustomCameraExit;
	}

	//------------------------------------------------------------------------------------------------
	bool RAM_IsTurretCompartmentRotate()
	{
		CompartmentAccessComponent compartmentAccess = m_OwnerCharacter.GetCompartmentAccessComponent();
		if (!compartmentAccess)
			return true;

		RAM_TurretCompartmentSlot compartment = RAM_TurretCompartmentSlot.Cast(compartmentAccess.GetCompartment());
		if (!compartment)
			return true;

		return compartment.IsCameraRotateByTurret();
	}

	//------------------------------------------------------------------------------------------------
	override int CameraSelector()
	{
		int cameraNum = super.CameraSelector();

		// Custom turret comparment without camera to turret rotation
		if (cameraNum == CharacterCameraSet.CHARACTERCAMERA_1ST_TURRET || cameraNum == CharacterCameraSet.CHARACTERCAMERA_ADS_VEHICLE)
		{
			if (!RAM_IsTurretCompartmentRotate())
			{
				if (cameraNum == CharacterCameraSet.CHARACTERCAMERA_1ST_TURRET)
					cameraNum = CharacterCameraSet.CHARACTERCAMERA_1ST_VEHICLE;
				else
				{
				}
			}
		}

		// Detach camera if not in vehicle
		if (m_RAMPointInfo)
		{
			if (cameraNum != CharacterCameraSet.CHARACTERCAMERA_1ST_VEHICLE)
				RAM_SetCustomPointInfo(null);
			else
			{
				if (m_RAMPointInfo.IsCameraMoveInstant())
					return CharacterCameraSet.CHARACTERCAMERA_1ST_RAM_INSTANT;
				else
					return CharacterCameraSet.CHARACTERCAMERA_1ST_RAM;
			}
		}

		return cameraNum;
	} 
}
