modded class CharacterCameraSet
{
	// Is it safe? I think no...
	static const int CHARACTERCAMERA_1ST_RAM			= 38;
	static const int CHARACTERCAMERA_1ST_RAM_INSTANT	= 39;
	static const int CHARACTERCAMERA_ADS_VEHICLE_PIP	= 40;

	//------------------------------------------------------------------------------------------------
	override void Init()
	{
		super.Init();

		// Register our custom cameras
		RegisterCameraCreator(CHARACTERCAMERA_1ST_RAM, RAM_CharacterCameraCustomPoint);
		RegisterCameraCreator(CHARACTERCAMERA_1ST_RAM_INSTANT, RAM_CharacterCameraCustomPoint);

		RegisterTransitionTime(CHARACTERCAMERA_1ST_VEHICLE, CHARACTERCAMERA_1ST_RAM_INSTANT, 0.0, true);
	}
}
