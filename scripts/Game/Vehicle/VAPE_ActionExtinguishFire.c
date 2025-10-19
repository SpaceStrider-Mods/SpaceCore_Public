//Created as of 5/12/2025
//discord: myvapeblewup
class VAPE_ActionExtinguishFire : ScriptedUserAction
{
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        VAPE_HeliFireExtinguisherComponent comp = VAPE_HeliFireExtinguisherComponent.Cast(pOwnerEntity.FindComponent(VAPE_HeliFireExtinguisherComponent));
        if (comp)
            comp.TriggerExtinguisher();
    }

    override bool CanBePerformedScript(IEntity user)
    {
        VAPE_HeliFireExtinguisherComponent comp = VAPE_HeliFireExtinguisherComponent.Cast(GetOwner().FindComponent(VAPE_HeliFireExtinguisherComponent));
        if (!comp)
            return false;

        return !comp.IsUsed() && comp.IsOnFire();
    }

    override bool CanBeShownScript(IEntity user)
    {
        VAPE_HeliFireExtinguisherComponent comp = VAPE_HeliFireExtinguisherComponent.Cast(GetOwner().FindComponent(VAPE_HeliFireExtinguisherComponent));
        if (!comp)
            return false;

        return !comp.IsUsed();
    }
}