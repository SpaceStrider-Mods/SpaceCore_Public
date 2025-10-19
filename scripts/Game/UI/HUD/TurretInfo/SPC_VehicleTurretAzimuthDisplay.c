class SPC_VehicleTurretAzimuthDisplay : SCR_InfoDisplayExtended
{
    protected ref map<string, ImageWidget> m_mTurretImages = new map<string, ImageWidget>();
    protected ref array<TurretControllerComponent> m_aTurretControllers = new array<TurretControllerComponent>();

    [Attribute(desc: "List of allowed turret unique names to be displayed.")]
    protected ref array<string> m_aAllowedTurrets = new array<string>();

    //------------------------------------------------------------------------------------------------
    override void DisplayStartDraw(IEntity owner)
    {
        if (!m_wRoot || !owner)
            return;

        m_mTurretImages.Clear();
        m_aTurretControllers.Clear();

        // Get the root vehicle entity
        IEntity vehicleRoot = owner.GetRootParent();
        if (!vehicleRoot)
            return;

        // Find all turret controllers, including those nested in other turrets
        FindAllTurretsRecursive(vehicleRoot);

        // Debugging: Print all found turret names
        //Print("Found Turrets:");
        foreach (TurretControllerComponent turretController : m_aTurretControllers)
        {
            Print(turretController.GetUniqueName());
        }

        // Recursively find all image widgets
        FindAllImageWidgets(m_wRoot);
        
        // Debugging: Print all found widget names
        //Print("Found Widgets:");
        foreach (string widgetName, ImageWidget widget : m_mTurretImages)
        {
            Print(widgetName);
        }
    }

    //------------------------------------------------------------------------------------------------
	
	protected void FindAllTurretsRecursive(IEntity entity)
	{
	    if (!entity)
	        return;
	
	    array<Managed> foundComponents = {};
	    entity.FindComponents(TurretControllerComponent, foundComponents);
	
	    foreach (Managed comp : foundComponents)
	    {
	        TurretControllerComponent turretController = TurretControllerComponent.Cast(comp);
	        if (turretController)
	        {
	            string turretName = turretController.GetUniqueName();
	            
	            // Debug Log
	            //PrintFormat("Detected Turret: %1", turretName);
	
	            if (m_aAllowedTurrets.Contains(turretName))
	                m_aTurretControllers.Insert(turretController);
	        }
	    }
	
	    // **Check all child entities to make sure we find nested turrets**
	    IEntity child = entity.GetChildren();
	    while (child)
	    {
	        FindAllTurretsRecursive(child); // Recursive call for deeper turrets
	        child = child.GetSibling();
	    }
	}



    //------------------------------------------------------------------------------------------------
    protected void FindAllImageWidgets(Widget root)
    {
        if (!root)
            return;

        Widget child = root.GetChildren();
        while (child)
        {
            ImageWidget image = ImageWidget.Cast(child);
            if (image)
            {
                string widgetName = child.GetName();
                if (!widgetName.IsEmpty())
                {
                    m_mTurretImages.Insert(widgetName, image);
                }
            }

            // Recursively search child widgets
            FindAllImageWidgets(child.GetChildren());

            child = child.GetSibling();
        }
    }

    //------------------------------------------------------------------------------------------------
    protected float GetTurretWorldAzimuth(TurretComponent turret)
    {
        if (!turret)
            return 0.0;

        vector worldDirection = turret.GetAimingDirectionWorld();
        float azimuth = worldDirection.ToYaw();

        // Normalize azimuth to [0, 360]
        azimuth = Math.MapAngle(azimuth);
        return azimuth;
    }

    //------------------------------------------------------------------------------------------------
    override void DisplayUpdate(IEntity owner, float timeSlice)
    {
        if (!owner || m_mTurretImages.IsEmpty() || m_aTurretControllers.IsEmpty())
        {
            //Print("DisplayUpdate skipped: Missing widgets or turrets.");
            return;
        }

        foreach (TurretControllerComponent turretController : m_aTurretControllers)
        {
            TurretComponent turret = turretController.GetTurretComponent();
            if (!turret)
                continue;

            string turretName = turretController.GetUniqueName();
            if (!m_mTurretImages.Contains(turretName))
                continue;

            float azimuth = GetTurretWorldAzimuth(turret);

            // Debugging - check if angles are updating
            //PrintFormat("Updating Turret: %1 - Azimuth: %2", turretName, azimuth);

            // Set rotation for the corresponding turret image
            m_mTurretImages[turretName].SetRotation(azimuth);
        }
    }

    //------------------------------------------------------------------------------------------------
    protected override void DisplayStopDraw(IEntity owner)
    {
        if (m_wRoot)
            m_wRoot.RemoveFromHierarchy();
    }
}
