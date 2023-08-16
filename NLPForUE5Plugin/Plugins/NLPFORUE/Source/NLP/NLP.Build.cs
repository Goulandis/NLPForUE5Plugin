using UnrealBuildTool;

public class NLP : ModuleRules
{
	public NLP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new []
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});
		//PrivateDependencyModuleNames.AddRange();
	}
}